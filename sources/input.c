#include <fites/input.h>
#include <fites/term.h>


int input_read_key() {
    int read_amount;
    char key;

    while ((read_amount = read(STDIN_FILENO, &key, 1)) != 1) {
        if (read_amount == -1 && errno != EAGAIN) die("failed to read (input_read_key)");
    }

    if (key == '\x1b') {
        char sequence[3];
        if (read(STDIN_FILENO, &sequence[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &sequence[1], 1) != 1) {
            return ALT(sequence[0]);
        }


        if (sequence[0] == '[') { 
            if (sequence[1] >= '0' && sequence[1] <= '9') {
                if (read(STDIN_FILENO, &sequence[2], 1) != 1) return '\x1b';
                if (sequence[2] == '~') {
                    switch (sequence[1]) {
                        #ifdef USE_PAGE_UP_PAGE_DOWN
                        case '5': return KEY_GO_TO_START_OF_FILE;
                        case '6': return KEY_GO_TO_END_OF_FILE;
                        #endif
                        #ifdef USE_HOME_END
                        case '1':
                        case '7':
                            return KEY_GO_TO_START_OF_LINE;
                        case '4':
                        case '8':
                            return KEY_GO_TO_END_OF_LINE;
                        #endif
                        case '3': return KEY_DELETE;

                        default: break;
                    }
                }
            } else {
            // catch arrow keys
            switch(sequence[1]) {
                #ifdef USE_ARROW_KEYS
                case 'A': return KEY_MOVE_UP;
                case 'B': return KEY_MOVE_DOWN;
                case 'C': return KEY_MOVE_RIGHT;
                case 'D': return KEY_MOVE_LEFT;
                #endif
                #ifdef USE_HOME_END
                case 'H': return KEY_GO_TO_START_OF_LINE;
                case 'F': return KEY_GO_TO_END_OF_LINE;
                #endif
            }
            
            }
        } else if (sequence[0] == 'O') {
            #ifdef USE_HOME_END
            switch (sequence[1]) {
                case 'H': return KEY_GO_TO_START_OF_LINE;
                case 'F': return KEY_GO_TO_END_OF_LINE;
            }
            #endif
        }
    }

    return key;
}

void input_handle_exec_command(int key) {
    switch (key) {
        case KEY_QUIT:
            term_reset();
            exit(0);
            break;
    }
}

void check_if_cursor_x_not_too_big(int index) {
    struct text_row* current = (index >= state_r().text_row_count) ? NULL : &state_w()->text[index];
    if (!current) die("invalid row selected (check_if_cursor_x_not_too_big)");
    if (current->size < state_r().cursor_x) {
        state_w()->cursor_x = current->size;
    }
}

void input_handle_movement(int key) {
    struct text_row* current = (state_r().cursor_y >= state_r().text_row_count) ? NULL : &state_w()->text[state_r().cursor_y];

    switch (key) {
        case KEY_MOVE_LEFT:
            if (state_r().cursor_x != 0) {
                state_w()->cursor_x--;
            } else if (state_r().cursor_y > 0) {
                state_w()->cursor_y--;
                state_w()->cursor_x = state_r().text[state_r().cursor_y].size;
            }
            break;
        case KEY_MOVE_RIGHT:
            if (current && state_r().cursor_x < current->size) {
                state_w()->cursor_x++;
            } else if (state_r().cursor_y + 1 < state_r().rows) {
                state_w()->cursor_y++;
                state_w()->cursor_x = 0; 
            }
            break;
        case KEY_MOVE_UP:
            if (state_r().cursor_y != 0) {
                state_w()->cursor_y--;
            }
            check_if_cursor_x_not_too_big(state_r().cursor_y);
            break;
        case KEY_MOVE_DOWN:
            if (state_r().cursor_y < state_r().text_row_count) {
                state_w()->cursor_y++;
            }
            check_if_cursor_x_not_too_big(state_r().cursor_y);
            break;
        case KEY_GO_TO_END_OF_FILE:
        case KEY_GO_TO_START_OF_FILE: {
            int how_much = state_r().text_row_count;
            while (how_much--) {
                input_handle_movement(key == KEY_GO_TO_START_OF_FILE ? KEY_MOVE_UP : KEY_MOVE_DOWN);
            }
            state_w()->cursor_x = 0;
            break;
        }
        case KEY_GO_TO_START_OF_LINE:
            state_w()->cursor_x = 0;
            break;
        case KEY_GO_TO_END_OF_LINE:
            state_w()->cursor_x = current->size;
            break;
    }
}

void input_handle_no_mode_selected(int key) {
    switch (key) {
        case KEY_MOVE_DOWN:
        case KEY_MOVE_UP:
        case KEY_MOVE_LEFT:
        case KEY_MOVE_RIGHT:
        case KEY_GO_TO_END_OF_FILE:
        case KEY_GO_TO_START_OF_FILE:
        case KEY_GO_TO_START_OF_LINE:
        case KEY_GO_TO_END_OF_LINE:
            input_handle_movement(key);
            break;
    }
}

void input_process_keypress(int key) {
    switch (state_r().last_key) {
        case KEY_EXEC:
            input_handle_exec_command(key);
            break;
        default:
            input_handle_no_mode_selected(key);
            break;
    }
    state_w()->last_key = key;
    return;
}

void input_loop() {
    int in = input_read_key();

    input_process_keypress(in);
}
