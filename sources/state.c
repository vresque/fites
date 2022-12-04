#include <fites/state.h>

static struct state _state = {0};

struct state* state_w() {
    return &_state;
}

struct state state_r() {
    return _state;
}

