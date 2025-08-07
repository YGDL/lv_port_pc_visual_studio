#ifndef __UI_SETTING_H
#define __UI_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t * ui_setting_menu;

lv_obj_t* ui_setting_page(lv_obj_t** focus_obj);
void ui_setting_deinit_page(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
