#include "ui.h"

lv_obj_t* ui_top_container; /* 顶部容器，用于放置顶部状态栏 */
lv_obj_t* ui_top_left;      /* 顶部左侧容器，用于对齐 */
lv_obj_t* ui_top_right;     /* 顶部右侧容器，用于对齐 */

lv_obj_t* ui_top_screen(lv_obj_t * parent)
{
    /* 创建一个新的容器作为状态栏的父容器 */
    ui_top_container = lv_obj_create(parent);
    lv_obj_remove_style_all(ui_top_container); /* 移除默认样式 */
    lv_obj_align(ui_top_container, LV_ALIGN_TOP_MID, 0, 0); /* 将容器对齐到父容器的顶部中间 */
    
    // 设置状态栏的大小和位置
    lv_obj_set_size(ui_top_container, LV_PCT(100), LV_PCT(10));

    lv_obj_set_style_bg_opa(ui_top_container, LV_OPA_0, 0); /* 设置背景透明 */

    lv_obj_set_flex_flow(ui_top_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_top_container, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /*******************************************************************************************************/
    /* 再创建两个透明容器用于对齐 */

    /* 左侧容器配置 */
    ui_top_left = lv_obj_create(ui_top_container);
    lv_obj_remove_style_all(ui_top_left); /* 移除默认样式 */
    lv_obj_set_size(ui_top_left, LV_PCT(50), LV_PCT(100));
    lv_obj_set_style_bg_opa(ui_top_left, LV_OPA_0, 0); /* 设置背景透明 */
    lv_obj_set_style_pad_gap(ui_top_left, 10, 0); // 设置子元素之间的间隔为10像素
    lv_obj_set_style_pad_left(ui_top_left, 10, 0); // 设置左边距为10像素

    lv_obj_set_flex_flow(ui_top_left, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_top_left, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* 右侧容器配置 */
    ui_top_right = lv_obj_create(ui_top_container);
    lv_obj_remove_style_all(ui_top_right); /* 移除默认样式 */
    lv_obj_set_size(ui_top_right, LV_PCT(50), LV_PCT(100));
    lv_obj_set_style_bg_opa(ui_top_right, LV_OPA_0, 0); /* 设置背景透明 */
    lv_obj_set_style_pad_gap(ui_top_right, 10, 0); // 设置子元素之间的间隔为10像素
    lv_obj_set_style_pad_right(ui_top_right, 10, 0); // 设置左边距为10像素

    lv_obj_set_flex_flow(ui_top_right, LV_FLEX_FLOW_ROW_REVERSE);
    lv_obj_set_flex_align(ui_top_right, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /*********************************************************************************************************/
    /* 在左侧容器中添加图标 */
    /* 时间显示 */
    lv_obj_t* icon_time = lv_label_create(ui_top_left);
    lv_label_set_text(icon_time, "12:00"); /* 设置时间图标和文本 */

    lv_obj_t* icon_txt = lv_label_create(ui_top_left);
    lv_label_set_text(icon_txt, "100%"); /* 设置电量图标和文本 */

    /* 在左侧容器中添加图标 */
    /* 电量显示 */
    lv_obj_t* icon_battery_txt = lv_label_create(ui_top_right);
    lv_label_set_text(icon_battery_txt, "100%"); /* 设置电量图标和文本 */
    
    lv_obj_t* icon_battery = lv_img_create(ui_top_right);
    lv_img_set_src(icon_battery, LV_SYMBOL_BATTERY_FULL);/* 设置电量图标和文本 */

    /* Wi-Fi显示 */
    lv_obj_t* icon_wifi = lv_img_create(ui_top_right);
    lv_img_set_src(icon_wifi, LV_SYMBOL_WIFI); /* 设置Wi-Fi图标和文本 */

    /* 返回容器句柄 */
    return ui_top_container;
}
