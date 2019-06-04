#pragma once
#include "rl_decoder.h"
#include <stdio.h>

typedef struct t_rl_image
{
	header header;
	info_header info;
	file_status status;
	color* color_table;
	rl_packet* packets;
}rl_image;

rl_packet* encode_pixels(image bitmap);
unsigned char color_index(pixel pixel, color* colors, int count);
unsigned char pack_channels_8(pixel pixel);
