#pragma once

#include "framework64/input.h"

typedef struct {
    fw64Input* input;
    Vec2 current_stick;
    Vec2 previous_stick;
} UiNavigation;

#ifdef __cplusplus
extern "C" {
#endif

void ui_navigation_init(UiNavigation* ui, fw64Input* input);
void ui_navigation_update(UiNavigation* ui);

int ui_navigation_moved_up(UiNavigation* ui);
int ui_navigation_moved_down(UiNavigation* ui);
int ui_navigation_acceped(UiNavigation* ui);

#ifdef __cplusplus
}
#endif
