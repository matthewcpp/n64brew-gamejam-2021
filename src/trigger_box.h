#pragma once

#include "framework64/node.h"

typedef struct {
    fw64Node* trigger_node;
    fw64Collider* target_collider;
    Box bounding_box;
    uint16_t prev_state;
    uint16_t current_state;
} TriggerBox;

#ifdef __cplusplus
extern "C" {
#endif

void trigger_box_init(TriggerBox* trigger_box, fw64Node* node, fw64Collider* collider);
void trigger_box_update(TriggerBox* trigger_box);
int trigger_box_was_triggered(TriggerBox* trigger_box);

#ifdef __cplusplus
}
#endif