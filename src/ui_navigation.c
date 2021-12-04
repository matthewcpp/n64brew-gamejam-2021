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

int ui_navigation_moved_right(UiNavigation* ui) {
    int moved_right = 0;
    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            moved_right |=   (ui->previous_stick[i].x < STICK_THRESHOLD && ui->current_stick[i].x > STICK_THRESHOLD ||
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_C_RIGHT) || 
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_DPAD_RIGHT) ||
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_R));        
        }
    }
    return moved_right;
}
int ui_navigation_moved_left(UiNavigation* ui) {
    int moved_left = 0;
    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            moved_left |=   (ui->previous_stick[i].x > -STICK_THRESHOLD && ui->current_stick[i].x < -STICK_THRESHOLD ||
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_C_LEFT) || 
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_DPAD_LEFT) ||
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_L));        
        }
    }
    return moved_left;
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

int ui_navigation_back(UiNavigation* ui) {
    int back = 0;
    for(int i = 0; i < 4; i++) {
        if(ui->connected_controllers[i]) {
            back |=     (fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_B) || 
                            fw64_input_button_pressed(ui->input, i, FW64_N64_CONTROLLER_BUTTON_Z));        
        }
    }    
    return back;
}
