#include "ui.h"

lv_obj_t * ui_menu;
lv_obj_t * setting_page;
/* 设置页面group */
static lv_group_t * setting_group;
static lv_indev_t * input_device;

// 全局指针，便于销毁遮罩和list
static lv_obj_t * mask = NULL;
static lv_obj_t * list = NULL;
static lv_group_t * g = NULL;

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

// list选项点击事件：关闭list和遮罩
static void list_btn_event_cb(lv_event_t* e)
{
    if (mask) {
        // 解冻 group，恢复正常焦点流转
        //if (g)
        //    lv_group_focus_freeze(g, false);
        lv_obj_del(mask);
        mask = NULL;
        list = NULL;
    }
}

static void max_connect_cb(lv_event_t * e)
{
    // 防止重复创建
    if (mask)
        return;

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
    lv_obj_add_event_cb(mask, list_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 创建list
    list = lv_list_create(mask);
    lv_obj_set_size(list, LV_PCT(70), LV_PCT(70));
    lv_obj_center(list);

    // 获取 group
    g = lv_group_get_default();

    // 添加选项并加入 group
    lv_obj_t* btns[9];
    btns[0] = lv_list_add_btn(list, LV_SYMBOL_OK, "A");
    btns[1] = lv_list_add_btn(list, LV_SYMBOL_OK, "B");
    btns[2] = lv_list_add_btn(list, LV_SYMBOL_OK, "C");
    btns[3] = lv_list_add_btn(list, LV_SYMBOL_OK, "D");
    btns[4] = lv_list_add_btn(list, LV_SYMBOL_OK, "E");
    btns[5] = lv_list_add_btn(list, LV_SYMBOL_OK, "F");
    btns[6] = lv_list_add_btn(list, LV_SYMBOL_OK, "G");
    btns[7] = lv_list_add_btn(list, LV_SYMBOL_OK, "H");
    btns[8] = lv_list_add_btn(list, LV_SYMBOL_OK, "I");

    for (int i = 0; i < 9; i++) {
        lv_obj_add_event_cb(btns[i], list_btn_event_cb, LV_EVENT_CLICKED, list);
        //if (g) lv_group_add_obj(g, btns[i]);
    }

    // 让第一个按钮获得焦点
    if (g) {
        lv_group_focus_obj(btns[0]);
        //lv_group_focus_freeze(g, true); // 冻结焦点，防止切换到其它对象
    }
}

static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt)
{
    lv_obj_t* obj = lv_menu_cont_create(parent);

    lv_obj_t* img = NULL;
    lv_obj_t* label = NULL;

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
    lv_obj_t* obj = create_text(parent, icon, txt);

    lv_obj_t* sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

void ui_setting_screen(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    /* 获取输入设备 */
    /* 使用鼠标滚轮作为输入设备，用于模拟按键输入设备 */
    input_device = get_input_device(LV_INDEV_TYPE_ENCODER);
    
    /* 创建一个新的容器作为菜单的父容器 */
    lv_obj_t * menu = lv_menu_create(parent);

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if (lv_color_brightness(bg_color) > 127) {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }

    lv_obj_set_size(menu, LV_PCT(100), LV_PCT(90)); /* 设置菜单的大小为父容器的100% */
    lv_obj_center(menu);

    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    //lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);

    setting_group = lv_group_create(); /* 创建一个新的setting_group */
    lv_group_set_default(setting_group); /* 设置为默认的group */
    lv_indev_set_group(input_device, setting_group); /* 将输入设置为当前group */
    

    lv_obj_t * cont;
    lv_obj_t * section;

    /* Wi-Fi设置子页 */
    lv_obj_t * wifi_ap_page = lv_menu_page_create(menu, "Wi-Fi AP");
    lv_obj_set_style_pad_hor(wifi_ap_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_ap_page);

    section = lv_menu_section_create(wifi_ap_page); /* 创建一个新的菜单节 */
    create_switch(section, LV_SYMBOL_AUDIO, "Wi-Fi AP", false);

    /* 连接二维码项 */

    /* AP配置页 */

    /* 连接管理项 */
    create_text(wifi_ap_page, NULL, "Connection Management");
    section = lv_menu_section_create(wifi_ap_page); /* 创建一个新的菜单节 */
    cont = create_text(section, LV_SYMBOL_LIST, "Maximum number of connections");
    lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);   /* 增加点击属性 */
    lv_obj_add_event_cb(cont, max_connect_cb, LV_EVENT_CLICKED, NULL);  /* 创建点击事件回调 */

    //cont = create_text(section, NULL, "Connected devices");
    //lv_menu_set_load_page_event(menu, cont, wifi_ap_connected_devices);

    //cont = create_text(section, NULL, "Blacklist");
    //lv_menu_set_load_page_event(menu, cont, wifi_ap_blacklist);


    lv_obj_t* wifi_sta_page = lv_menu_page_create(menu, "Wi-Fi STA");
    lv_obj_set_style_pad_hor(wifi_sta_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_sta_page);

    section = lv_menu_section_create(wifi_sta_page); /* 创建一个新的菜单节 */
    create_switch(section, LV_SYMBOL_AUDIO, "Wi-Fi STA", false);


    /* 创建设置子页 */
    /* Wi-Fi设置子页 */
    lv_obj_t * wifi_page = lv_menu_page_create(menu, "Wi-Fi Setting");
    lv_obj_set_style_pad_hor(wifi_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(wifi_page);

    section = lv_menu_section_create(wifi_page);    /* 创建一个新的菜单节 */
    cont = create_text(section, LV_SYMBOL_SETTINGS, "Wi-Fi AP");
    lv_menu_set_load_page_event(menu, cont, wifi_ap_page);

    cont = create_text(section, LV_SYMBOL_SETTINGS, "Wi-Fi STA");
    lv_menu_set_load_page_event(menu, cont, wifi_sta_page);

    /* 关于页 */
    lv_obj_t* about_page = lv_menu_page_create(menu, "About");
    lv_obj_set_style_pad_hor(about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);

    section = lv_menu_section_create(about_page);    /* 创建一个新的菜单节 */
    create_text(section, NULL, "ESP32-GPX Version 1.0.0");

    /* 创建一个设置基础页 */
    setting_page = lv_menu_page_create(menu, "Settings");
    lv_obj_set_style_pad_hor(setting_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);

    section = lv_menu_section_create(setting_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, "Wi-Fi");
    lv_menu_set_load_page_event(menu, cont, wifi_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */
    lv_group_focus_obj(cont);

    create_text(setting_page, NULL, "Others");

    section = lv_menu_section_create(setting_page);
    cont = create_text(section, NULL, "About");
    lv_menu_set_load_page_event(menu, cont, about_page);

    lv_group_add_obj(setting_group, cont); /* 将菜单添加到group中 */

    /* 边栏型菜单页 */
    //lv_menu_set_sidebar_page(menu, setting_page);
    /* 非边栏型菜单页 */
    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_set_page(menu, setting_page);

}
