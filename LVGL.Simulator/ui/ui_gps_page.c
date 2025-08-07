#include "ui.h"

/**************************************************************************************************************/
/* 静态全局对象指针 */
static lv_obj_t* ui_gps_screen = NULL;
static lv_obj_t* ui_gps_container = NULL;
static lv_obj_t* ui_top_container = NULL;
static lv_obj_t* ui_bottom_container = NULL;

/* 应用按钮对象 */
static lv_obj_t* ui_gps_buttom = NULL;

/* 输入组管理 */


/**************************************************************************************************************/
/* 回调函数组 */
static void ui_root_back_buttom_cb(lv_event_t* e);



/**************************************************************************************************************/
/**
 * @brief gps页面函数
 *
 * 用于初始化桌面组件，如设置组件、GPS追踪组件
 *
 * @param[in] focus_obj 当前页聚焦对象，若无聚焦对象，输入NULL
 * @return ui_gps_screen gps页面静态指针，用于页面栈加载屏幕
 */
lv_obj_t* ui_gps_init_page(lv_obj_t** focus_obj)
{
    /* 创建gps屏幕页面 */
    ui_gps_screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(ui_gps_screen); /* 移除默认样式 */

    /* 在新页容器里创建顶部菜单 */
    ui_top_container = ui_top_screen(ui_gps_screen);

    /* 创建底部容器 */
    ui_bottom_container = lv_obj_create(ui_gps_screen);
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

    lv_obj_add_event_cb(ui_gps_buttom, ui_root_back_buttom_cb, LV_EVENT_CLICKED, NULL);

    /* 如果有焦点，则设置焦点对象 */
    if (focus_obj)
        lv_group_focus_obj(*focus_obj);

    return ui_gps_screen;
}

/**
 * @brief 删除页面对象，重置所有对象指针
 *
 * 重置所有静态指针
 *
 */
void ui_gps_deinit_page(void)
{
    if (ui_gps_screen) lv_obj_del_delayed(ui_gps_screen, 1000);

    ///* 静态全局对象指针 */
    //ui_gps_screen = NULL;
    //ui_gps_container = NULL;
    //ui_top_container = NULL;
    //ui_bottom_container = NULL;

    ///* 应用按钮对象 */
    //ui_gps_buttom = NULL;
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
static void ui_root_back_buttom_cb(lv_event_t* e)
{
    ui_page_stack(ui_desktop_init_page, ui_desktop_deinit_page, e->user_data);
}
