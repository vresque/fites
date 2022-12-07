#include <fites/state.h>
#include <fites/fites.h>

struct state* state;

void state_init() {
    state = malloc(sizeof(struct state));
    if (!state) die("failed to allocate state");

    state->cursor_x = 0;
    state->cursor_y = 0;
    state->text_row_count = 0;
    state->text = NULL;
    state->row_offset = 0;
    state->col_offset = 0;
    state->filename = NULL;
    state->rendered_x = 0;
    state->status[0] = '\0';
    state->status_time = 0;
    state->buffer_is_dirty = 0;
    state->current_syntax = NULL;
}


struct state* state_w() {
    return state;
}

struct state state_r() {
    return *state;
}

