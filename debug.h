#pragma once
#include <windows.h>
#define debug_init(debug_buffer){ZeroMemory(&debug_buffer, sizeof(debug_buffer)); AllocConsole(); debug_buffer.console_handle = GetStdHandle(-11); debug_buffer.set_output = set_debug;} 
#define debug_log(debug){WriteConsole(debug.console_handle, debug.output_buffer, debug.output_length, debug.output_result, 0);}

typedef struct t_program_debug
{
	HANDLE console_handle;
	int output_result, output_length;
	wchar_t* output_buffer;

	int (*set_output)(struct t_program_debug*, wchar_t*);
}program_debug;

int set_debug(program_debug* debug, wchar_t* output);

program_debug debug;