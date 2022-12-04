#include <fites/term.h>
#include <sys/ioctl.h>
#include <fites/input.h>


void get_cursor_position(int* rows, int* cols) {
    UNUSED(rows)
    UNUSED(cols)
    char buffer[32];
    unsigned int index = 0;
    
    
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) die("failed to write to stdout (get_cursor_position)");
    while (index < sizeof(buffer) - 1) {
        if (read(STDIN_FILENO, &buffer[index], 1) != 1) break;
        if (buffer[index] == 'R') break;
        index++;
    }
    buffer[index] = '\0';

    if (buffer[0] != '\x1b' || buffer[1] != '[') die("bad buffer format (get_cursor_position)");
   // Clang is annoying here
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    if (sscanf(&buffer[2], "%d;%d", rows, cols) != 2) die("failed to read position from buffer (get_cursor_position)");
#pragma GCC diagnostic pop
    return;
}

void get_window_size() {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)         die("failed to get window size (get_window_size)");
        get_cursor_position(&state_w()->rows, &state_w()->cols);
    } else {
        state_w()->cols = ws.ws_col;
        state_w()->rows = ws.ws_row;
    }
}

void enable_raw_mode() {
    struct termios raw;

    atexit(disable_raw_mode);
    if (tcgetattr(STDIN_FILENO, &state_w()->terminal) == -1) die("failed to tcgetattr (enable_raw_mode)");
    raw = state_w()->terminal;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("failed to tcsetattr (enable_raw_mode)");
}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &state_w()->terminal) == -1) die("failed to tcsetattr (disable_raw_mode)");
}

/* editor draw loop*/

void term_reset() {
    // clear screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // reposition cursor
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_empty_row(int row, struct buffer* buf) {
        if (state_r().text_row_count == 0 && row == state_r().rows / 3) {
        char welcome[80];

        int length = snprintf(welcome, sizeof(welcome), "Fites v%s - The fast, interactive, and trusty editing software", VERSION);
        if (length > state_r().cols) length = state_w()->cols;

        int padding = (state_r().cols - length) / 2;
        if (padding) {
            buffer_append(buf, "~", 1);
            padding--;
        }

        while (padding--) buffer_append(buf, " ", 1);

        buffer_append(buf, welcome, length);
    } else {
        buffer_append(buf, "~", 1);
    }
}

void draw_filled_row(int row, struct buffer* buf) {
    int len = state_r().text[row].size - state_r().col_offset;
    if (len < 0) len = 0;
    if (len >= state_r().cols) len = state_r().cols;
    buffer_append(buf, &state_r().text[row].buffer[state_r().col_offset], len);
}


void draw_row(int row, struct buffer* buf) {
    int real_row = row + state_r().row_offset;
    if (real_row >= state_w()->text_row_count) {
        draw_empty_row(row, buf);
    } else {
        draw_filled_row(real_row, buf);
    }
}

void term_draw_rows(struct buffer* buf) {
    int row;
    for (row = 0; row < state_r().rows; row++) {
        draw_row(row, buf);        
        // clear line
        buffer_append(buf, "\x1b[K", 3);
        if (row < state_r().rows - 1) {
            buffer_append(buf, "\r\n", 2);
        }
    }
}

void term_scroll() {
    if (state_r().cursor_y < state_r().row_offset) {
        state_w()->row_offset = state_r().cursor_y;
    }
    if (state_r().cursor_y >= state_r().row_offset + state_r().rows) {
        state_w()->row_offset = state_r().cursor_y - state_r().rows + 1;
    }
    if (state_r().cursor_x < state_r().col_offset) {
        state_w()->col_offset = state_r().cursor_x;
    }
    if (state_r().cursor_x >= state_r().col_offset + state_r().cols) {
        state_w()->col_offset = state_r().cursor_x - state_r().cols + 1;
    }
}

void draw_cursor(struct buffer* buffer) {
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (state_r().cursor_y - state_r().row_offset) + 1, (state_r().cursor_x - state_r().col_offset) + 1);
    buffer_append(buffer, buf, strlen(buf));
}

void term_loop() {
    term_scroll();



    struct buffer buffer = BUFFER_INIT;
    // hide cursor
    buffer_append(&buffer, "\x1b[?25l", 6);
    buffer_append(&buffer, "\x1b[H", 3);

    term_draw_rows(&buffer);

    // show cursor
    draw_cursor(&buffer);
    buffer_append(&buffer, "\x1b[?25h", 6);

    write(STDOUT_FILENO, buffer.buf, buffer.count);

    buffer_drop(&buffer);
}
