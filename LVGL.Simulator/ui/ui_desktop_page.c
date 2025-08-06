#include "ui.h"

/**************************************************************************************************************/
/* 静态全局对象指针 */
static lv_obj_t* ui_desktop_screen = NULL;
static lv_obj_t* ui_desktop_container = NULL;
static lv_obj_t* ui_top_container = NULL;
static lv_obj_t* ui_bottom_container = NULL;

/* 应用按钮对象 */
static lv_obj_t* ui_gps_buttom = NULL;
static lv_obj_t* ui_setting_buttom = NULL;
static lv_obj_t* ui_buttom_1 = NULL;
static lv_obj_t* ui_buttom_2 = NULL;
static lv_obj_t* ui_buttom_3 = NULL;
static lv_obj_t* ui_buttom_4 = NULL;
static lv_obj_t* ui_buttom_5 = NULL;
static lv_obj_t* ui_buttom_6 = NULL;
static lv_obj_t* ui_buttom_7 = NULL;
static lv_obj_t* ui_buttom_8 = NULL;
static lv_obj_t* ui_buttom_9 = NULL;
static lv_obj_t* ui_buttom_10 = NULL;
static lv_obj_t* ui_buttom_11 = NULL;
static lv_obj_t* ui_buttom_12 = NULL;
static lv_obj_t* ui_buttom_13 = NULL;
static lv_obj_t* ui_buttom_14 = NULL;
static lv_obj_t* ui_buttom_15 = NULL;
static lv_obj_t* ui_buttom_16 = NULL;
static lv_obj_t* ui_buttom_17 = NULL;
static lv_obj_t* ui_buttom_18 = NULL;
static lv_obj_t* ui_buttom_19 = NULL;

/* 输入组管理 */


/**************************************************************************************************************/
/* 回调函数组 */
static void ui_setting_buttom_cb(lv_event_t* e);



/**************************************************************************************************************/
/**
 * @brief desktop页面函数
 *
 * 用于初始化桌面组件，如设置组件、GPS追踪组件
 * 
 * @param[in] focus_obj 当前页聚焦对象，若无聚焦对象，输入NULL
 * @return ui_desktop_screen desktop页面静态指针，用于页面栈加载屏幕
 */
lv_obj_t* ui_desktop_page(lv_obj_t** focus_obj)
{
    /* 创建desktop屏幕页面 */
    ui_desktop_screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(ui_desktop_screen); /* 移除默认样式 */

    /* 在新页容器里创建顶部菜单 */
    ui_top_container = ui_top_screen(ui_desktop_screen);

    /* 创建底部容器 */
    ui_bottom_container = lv_obj_create(ui_desktop_screen);
    lv_obj_remove_style_all(ui_bottom_container); /* 移除默认样式 */
    lv_obj_align(ui_bottom_container, LV_ALIGN_BOTTOM_MID, 0, 0);     /* 将容器对齐到父容器的顶部中间 */
    lv_obj_set_size(ui_bottom_container, LV_PCT(100), LV_PCT(90));    /* 设置菜单容器的大小为父容器的100% */
    lv_obj_set_flex_flow(ui_bottom_container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_bottom_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_top(ui_bottom_container, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_bottom_container, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_bottom_container, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_bottom_container, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 创建按钮对象 */
    /* GPS按钮 */
    ui_gps_buttom = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_gps_buttom, 70, 70);
    lv_obj_add_flag(ui_gps_buttom, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_gps_buttom, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_gps_buttom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_gps_buttom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_gps_buttom_lebel = lv_label_create(ui_gps_buttom);
    lv_obj_set_align(ui_gps_buttom_lebel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_gps_buttom_lebel, 0, 40);
    lv_label_set_text(ui_gps_buttom_lebel, "GPS");
    lv_obj_set_style_text_color(ui_gps_buttom_lebel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_setting_buttom = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_setting_buttom, 70, 70);
    lv_obj_add_flag(ui_setting_buttom, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_setting_buttom, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_setting_buttom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_setting_buttom, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_t* ui_setting_buttom_lebel = lv_label_create(ui_setting_buttom);
    lv_obj_set_align(ui_setting_buttom_lebel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_setting_buttom_lebel, 0, 40);
    lv_label_set_text(ui_setting_buttom_lebel, "SET");
    lv_obj_set_style_text_color(ui_setting_buttom_lebel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_setting_buttom, ui_setting_buttom_cb, LV_EVENT_CLICKED, &ui_setting_buttom);

    /* 设置按钮 */
    ui_buttom_1 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_1, 70, 70);
    lv_obj_add_flag(ui_buttom_1, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_1, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_1 = lv_label_create(ui_buttom_1);
    lv_obj_set_align(ui_buttom_lebel_1, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_1, 0, 40);
    lv_label_set_text(ui_buttom_lebel_1, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_2 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_2, 70, 70);
    lv_obj_add_flag(ui_buttom_2, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_2, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_2 = lv_label_create(ui_buttom_2);
    lv_obj_set_align(ui_buttom_lebel_2, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_2, 0, 40);
    lv_label_set_text(ui_buttom_lebel_2, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_3 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_3, 70, 70);
    lv_obj_add_flag(ui_buttom_3, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_3, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_3 = lv_label_create(ui_buttom_3);
    lv_obj_set_align(ui_buttom_lebel_3, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_3, 0, 40);
    lv_label_set_text(ui_buttom_lebel_3, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_4 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_4, 70, 70);
    lv_obj_add_flag(ui_buttom_4, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_4, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_4 = lv_label_create(ui_buttom_4);
    lv_obj_set_align(ui_buttom_lebel_4, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_4, 0, 40);
    lv_label_set_text(ui_buttom_lebel_4, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_5 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_5, 70, 70);
    lv_obj_add_flag(ui_buttom_5, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_5, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_5 = lv_label_create(ui_buttom_5);
    lv_obj_set_align(ui_buttom_lebel_5, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_5, 0, 40);
    lv_label_set_text(ui_buttom_lebel_5, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_6 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_6, 70, 70);
    lv_obj_add_flag(ui_buttom_6, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_6, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_6 = lv_label_create(ui_buttom_6);
    lv_obj_set_align(ui_buttom_lebel_6, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_6, 0, 40);
    lv_label_set_text(ui_buttom_lebel_6, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_7 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_7, 70, 70);
    lv_obj_add_flag(ui_buttom_7, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_7, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_7 = lv_label_create(ui_buttom_7);
    lv_obj_set_align(ui_buttom_lebel_7, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_7, 0, 40);
    lv_label_set_text(ui_buttom_lebel_7, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_8 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_8, 70, 70);
    lv_obj_add_flag(ui_buttom_8, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_8, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_8 = lv_label_create(ui_buttom_8);
    lv_obj_set_align(ui_buttom_lebel_8, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_8, 0, 40);
    lv_label_set_text(ui_buttom_lebel_8, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置按钮 */
    ui_buttom_9 = lv_btn_create(ui_bottom_container);
    lv_obj_set_size(ui_buttom_9, 70, 70);
    lv_obj_add_flag(ui_buttom_9, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_style_bg_color(ui_buttom_9, lv_color_hex(0x8BCC77), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_buttom_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_buttom_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* ui_buttom_lebel_9 = lv_label_create(ui_buttom_9);
    lv_obj_set_align(ui_buttom_lebel_9, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_buttom_lebel_9, 0, 40);
    lv_label_set_text(ui_buttom_lebel_9, "SET");
    lv_obj_set_style_text_color(ui_buttom_lebel_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);


    /* 如果有焦点，则设置焦点对象 */
    if(focus_obj)
        lv_group_focus_obj(*focus_obj);

    return ui_desktop_screen;
}

/**************************************************************************************************************/
/**
 * @brief 设置按钮回调函数
 *
 * 调用设置菜单
 *
 * @param[in] e 事件句柄
 * @return 无返回值
 */
static void ui_setting_buttom_cb(lv_event_t* e)
{
    ui_page_stack(ui_setting_page, e->user_data);
}
