#include <fites/finder.h>
#include <fites/fites.h>


void find_callback(char* query, int key) {
	static int last = -1;
	static int dir = 1;
	
	if (key == '\r' || key == '\x1b') {
		last = -1;
		dir = 1;
		return;
	} else if (key == KEY_MOVE_RIGHT || key == KEY_MOVE_DOWN) {
		dir = 1;
	} else if (key == KEY_MOVE_LEFT || key == KEY_MOVE_UP) {
		dir = -1;
	} else {
		last = -1;
		dir = 1;
	}
	
	if (last == -1) dir = 1;
	int current = last;

	int i = 0;
	for (i = 0; i < state_r().text_row_count; i++) {
		current += dir;
		if (current == -1) current = state_r().text_row_count - 1;
		else if (current == state_r().text_row_count) current = 0;
		
		struct text_row* row = &state_w()->text[current];
		char* match = strstr(row->rendered, query);
		if (match) {
			last = current;
			state_w()->cursor_y = current;
			state_w()->cursor_x = text_rendered_x_to_cursor_x(row, match - row->rendered);
			state_w()->row_offset = state_r().text_row_count;
			break;
		}
	}

}


void find() {
	int saved_cx = state_r().cursor_x;
	int saved_cy = state_r().cursor_y;
	int saved_col_off = state_r().col_offset;
	int saved_row_off = state_r().row_offset;
	char* query = editor_prompt_with_callback("Find: %s (ESC/Movement/Enter)", find_callback);

	if (query) { 
		free(query);
	} else {
		state_w()->cursor_x = saved_cx;
		state_w()->cursor_y = saved_cy;
		state_w()->col_offset = saved_col_off;
		state_w()->row_offset = saved_row_off;
	};
}