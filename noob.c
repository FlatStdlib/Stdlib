/*

// Const variable
const int: n = 3;

// Decalre variable
int: age;

// Declare and initialize a variable
int: age = 19;

// Declare, allocate stack space, and initialize variable
int: position[2] = {10, 3};

// Declare, allocate stack space and Initialize variable
string: foo = "Hello World";

fnc foo(char n)
{

} string;

*/
#include <clibp.h>

#define EOC "END_OF_CODE"

#define EQ_IDENIFIER "="
#define SUBT_IDENIFIER "-"
#define ADD_IDENIFIER "+"
#define DIVIDE_IDENIFIER "/"

#define PRIVATE_IDENIFIER "static"
#define CONST_IDENIFIER "const"
#define HEAP_IDENIFIER "<HEAP>"
#define HEAP_ALLOC_START_IDENIFIER "<HEAP("
#define HEAP_ALLOC_END_IDENIFIER ")>"

#define NOOB_TYPES 7
#define NOOB_ALL_TYPES 9

typedef enum {
	_NO_TYPE 	= -1,
	_VOID		= 0,
	_INT 		= 1,
	_LONG 		= 2,
	_CHAR 		= 3,
	_FLOAT 		= 4,
	_DOUBLE 	= 5,
	_STRING 	= 6,
	_ENUM		= 7,
	_STRUCT 	= 8,
	_FUNCTION 	= 9,
	_HEAP		= 10
} _DATA_TYPE;

void *_DATA_TYPES[][2] = {
	{(void *)_VOID, 	"void"},
	{(void *)_INT, 		"int"},
	{(void *)_LONG, 	"long"},
	{(void *)_CHAR, 	"char"},
	{(void *)_FLOAT, 	"float"},
	{(void *)_DOUBLE, 	"double"},
	{(void *)_STRING, 	"string"},
	{(void *)_HEAP, 	"heap"},
	{(void *)_NO_TYPE, 	"no_type"},
	NULL
};

_DATA_TYPE string_to_type(string t)
{
	if(!t)
		return _NO_TYPE;

	for(int i = 0; i < NOOB_ALL_TYPES; i++)
	{
		if(mem_cmp(_DATA_TYPES[i][1], t, str_len(_DATA_TYPES[i][1])))
			return (_DATA_TYPE)_DATA_TYPES[i][0];
	}

	return _NO_TYPE;
}

string type_to_string(_DATA_TYPE t)
{
	for(int i = 0; i < NOOB_ALL_TYPES; i++)
	{
		if((_DATA_TYPE)_DATA_TYPES[i][0] == t)
			return _DATA_TYPES[i][1];
	}

	return NULL;
}

typedef struct {
	bool 			is_const;
	bool 			is_static;
	bool			is_init;
	_DATA_TYPE		type;
	string 			name;
	string			value;
} _variable;

typedef struct {
	bool			is_init;
	bool			is_static;
	string			name;
	array			values;
} _enum;

typedef struct {
	bool			is_init;
	bool			is_static;
	string			name;
	array			fields;
} _struct;

typedef struct {
	bool			is_init;
	string			name;
	array			parameters;
	string 			return_type;
} _function;

typedef struct {
	string			filename;
	string 			filepath;
	array			globals;
	array			enums;
	array 			structs;
	array 			variables;
	array			functions;
} file_t;

typedef struct {
	array			files;
} code_base;

string cGen[1024 * 10];

string get_file_content(string filename)
{
	if(!filename) return NULL;

	fd_t file = open_file(filename, 0, 0);
	if(!file) return NULL;

	i32 size = file_content_size(file);
	string content = allocate(0, size + 1);
	if(!content)
	{
		file_close(file);
		return NULL;
	}

	int bytes = file_read(file, content, size);
	file_close(file);
	if(bytes <= 0)
	{
		_pfree(content);
		return NULL;
	}

	return content;
}

bool string_startswith(string src, string needle)
{
	if(!src || !needle)
		return false;

	int len = str_len(src);
	int slen = str_len(needle);

	for(int i = 0; i < slen; i++)
	{
		if(src[i] != needle[i])
			return false;
	}

	return true;
}

fn validate_variable(string line, sArr args)
{
	if(!line)
		return;

	_variable *v = allocate(0, sizeof(_variable));
	int len = __get_size__(args);
	int line_len = __get_size__(line);

	if(args[0][0] == '<' && string_startswith(args[0], HEAP_ALLOC_START_IDENIFIER))
	{
		int start = find_char(args[0], '(') + 1, end = find_char(args[0], ')');
		if(start == -1 && end == -1 || args[0][end + 1] != '>')
			clibp_panic("Invalid HEAP allocation ending code...");

		string heap_size = allocate(0, end - start + 1);
		for(int i = start, idx = 0; i < end; i++, idx++)
			heap_size[idx] = args[0][i];

		char *raw_type = args[0] + (end + 2);
		if(raw_type[str_len(raw_type) - 1] == ':')
			raw_type[str_len(raw_type) - 1] = '\0';
		else
			clibp_panic("missing colon -> ':' after type!");

		v->type = string_to_type(raw_type);
		print("Var Type ID: "), _printi(v->type), print("\n");
		print_args((string []){"Var Type: ", type_to_string(v->type), "\n", 0});
		print("Var Heap Sz: "), print(heap_size), print("\n");

		v->name = str_dup(args[1]);
		if(len == 2 && v->name[__get_size__(v->name) - 1] == ';')
		{
			return; // EOC
		}

		/* More BS Here */
		if(args[2][0] != '=')
			clibp_panic("missing set -> '=' symbol");

		if(len < 4)
			clibp_panic("incomplete variable line");

		// TODO; Merge 3 >= end to string as value
		print_args((string []){"Var Value: ", args[3], "\n", 0});
		return;
	}

	int start = 0;
	if(mem_cmp(args[0], CONST_IDENIFIER, __get_size__(args[0])))
		v->is_const = true, start++;

	int n = __get_size__(args[start]) - 2;
	if(args[start][n] == ':')
		args[start][n] = '\0';
	else
		clibp_panic("missing colon -> ':' after type!");

	v->type = string_to_type(args[start++]);
	print("Var Type ID: "), _printi(v->type), print("\n");
	print_args((string []){"Var Type: ", type_to_string(v->type), "\n", 0});
	v->name = str_dup(args[start++]);
	if(args[start++][0] != '=')
		clibp_panic("missing set -> '=' symbol");

	if(len - start < 1)
		clibp_panic("incomplete variable line");

	// TODO; Merge 3 >= end to string as value
	print_args((string []){"Var Value: ", args[start], "\n", 0});

	if(line[line_len - 2] != ';')
		clibp_panic("missing semi-colon -> ';' at the end of line");
}

fn validate_function(char *line, char *code_block)
{
	print("Code Block: "), println(code_block);
}

i8 entry(int argc, char *argv[])
{
	if(argc < 2)
	{
		println("Usage: ./noob <file>");
		return 1;
	}

	println(argv[1]);
	string FILE_NAME = argv[1];
	string FILE_DATA = get_file_content(FILE_NAME);
	if(!FILE_DATA)
	{
		println("Error reading file");
		return 1;
	}

	int line_count = 0;
	sArr lines = split_lines(FILE_DATA, &line_count);

	bool ignore_comment = false;
	sArr args = NULL;
	int arg_c = 0;
	for(int i = 0; i < line_count; i++)
	{
		if(!lines[i])
			break;

		int len = __get_size__(lines[i]);
		if(len < 2)
			continue;

		if(count_char(lines[i], ' ') > -1)
			args = split_string(lines[i], ' ', &arg_c);

		if(lines[i][0] == '/' && lines[i][1] == '/')
			continue;

//		__sprintf(_OUTPUT_, "[ %d ]: %s", (void *)&i, lines[i]);
//		println(_OUTPUT_);

		/* Remove here on, Arguments are needed */
		if(arg_c < 2)
		{
			continue;
		}

		if(string_to_type(args[0]) != _NO_TYPE || args[0][0] == '<')
		{
			// variable
			print("Variable Found: "), println(args[1]);
			validate_variable(lines[i], args);
			pfree_array((array)args);
			continue;
		}

		if(mem_cmp(args[0], "struct", str_len(args[0])))
		{
			print("Struct Found: "), println(args[1]);
			pfree_array((array)args);
			continue;
		}

		if(mem_cmp(args[0], "fnc", str_len(args[0])))
		{
			if(i - 1 != 0)
			{
				if(lines[i - 1][0] == '[')
				{
					println("Attribute Found");
				}
			}
			print("Function Found: "), println(args[1]);

			char buff[4096] = {0};
			bool capture = find_char(lines[i], '{');
			if(capture == -1) capture = false;
			for(int c = i + 1; c < line_count; c++)
			{
				int len = str_len(lines[c]);
				if(capture == false && lines[c][0] == '{') {
					capture = true;
					continue;
				}

				if(capture && (lines[c][0] == '}' || lines[c][len] == '}' || lines[c][len - 1] == '}'))
				{
					validate_function(lines[i], buff);
					break;
				}

				if(capture) str_append(buff, lines[c]);
			}
			pfree_array((array)args);
			continue;
		}

		pfree_array((array)args);
	}

	return 0;
}
