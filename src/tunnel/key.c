#include "key.h"

void key_init(Key* key, fw64Node* node) {
    key->rotation = 0.0f;
    key->node = node;
}

void key_update(Key* key, float time_delta) {
    key->rotation += KEY_ROTATION_SPEED * time_delta;

    quat_from_euler(&key->node->transform.rotation, 15.0f, key->rotation, 0.0f);
    fw64_node_update(key->node);
}
