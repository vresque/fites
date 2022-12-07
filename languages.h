#ifndef LANGUAGES_H
#define LANGUAGES_H

char* c_extensions[] = { ".c", ".h", ".cpp", NULL };
char* rust_extensions[] = { ".rs", ".rust", NULL };
char* python_extensions[] =  { ".py", ".py3", ".python", NULL };
// Add your own language here
struct syntax languages[] = {
	{
		.filetype = "c",
		.filematch = c_extensions,
		.flags = HL_HIGHLIGHT_NUMBERS
	},
	{
		.filetype = "rust",
		.filematch = rust_extensions,
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_UNDERSCORED_NUMBERS
	},
	{
		.filetype = "python",
		.filematch = python_extensions,
		.flags = HL_HIGHLIGHT_NUMBERS
	},
};

#endif