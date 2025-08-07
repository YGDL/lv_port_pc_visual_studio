#ifndef __UI_DESKTOP_PAGE_H
#define __UI_DESKTOP_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

lv_obj_t* ui_desktop_init_page(lv_obj_t** focus_obj);
void ui_desktop_deinit_page(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
