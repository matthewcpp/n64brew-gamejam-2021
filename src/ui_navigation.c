#include "ui_navigation.h"

#include "framework64/n64/controller_button.h"

#define STICK_THRESHOLD 0.2f

void ui_navigation_init(UiNavigation* ui, fw64Input* input) {
    ui->input = input;
    for(int i = 0; i < 4; i++) {
        ui->connected_controllers[i] = fw64_input_controller_is_connected(ui->input, i);
        if(ui->connected_controllers[i]) {
            fw64_input_stick(ui->input, i, &ui->current_stick[i]);
        }
        else {
            ui->current_stick[i].x = 0;
            ui->current_stick[i].y = 0;
        }
        ui->previous_stick[i] = ui->current_stick[i];
    }
}

void ui_navigation_update(UiNavigation* ui) {
    for(int i = 0; i < 4; i++) {
        ui->previous_stick[i] = ui->current_stick[i];
        ui->connected_controllers[i] = fw64_input_controller_is_connected(ui->input, i);
        if(ui->connected_controllers[i]) {
            fw64_input_stick(ui->input, i, &ui->current_stick[i]);            
        }
        else {
            ui->current_stick[i].x = 0;
            ui->current_stick[i].y = 0;
        }
    }
}

int ui_navigation_moved_up(UiNavigation* ui) {

    int moved_up = 0;

    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            moved_up |= (ui->previous_stick[i].y < STICK_THRESHOLD && ui->current_stick[i].y > STICK_THRESHOLD ||
                        fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_C_UP) || 
                        fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_DPAD_UP));        
        }
    }
    return moved_up;
}

int ui_navigation_moved_down(UiNavigation* ui) {

    int moved_down = 0;

    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            moved_down |=   (ui->previous_stick[i].y > -STICK_THRESHOLD && ui->current_stick[i].y < -STICK_THRESHOLD ||
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_C_DOWN) || 
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_DPAD_DOWN));        
        }
    }
    return moved_down;
}

int ui_navigation_accepted(UiNavigation* ui) {

    int accepted = 0;

    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            accepted |=     (fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_A) || 
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_START));        
        }
    }
    return accepted;
}
