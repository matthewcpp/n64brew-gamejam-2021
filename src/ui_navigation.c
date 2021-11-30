#include "ui_navigation.h"

#include "framework64/n64/controller_button.h"

#define STICK_THRESHOLD 0.2f

void ui_navigation_init(UiNavigation* ui, fw64Input* input) {
    ui->input = input;

    fw64_input_stick(ui->input, 0, &ui->current_stick);
    ui->previous_stick = ui->current_stick;
}

void ui_navigation_update(UiNavigation* ui) {
    ui->previous_stick = ui->current_stick;
    fw64_input_stick(ui->input, 0, &ui->current_stick);
}

int ui_navigation_moved_up(UiNavigation* ui) {
    return ui->previous_stick.y < STICK_THRESHOLD && ui->current_stick.y > STICK_THRESHOLD ||
    fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_C_UP) || 
    fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_DPAD_UP);
}

int ui_navigation_moved_down(UiNavigation* ui) {
    return ui->previous_stick.y > -STICK_THRESHOLD && ui->current_stick.y < -STICK_THRESHOLD ||
    fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_C_DOWN) || 
    fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_DPAD_DOWN);
}

int ui_navigation_acceped(UiNavigation* ui) {
    return fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_A) || 
    fw64_input_button_pressed(ui->input, 0, FW64_N64_CONTROLLER_BUTTON_START);
}
