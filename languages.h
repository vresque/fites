#ifndef LANGUAGES_H
#define LANGUAGES_H

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_UNDERSCORED_NUMBERS (1 << 1)
#define HL_HIGHLIGHT_STRINGS (1 << 2)
#define HL_HAS_SPANNED_COMMENTS (1 << 3)
#define HL_HAS_MULTILINE_COMMENT (1 << 4)

char* c_extensions[] = { ".c", ".h", ".cpp", NULL };
char* c_keywords[] = { 	"switch", "if", "while", "for", "break",
						"continue", "return", "else", "struct", "union",
						"typedef", "static", "enum", "class", "case", "true", "false", "signed", NULL };
char* c_types[] = { "int", "long", "double", "float", "char", "unsigned", "signed", "void", NULL };

char* rust_extensions[] = { ".rs", ".rust", NULL };
char* rust_keywords[] = { "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return", "self", "static", "struct", "super", "trait", "true", "type", "unsafe", "use", "where", "while", "async", "await", "dyn", NULL };
char* rust_types[] = { "Self", "Vec", "String", "str", "i8", "i16", "i32", "i64", "i128", "isize", "u8", "u16", "u32", "u64", "u128", "usize", "f32", "f64", "bool", "()", NULL};

char* python_extensions[] =  { ".py", ".py3", ".python", NULL };
char* python_keywords[] = { "False", "await", "else", "import", "pass", "None", "break", "except", "in", "raise", "True", "break", "except", "in", "raise", "True", "class", "finally", "is", "return", "and", "continue", "for", "lambda", "try", "as", "def", "from", "nonlocal", "while", "assert", "del", "global", "not", "with", "async", "elif", "if", "or", "yield", NULL };
char* python_types[] = { NULL };

char* multiline_comment_basic[] = { "/*", "*/" };

// Add your own language here
struct syntax languages[] = {
	{
		.filetype = "c",
		.filematch = c_extensions,
		.line_comment = "//",
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HAS_SPANNED_COMMENTS | HL_HAS_MULTILINE_COMMENT ,
		.types = c_types,
		.keywords = c_keywords,
		.multiline_comment = multiline_comment_basic,
	},
	{
		.filetype = "rust",
		.filematch = rust_extensions,
		.line_comment = "//",
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HIGHLIGHT_UNDERSCORED_NUMBERS | HL_HAS_SPANNED_COMMENTS | HL_HAS_MULTILINE_COMMENT,
		.keywords = rust_keywords,
		.types = rust_types,
		.multiline_comment = multiline_comment_basic,

	},
	{
		.filetype = "python",
		.line_comment = "#",
		.filematch = python_extensions,
		.flags = HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
		.keywords = python_keywords,
		.types = python_types,
		.multiline_comment = NULL,
	},
};

#endif