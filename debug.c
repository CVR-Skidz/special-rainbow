#include "debug.h"

int set_debug(program_debug* debug, wchar_t* output)
{
	if (lstrlenW(output))
	{
		debug->output_length = lstrlenW(output);
		debug->output_buffer = output;

		return 1;
	}

	return 0;
}