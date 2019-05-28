#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#define FILE_OFFSET_16(stream) {ftell(stream); fseek(stream, 2, SEEK_CUR);}
#define FILE_OFFSET_32(stream) {ftell(stream); fseek(stream, 4, SEEK_CUR);}
#define CHANNELS 4

typedef struct t_header
{
	char signature[2];
	int size, offset;
	wchar_t* summary;
}header;

typedef enum t_compression_type
{
	NONE,
	EIGHT_BIT_RLE,
	FOUR_BIT_RLE
}compression_type;

typedef struct t_info_header
{
	int width, height, total, bits_per_pixel;
	compression_type compression;

	wchar_t* summary;
}info_header;

//bitmap file functions
int bitmap_header(char* path, header* output);
int bitmap_info(char* path, info_header* output);

//data formatting functions
void set_header_summary(header* file_header);
void set_info_summary(header* file_header);
int integer_digits(int value);
