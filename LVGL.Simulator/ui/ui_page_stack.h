#ifndef __UI_PAGE_STACK_H
#define __UI_PAGE_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

/* 页面栈 */
typedef struct page_stack_t {
    uint8_t page_state : 1;

    /* 页面初始化函数指针 */
    lv_obj_t* (*page_point)(lv_obj_t* );

    /* 页面指针 */
    lv_obj_t* page_obj;

    /* 聚焦对象指针 */
    lv_obj_t** focus_obj;

    /* 上一节链表 */
    struct page_stack_t* last_page_stack;
}page_stack_typedef;

void ui_page_stack(lv_obj_t* (*next_page_point)(lv_obj_t*), lv_obj_t** focus_obj);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
