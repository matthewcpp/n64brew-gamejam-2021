#pragma once

#include "framework64/input.h"

typedef struct {
    fw64Input* input;
    Vec2 current_stick[4];
    Vec2 previous_stick[4];
    int connected_controllers[4];
} UiNavigation;

#ifdef __cplusplus
extern "C" {
#endif

void ui_navigation_init(UiNavigation* ui, fw64Input* input);
void ui_navigation_update(UiNavigation* ui);

int ui_navigation_moved_up(UiNavigation* ui);
int ui_navigation_moved_down(UiNavigation* ui);
int ui_navigation_moved_right(UiNavigation* ui);
int ui_navigation_moved_left(UiNavigation* ui);
int ui_navigation_accepted(UiNavigation* ui);
int ui_navigation_back(UiNavigation* ui);

#ifdef __cplusplus
}
#endif
