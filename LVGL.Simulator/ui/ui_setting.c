#include "ui.h"

lv_obj_t* ui_setting_container;
lv_obj_t* ui_menu;
lv_obj_t* setting_page;

static lv_timer_t* anim_timer = NULL; // 用于存储定时器指针
static lv_timer_t* dele_timer = NULL; // 用于存储定时器指针


static lv_style_t style_default;
static lv_style_t style_btn;
static lv_style_t style_cont;
static lv_style_t style_radio;
static lv_style_t style_radio_chk;
static uint32_t active_index_1 = 0;
static uint32_t active_index_2 = 0;

/* 设置页面group */
static lv_group_t* setting_group;
static lv_group_t* list_group;
static lv_indev_t* touch_device;
static lv_indev_t* button_device;

// 全局指针，便于销毁遮罩和list
static lv_obj_t* menu = NULL;
static lv_obj_t* mask = NULL;
static lv_obj_t* list = NULL;

/* 最大连接数 */
static uint8_t max_connect_array[] = {
    1, 2, 3, 4, 5
};

/* 静态全局 */
static void ui_audio_page(lv_obj_t* parent);



/* 回调函数组 */
static void root_back_bottom_cb(lv_event_t* e);


/**********************************************************************************************************************************/
// 按钮回调函数
static void audio_button_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* user_data = lv_event_get_user_data(e); // 获取另一个按钮的指针

    if (code == LV_EVENT_CLICKED) {
        // 设置当前按钮为选中状态
        lv_obj_add_state(btn, LV_STATE_CHECKED);
        lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_CHECKED);
        lv_obj_set_style_outline_width(btn, 2, LV_STATE_CHECKED);
        lv_obj_set_style_outline_color(btn, lv_color_hex(0x8BC5F4), LV_STATE_CHECKED);
        lv_obj_set_style_text_color(lv_obj_get_child(btn, 0), lv_color_hex(0x8BC5F4), 0);
        lv_obj_set_style_text_color(lv_obj_get_child(btn, 1), lv_color_hex(0x8BC5F4), 0);
        
        // 清除另一个按钮的选中状态
        lv_obj_clear_state(user_data, LV_STATE_CHECKED);
        lv_obj_set_style_bg_color(user_data, lv_color_hex(0xEBEBEB), 0); // 灰色背景
        lv_obj_set_style_outline_width(user_data, 0, 0);
        lv_obj_set_style_text_color(lv_obj_get_child(user_data, 0), lv_color_hex(0xC4C4C4), 0);
        lv_obj_set_style_text_color(lv_obj_get_child(user_data, 1), lv_color_hex(0xC4C4C4), 0);
    }
}

// 焦点变化回调函数
static void focus_cb(lv_group_t* group)
{
    lv_obj_t* focused = lv_group_get_focused(group);
    if (!focused) return;

    // 方法1：直接滚动到对象
    //lv_obj_scroll_to_view(focused, LV_ANIM_ON);

    // 方法2：递归滚动（适用于嵌套容器）
    lv_obj_scroll_to_view_recursive(focused, LV_ANIM_ON);
}

/* 回调函数组 */
static void anim_y_cb(void* var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void timer_del_cb(lv_timer_t* timer)
{
    LV_UNUSED(timer);

    if (list_group) lv_group_del(list_group); // 删除局部组

    if (touch_device) lv_indev_set_group(touch_device, setting_group); // 恢复触摸设备的输入组
    if (button_device) lv_indev_set_group(button_device, setting_group); // 恢复按钮设备的输入组

    if (mask) {
        // 解冻 group，恢复正常焦点流转
        if (setting_group) {
            lv_group_focus_freeze(setting_group, false);

            lv_obj_del(mask);
            mask = NULL;
            list = NULL;
        }
    }

    if (timer) {
        lv_timer_del(timer);    // 删除定时器
        dele_timer = NULL;           // 清空定时器指针
    }
}

static void timer_process_cb(lv_timer_t* timer)
{
    LV_UNUSED(timer);

    /* 启动删除前的动画 */
    lv_coord_t mask_h = lv_obj_get_height(mask);
    lv_coord_t list_h = lv_obj_get_height(list);
    //lv_coord_t mask_h = 320;
    //lv_coord_t list_h = 320 * 0.7;
    lv_coord_t target_y = (mask_h - list_h) / 2;

    // 动画移动到居中
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, list);
    lv_anim_set_values(&a, -target_y, list_h);
    lv_anim_set_time(&a, 500);
    lv_anim_set_exec_cb(&a, anim_y_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);

    if (timer) {
        lv_timer_del(timer);    // 删除定时器
        anim_timer = NULL;           // 清空定时器指针
    }
}

static lv_indev_t * get_input_device(lv_indev_type_t indev_type)
{
    lv_indev_t * device = NULL;

    do {
        device = lv_indev_get_next(device);
        if (device->driver->type == indev_type) {
            break; // 找到第一个合适的输入设备
        }
    } while (device != NULL);

    return device;
}

// list选项点击事件：退出动画，启动删除定时器
static void list_btn_event_cb(lv_event_t* e)
{
    /* 同步锁，防止定时器异步 */
    if (!anim_timer && !dele_timer) {
        if (!anim_timer) {
            anim_timer = lv_timer_create(timer_process_cb, 200, NULL); // 创建一个空定时器，防止遮罩被重复删除
        }

        if (!dele_timer) {
            dele_timer = lv_timer_create(timer_del_cb, 700, NULL); // 创建一个空定时器，防止遮罩被重复删除
        }
    }

    /* 设定复选框为被选中的列表子项 */
}

static void max_connect_cb(lv_event_t * e)
{
    // 防止重复创建
    if (mask)
        return;

    /* 冻结全局组的输入权限 */
    lv_group_focus_freeze(setting_group, true);

    /* 建立局部组，增加输入设备 */
    list_group = lv_group_create();                 /* 创建一个新的list_group */
    lv_group_set_default(list_group);               /* 设置为默认的group */
    lv_indev_set_group(touch_device, list_group);   /* 将输入设置为当前group */
    lv_indev_set_group(button_device, list_group);  /* 将输入设置为当前group */

    lv_obj_t * parent = lv_layer_top(); // 使用顶层，确保遮罩在最上层

    // 创建半透明遮罩
    mask = lv_obj_create(parent);
    lv_obj_remove_style_all(mask);
    lv_obj_set_style_bg_opa(mask, LV_OPA_50, 0);
    lv_obj_set_style_bg_color(mask, lv_color_black(), 0);
    lv_obj_set_size(mask, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_border_width(mask, 0, 0);

    // 点击遮罩空白处也关闭
    lv_obj_add_flag(mask, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(mask, list_btn_event_cb, LV_EVENT_CLICKED, &active_index_1);

    /* 创建列表 */
    list = lv_list_create(mask);
    lv_obj_set_size(list, LV_PCT(70), LV_PCT(70));
    //lv_obj_center(list);

    /* 设置列表样式 */
    lv_style_init(&style_default);

    /* 设置前后左右四边边距 */
    lv_style_set_pad_top(&style_default, 0);
    lv_style_set_pad_left(&style_default, 0);
    lv_style_set_pad_right(&style_default, 0);
    lv_style_set_pad_bottom(&style_default, 0);

    /* 设置背景参数 */
    lv_style_set_bg_opa(&style_default, 255);   // 半透明度设置，不可见容器
    lv_style_set_bg_color(&style_default, lv_color_hex(0xffffff));  // 背景颜色设置
    lv_style_set_bg_grad_dir(&style_default, LV_GRAD_DIR_NONE); // 渐变方向设置

    /* 边框参数设置 */
    lv_style_set_border_width(&style_default, 0);   // 边框宽度设置
    lv_style_set_border_side(&style_default, LV_BORDER_SIDE_NONE);  // 边框边设置
    lv_style_set_radius(&style_default, 6); // 圆角半径设置
    lv_style_set_shadow_width(&style_default, 0);   // 阴影宽度设置
    lv_obj_add_style(list, &style_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置列表样式 */
    lv_style_init(&style_btn);

    /* 设置前后左右四边边距 */
    lv_style_set_pad_top(&style_btn, 0);
    lv_style_set_pad_left(&style_btn, 0);
    lv_style_set_pad_right(&style_btn, 0);
    lv_style_set_pad_bottom(&style_btn, 0);

    /* 设置背景参数 */
    lv_style_set_bg_opa(&style_btn, 255);   // 半透明度设置，不可见容器
    lv_style_set_bg_color(&style_btn, lv_color_hex(0xffffff));  // 背景颜色设置
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_NONE); // 渐变方向设置

    /* 边框参数设置 */
    lv_style_set_border_width(&style_btn, 0);   // 边框宽度设置
    lv_style_set_border_side(&style_btn, LV_BORDER_SIDE_NONE);  // 边框边设置
    lv_style_set_radius(&style_btn, 0); // 圆角半径设置
    lv_style_set_shadow_width(&style_btn, 0);   // 阴影宽度设置


    /**************************************************************************************************************************/
    /* 复选框样式 */
    lv_style_init(&style_radio);
    lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE);

    lv_style_init(&style_radio_chk);
    //lv_style_set_bg_image_src(&style_radio_chk, NULL);
    lv_style_set_bg_img_opa(&style_radio_chk, LV_OPA_0);

    // 添加选项并加入 group
    lv_obj_t* btns[5] = { 0 };
    char buf[5] = { 0 };                // 用于存储标签文本

    for (int i = 0; i < 5; i++) {
        /* 创建列表按钮 */
        btns[i] = lv_list_add_btn(list, NULL, NULL);
        lv_obj_set_size(btns[i], LV_PCT(100), 60); // 设置按钮大小
        lv_obj_set_style_pad_left(btns[i], 20, 0); // 设置容器左边距
        lv_obj_set_style_pad_right(btns[i], 10, 0); // 设置容器左边距
        lv_obj_add_style(btns[i], &style_btn, LV_PART_MAIN | LV_STATE_DEFAULT);

        /* 设置容器里的对齐方式 */
        lv_obj_set_flex_flow(btns[i], LV_FLEX_FLOW_ROW);   // 设置容器的布局为行
        lv_obj_set_flex_align(btns[i], LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    // 设置标签和复选框两边对齐，并且垂直居中

        /* 创建标签对象和复选框对象 */
        lv_obj_t* label = lv_label_create(btns[i]);
        lv_snprintf(buf, sizeof(buf), "%d", i + 1);
        lv_label_set_text(label, buf);

        lv_obj_t* checkbox = lv_checkbox_create(btns[i]);
        lv_obj_add_flag(checkbox, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_style(checkbox, &style_radio, LV_PART_INDICATOR);
        lv_obj_add_style(checkbox, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);

        /* 全局状态显示 */
        if( i == 0)
            lv_obj_add_state(checkbox, LV_PART_INDICATOR | LV_STATE_CHECKED | LV_STATE_DISABLED); /* 选中并禁用该复选框 */
        else
            lv_obj_add_state(checkbox, LV_PART_INDICATOR | LV_STATE_DISABLED); /* 选中并禁用该复选框 */
        
        lv_checkbox_set_text(checkbox, "");
        lv_obj_add_event_cb(btns[i], list_btn_event_cb, LV_EVENT_CLICKED, &max_connect_array[i]);
    }

     //让第一个按钮获得焦点
    if (list_group) {
        lv_group_focus_obj(lv_obj_get_child(btns[0], 0));
    }

    /* 点太快有概率死在这 */
    lv_coord_t mask_h = lv_obj_get_height(mask);
    lv_coord_t list_h = lv_obj_get_height(list);
    //lv_coord_t mask_h = 320;
    //lv_coord_t list_h = 320 * 0.7;
    lv_coord_t target_y = (mask_h - list_h) / 2;

    // 先把 list 放到屏幕下方
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, list_h);

    // 动画移动到居中
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, list);
    lv_anim_set_values(&a, list_h, - target_y);
    lv_anim_set_time(&a, 500);
    lv_anim_set_exec_cb(&a, anim_y_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);
}

static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt, bool txt_label)
{
    lv_obj_t* obj = lv_menu_cont_create(parent);

    lv_obj_t* img = NULL;
    lv_obj_t* label = NULL;

    lv_obj_set_size(obj, LV_PCT(100), 50); /* 设置菜单的大小为父容器的100% */

    // 不起作用……
    if (txt_label)
    {
        //lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // 对齐标签到容器中心
        //lv_obj_center(obj); // 对齐标签到容器中心
        lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    }

    if (icon) {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if (txt) {
        label = lv_label_create(obj);

        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    //if (builder_variant == 1 && icon && txt) {
    //    lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    //    lv_obj_swap(img, label);
    //}

    return obj;
}

static lv_obj_t* create_switch(lv_obj_t* parent, const char* icon, const char* txt, bool chk)
{
    lv_obj_t* obj = create_text(parent, icon, txt, false);

    lv_obj_t* sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

/**********************************************************************************************************************************/
/* 子项菜单页 */

/* 音频菜单设置单页 */
static void ui_audio_page(lv_obj_t* parent)
{
    lv_obj_t* section = lv_menu_section_create(parent);    /* 创建一个新的菜单节 */

    /* 音频状态设置容器 */
    lv_obj_t* audio_state_container = lv_menu_cont_create(section);
    lv_obj_set_size(audio_state_container, LV_PCT(100), 100); /* 设置菜单的大小为父容器的100% */
    lv_obj_set_flex_flow(audio_state_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(audio_state_container, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* 创建音频状态开关 */
    // 创建第一个按钮
    lv_obj_t* btn1 = lv_btn_create(audio_state_container);
    lv_obj_set_size(btn1, LV_PCT(35), LV_PCT(90));
    lv_obj_set_flex_flow(btn1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn1, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t* icon1 = lv_img_create(btn1);
    lv_img_set_src(icon1, LV_SYMBOL_VOLUME_MAX);
    lv_obj_t* label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Button 1");

    lv_group_add_obj(setting_group, btn1);

    // 创建第一个按钮
    lv_obj_t* btn2 = lv_btn_create(audio_state_container);
    lv_obj_set_size(btn2, LV_PCT(35), LV_PCT(90));
    lv_obj_set_flex_flow(btn2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn2, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t* icon2 = lv_img_create(btn2);
    lv_img_set_src(icon2, LV_SYMBOL_MUTE);
    lv_obj_t* label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Button 2");

    lv_group_add_obj(setting_group, btn2);

    // 设置按钮的初始样式
    // 默认状态样式
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0xEBEBEB), 0); // 灰色背景
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0xEBEBEB), 0); // 灰色背景

    // 选中状态样式
    lv_obj_set_style_bg_color(btn1, lv_color_white(), LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(btn2, lv_color_white(), LV_STATE_CHECKED);
    lv_obj_set_style_outline_width(btn1, 2, LV_STATE_CHECKED);
    lv_obj_set_style_outline_width(btn2, 2, LV_STATE_CHECKED);
    lv_obj_set_style_outline_color(btn1, lv_color_hex(0x8BC5F4), LV_STATE_CHECKED);
    lv_obj_set_style_outline_color(btn2, lv_color_hex(0x8BC5F4), LV_STATE_CHECKED);

    // 设置按钮1初始为选中状态
    lv_obj_add_state(btn1, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(lv_obj_get_child(btn1, 0), lv_color_hex(0x8BC5F4), 0);
    lv_obj_set_style_text_color(lv_obj_get_child(btn1, 1), lv_color_hex(0x8BC5F4), 0);
    lv_obj_set_style_text_color(lv_obj_get_child(btn2, 0), lv_color_hex(0xC4C4C4), 0);
    lv_obj_set_style_text_color(lv_obj_get_child(btn2, 1), lv_color_hex(0xC4C4C4), 0);

    // 添加事件回调，互相传递对方按钮的指针作为用户数据
    lv_obj_add_event_cb(btn1, audio_button_cb, LV_EVENT_ALL, btn2);
    lv_obj_add_event_cb(btn2, audio_button_cb, LV_EVENT_ALL, btn1);

    /* 音频播放值设置容器 */
    lv_obj_t* audio_value_container = lv_menu_cont_create(section);

    lv_obj_set_size(audio_value_container, LV_PCT(100), 400); /* 设置菜单的大小为父容器的100% */
    lv_obj_set_flex_flow(audio_value_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(audio_value_container, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* ring_label = lv_label_create(audio_value_container);
    lv_label_set_text(ring_label, "Button 1");

    lv_obj_t* ring_slider = lv_slider_create(audio_value_container);
    lv_slider_set_range(ring_slider, 0, 100);
    lv_slider_set_value(ring_slider, 50, LV_ANIM_ON);
    lv_obj_set_size(ring_slider, LV_PCT(90), 5);
    lv_obj_set_style_bg_color(ring_slider, lv_color_hex(0xB0B0B0), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ring_slider, lv_color_hex(0xFFFFFF), LV_PART_KNOB);
    lv_obj_set_style_border_color(ring_slider, lv_color_hex(0x2095F6), LV_PART_KNOB);
    lv_obj_set_style_border_width(ring_slider, 4, LV_PART_KNOB);

    lv_group_add_obj(setting_group, ring_slider);

    lv_obj_t* notify_label = lv_label_create(audio_value_container);
    lv_label_set_text(notify_label, "Button 1");

    lv_obj_t* notify_slider = lv_slider_create(audio_value_container);
    lv_slider_set_range(notify_slider, 0, 100);
    lv_slider_set_value(notify_slider, 50, LV_ANIM_ON);
    lv_obj_set_size(notify_slider, LV_PCT(90), 5);
    lv_obj_set_style_bg_color(notify_slider, lv_color_hex(0xB0B0B0), LV_PART_MAIN);
    lv_obj_set_style_bg_color(notify_slider, lv_color_hex(0xFFFFFF), LV_PART_KNOB);
    lv_obj_set_style_border_color(notify_slider, lv_color_hex(0x2095F6), LV_PART_KNOB);
    lv_obj_set_style_border_width(notify_slider, 4, LV_PART_KNOB);

    lv_group_add_obj(setting_group, notify_slider);
    //lv_group_set_focus_cb(setting_group, focus_cb);
}



/**********************************************************************************************************************************/
/* 根菜单页 */
lv_obj_t* ui_setting_page(lv_obj_t** focus_obj)
{
    /* 获取输入设备 */
    /* 使用鼠标滚轮作为输入设备，用于模拟按键输入设备 */
    touch_device = get_input_device(LV_INDEV_TYPE_POINTER);
    button_device = get_input_device(LV_INDEV_TYPE_ENCODER);

    ui_setting_container = lv_obj_create(NULL); /* 创建新页面的容器 */
    lv_obj_remove_style_all(ui_setting_container); /* 移除默认样式 */
    lv_obj_set_size(ui_setting_container, LV_PCT(100), LV_PCT(100)); /* 设置菜单的大小为父容器的100% */
    lv_obj_set_flex_flow(ui_setting_container, LV_FLEX_FLOW_COLUMN);

    ui_top_screen(ui_setting_container); /* 在新页容器里创建顶部菜单 */

    setting_group = lv_group_create(); /* 创建一个新的setting_group */
    lv_group_set_default(setting_group); /* 设置为默认的group */
    lv_indev_set_group(touch_device, setting_group); /* 将输入设置为当前group */
    lv_indev_set_group(button_device, setting_group); /* 将输入设置为当前group */

    /* 创建一个新的容器作为菜单的父容器 */
    lv_obj_t* ui_menu_container = lv_obj_create(ui_setting_container);
    lv_obj_remove_style_all(ui_menu_container); /* 移除默认样式 */
    lv_obj_align(ui_menu_container, LV_ALIGN_BOTTOM_MID, 0, 0);     /* 将容器对齐到父容器的顶部中间 */
    lv_obj_set_size(ui_menu_container, LV_PCT(100), LV_PCT(90));    /* 设置菜单容器的大小为父容器的100% */


    /*****************************************************************************************************************************/
    /* 创建菜单控件 */
    menu = lv_menu_create(ui_menu_container);
    lv_obj_set_size(menu, LV_PCT(100), LV_PCT(100)); /* 设置菜单容器的大小为父容器的100% */

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if (lv_color_brightness(bg_color) > 127) {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }

    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    lv_obj_add_event_cb(menu, root_back_bottom_cb, LV_EVENT_CLICKED, menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /*************************************************************************************************/
    /* Wi-Fi设置子页 */
    lv_obj_t * wifi_ap_page = lv_menu_page_create(menu, "Wi-Fi AP");
    lv_obj_set_style_pad_hor(wifi_ap_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_ap_page);

    section = lv_menu_section_create(wifi_ap_page); /* 创建一个新的菜单节 */
    create_switch(section, LV_SYMBOL_AUDIO, "Wi-Fi AP", false);

    /* 连接二维码项 */

    /* AP配置页 */

    /* 连接管理项 */
    create_text(wifi_ap_page, NULL, "Connection Management", true);
    section = lv_menu_section_create(wifi_ap_page); /* 创建一个新的菜单节 */
    cont = create_text(section, LV_SYMBOL_LIST, "Maximum number of connections", false);

    lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);   /* 增加点击属性 */
    lv_obj_add_event_cb(cont, max_connect_cb, LV_EVENT_CLICKED, NULL);  /* 创建点击事件回调 */

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    //cont = create_text(section, NULL, "Connected devices");
    //lv_menu_set_load_page_event(menu, cont, wifi_ap_connected_devices);

    //cont = create_text(section, NULL, "Blacklist");
    //lv_menu_set_load_page_event(menu, cont, wifi_ap_blacklist);


    lv_obj_t* wifi_sta_page = lv_menu_page_create(menu, "Wi-Fi STA");
    lv_obj_set_style_pad_hor(wifi_sta_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_sta_page);

    section = lv_menu_section_create(wifi_sta_page); /* 创建一个新的菜单节 */
    create_switch(section, LV_SYMBOL_AUDIO, "Wi-Fi STA", false);

    /************************************************************************************************************************/
    /* 创建设置子页 */

    /* Wi-Fi设置子页 */
    lv_obj_t * wifi_page = lv_menu_page_create(menu, "Wi-Fi Setting");

    lv_obj_set_style_pad_hor(wifi_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_page);

    section = lv_menu_section_create(wifi_page);    /* 创建一个新的菜单节 */
    cont = create_text(section, LV_SYMBOL_SETTINGS, "Wi-Fi AP", false);
    
    lv_menu_set_load_page_event(menu, cont, wifi_ap_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    cont = create_text(section, LV_SYMBOL_SETTINGS, "Wi-Fi STA", false);
    
    lv_menu_set_load_page_event(menu, cont, wifi_sta_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    /***************************************************************/
    /* 创建音频设置页 */
    lv_obj_t* audio_page = lv_menu_page_create(menu, "Audio Setting");

    lv_obj_set_style_pad_hor(audio_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(audio_page);

    ui_audio_page(audio_page); /* 在新页容器里创建音频设置页 */

    /***************************************************************/
    /* 关于页 */
    lv_obj_t* about_page = lv_menu_page_create(menu, "About");
    lv_obj_set_style_pad_hor(about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    

    section = lv_menu_section_create(about_page);    /* 创建一个新的菜单节 */
    create_text(section, NULL, "ESP32-GPX Version 1.0.0", true);

    /*********************************************************************************************************************************/

    /* 创建一个设置基础页 */
    setting_page = lv_menu_page_create(menu, "Settings");
    setting_page->user_data = 1;
    lv_obj_set_style_pad_hor(setting_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    // 设置菜单页面布局 - 垂直Flex布局
    lv_obj_set_flex_flow(setting_page, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(setting_page, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /***************************************************************/
    /* 无线连接 */
    create_text(setting_page, NULL, "Wireless Connect", true);

    section = lv_menu_section_create(setting_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "Wi-Fi", false);
    
    lv_menu_set_load_page_event(menu, cont, wifi_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */
    lv_group_focus_obj(cont);

    /****************************************************************/
    /* 状态 */
    create_text(setting_page, NULL, "State", true);

    section = lv_menu_section_create(setting_page);
    cont = create_text(section, LV_SYMBOL_BELL, "Audio", false);

    lv_menu_set_load_page_event(menu, cont, audio_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    cont = create_text(section, LV_SYMBOL_SETTINGS, "Display and Brightness", false);

    lv_menu_set_load_page_event(menu, cont, wifi_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    /****************************************************************/
    /* 其他 */
    create_text(setting_page, NULL, "Others", true);

    section = lv_menu_section_create(setting_page);
    cont = create_text(section, NULL, "About", false);
    
    lv_menu_set_load_page_event(menu, cont, about_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    /* 边栏型菜单页 */
    //lv_menu_set_sidebar_page(menu, setting_page);
    /* 非边栏型菜单页 */
    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_set_page(menu, setting_page);

    return ui_setting_container;
}

void ui_setting_deinit_page(void)
{
    if (ui_setting_container) lv_obj_del_delayed(ui_setting_container, 1000);

    ///* 静态全局对象指针 */
    //ui_setting_container = NULL;
}

/**
 * @brief 
 * @param e 
 */
static void root_back_bottom_cb(lv_event_t* e)
{
    lv_obj_t* page;
    page = lv_menu_get_cur_main_page(menu);
    ui_page_stack(ui_desktop_init_page, ui_desktop_deinit_page, NULL);
}
