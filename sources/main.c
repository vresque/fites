#include <fites/fites.h>
#include <fites/term.h>
#include <fites/input.h>

void main_loop() {
    get_window_size();
    while (true) {
        term_loop();
        input_loop();
    }
}


int main(int argc, char** argv) {
    state_init();

    if (argc >= 2) {
        editor_open(argv[1]);
    }

    enable_raw_mode();
    main_loop();
    return 0;
}

void die(const char* msg) {
    term_reset();
    
    perror(msg);
    exit(1);
}
