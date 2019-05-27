#pragma once
#include <stdio.h>; 
#define FILE_OFFSET_16(stream) {ftell(stream); fseek(stream, 2, SEEK_CUR);}
#define FILE_OFFSET_32(stream) {ftell(stream); fseek(stream, 4, SEEK_CUR);}

typedef struct t_header
{
	char signature[2];
	int size, offset;
}header;

typedef struct t_info_header
{
	int width, height, pixels;
}info_header;

int bitmap_header(char* path, header* output);