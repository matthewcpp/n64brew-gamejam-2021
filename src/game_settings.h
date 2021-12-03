#pragma once

//time (in seconds) each player has control in multiplayer, fixed timer mode

#define CONTROL_TRANSFER_TIME_FAST 0.1f
#define CONTROL_TRANSFER_TIME_MED  1.0f
#define CONTROL_TRANSFER_TIME_SLOW 5.0f

typedef enum ControlMode {
	CONTROL_MODE_SINGLE,
	CONTROL_MODE_MULTI_TIMER,
	CONTROL_MODE_MULTI_ONPRESS
} ControlMode;

typedef struct GameSettings {
	ControlMode control_mode;
	float control_transfer_time;
} GameSettings;