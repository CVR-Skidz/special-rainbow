#include <stdio.h>
#include "bm_reader.h"

typedef struct t_rl_packet
{
	unsigned char run_count, run;
}rl_packet;

//packet funcitons
rl_packet* map_packet(char* path, header* image_header, info_header* image_info);