#pragma once
#include "bm_reader.h"
#include <stdio.h>

int write_bitmap(image* bitmap, char* path);

//storage functions
int write_header(header* header, char* path);
int write_info(info_header* header, char* path);
int write_color_table(color* colors, char* path, int count);
int write_pixel_data(pixel** pixels, info_header* header, color* colors, char* path);
