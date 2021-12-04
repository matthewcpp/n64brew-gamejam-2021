#include "trigger_box.h"

void trigger_box_init(TriggerBox* trigger_box, fw64Node* node, fw64Collider* collider) {
    trigger_box->trigger_node = node;
    trigger_box->target_collider = collider;
    trigger_box->current_state = 0;
    trigger_box->prev_state = 0;

    if (node && collider) {
        Box source = {{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}};
        fw64_transform_xform_box(&node->transform, &source, &trigger_box->bounding_box);
    }
}

void trigger_box_update(TriggerBox* trigger_box) {
    if (!trigger_box->trigger_node || !trigger_box->target_collider)
        return;

    trigger_box->prev_state = trigger_box->current_state;

    int result = fw64_collider_test_box(trigger_box->target_collider, &trigger_box->bounding_box);
    trigger_box->current_state = result;
}

int trigger_box_was_triggered(TriggerBox* trigger_box) {
    return !trigger_box->prev_state && trigger_box->current_state;
}