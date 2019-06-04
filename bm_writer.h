#pragma once
#include "bm_reader.h"
#include <stdio.h>

//storage functions
int write_header(header* header, char* path);
int write_info(info_header* header, char* path);
int write_color_table(color* colors, char* path, int count);