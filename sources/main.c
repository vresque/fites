#include <fites/fites.h>
#include <fites/term.h>
#include <fites/input.h>

void main_loop() {
    state_w()->cursor_x = 0;
    state_w()->cursor_y = 0;
    get_window_size();
    while (true) {
        term_loop();
        input_loop();
    }
}


int main() {
    enable_raw_mode();

    main_loop();
    return 0;
}

void die(const char* msg) {
    term_reset();
    
    perror(msg);
    exit(1);
}
