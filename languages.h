#ifndef LANGUAGES_H
#define LANGUAGES_H

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_UNDERSCORED_NUMBERS (1 << 1)
#define HL_HIGHLIGHT_STRINGS (1 << 2)
#define HL_HAS_SPANNED_COMMENTS (1 << 3)


char* c_extensions[] = { ".c", ".h", ".cpp", NULL };
char* rust_extensions[] = { ".rs", ".rust", NULL };
char* python_extensions[] =  { ".py", ".py3", ".python", NULL };
// Add your own language here
struct syntax languages[] = {
	{
		.filetype = "c",
		.filematch = c_extensions,
		.line_comment = "//",
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HAS_SPANNED_COMMENTS
	},
	{
		.filetype = "rust",
		.filematch = rust_extensions,
		.line_comment = "//",
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HIGHLIGHT_UNDERSCORED_NUMBERS | HL_HAS_SPANNED_COMMENTS
	},
	{
		.filetype = "python",
		.line_comment = "#",
		.filematch = python_extensions,
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
	},
};

#endif