#include "ui_page_stack.h"

/**************************************************************************************************************/
/* 静态指针，记录当前页 */
static page_stack_typedef* page_stack = NULL;


/**************************************************************************************************************/
/**
 * @brief ui页面管理栈
 *
 * 管理ui页面栈，不允许重入。进入新页，记录当前页焦点，删除当前页。当前页退出后删除，并恢复前页焦点。
 *
 * @return NULL
 */
void ui_page_stack(lv_obj_t* (*next_page_init_point)(lv_obj_t** ), void (*next_page_deinit_point)(void), lv_obj_t** focus_obj)
{
    page_stack_typedef* del_page_stack = NULL;

    /* 根页 */
    if (page_stack) {
        /* 查询next_page是前进还是回退 */
        if ((page_stack->last_page_stack != NULL) ? (page_stack->last_page_stack->page_init_point == next_page_init_point) : 0) {
            /* 后退到前页 */
            /* 弹出当前页的栈 */
            del_page_stack = page_stack;
            page_stack = page_stack->last_page_stack;

            /* 下个页面初始化 */
            /* 传入前页焦点指针 */
            page_stack->page_obj = page_stack->page_init_point(page_stack->focus_obj);

            /* 加载初始化完成的页面 */
            /* 在屏幕动画期间，所有输入都将被禁用。 */
            if (page_stack->page_obj)
                /* 不使用这个函数删除 */
                lv_scr_load_anim(page_stack->page_obj, LV_SCR_LOAD_ANIM_OUT_RIGHT, 500, 100, false);

            /* 删除上个页面 */
            del_page_stack->page_deinit_point();

            /* 释放链表节 */
            lv_mem_free(del_page_stack);
        }
        else {
            /* 前进到下一页 */
            /* 压入新页的栈 */
            page_stack_typedef* next_page_stack = lv_mem_alloc(sizeof(page_stack_typedef));
            /* 记录焦点位置 */
            page_stack->focus_obj = focus_obj;

            /* 初始化当新栈数据，并压栈 */
            next_page_stack->page_init_point = next_page_init_point;
            next_page_stack->page_deinit_point = next_page_deinit_point;
            next_page_stack->focus_obj = NULL;
            next_page_stack->last_page_stack = page_stack;
            page_stack = next_page_stack;

            /* 下页面初始化 */
            page_stack->page_obj = page_stack->page_init_point(page_stack->focus_obj);

            /* 加载初始化完成的页面 */
            if (page_stack->page_obj)
                lv_scr_load_anim(page_stack->page_obj, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 100, false);

            /* 删除上个页面 */
            page_stack->last_page_stack->page_deinit_point();
        }
    }
    else {
        /* 前进到下一页 */
        /* 压入新页的栈 */
        page_stack = lv_mem_alloc(sizeof(page_stack_typedef));
        /* 初始化当前栈数据 */
        page_stack->page_init_point = next_page_init_point;
        page_stack->page_deinit_point = next_page_deinit_point;
        page_stack->focus_obj = NULL;
        page_stack->last_page_stack = NULL;

        /* 下页面初始化 */
        page_stack->page_obj = page_stack->page_init_point(page_stack->focus_obj);

        /* 加载初始化完成的页面 */
        if (page_stack->page_obj)
            lv_scr_load_anim(page_stack->page_obj, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 100, false);
    }
}
