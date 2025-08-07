#ifndef __UI_GPS_PAGE_H
#define __UI_GPS_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

lv_obj_t* ui_gps_init_page(lv_obj_t** focus_obj);
void ui_gps_deinit_page(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
