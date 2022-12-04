#include <fites/state.h>
#include <fites/fites.h>

struct state* state;

void state_init() {
    state = malloc(sizeof(struct state));
    if (!state) die("failed to allocate state");
}


struct state* state_w() {
    return state;
}

struct state state_r() {
    return *state;
}

