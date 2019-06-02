#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <Strsafe.h>
#include <windows.h>
#include <math.h>
#include "debug.h"
#define FILE_OFFSET_16(stream) {ftell(stream); fseek(stream, 2, SEEK_CUR);}
#define FILE_OFFSET_32(stream) {ftell(stream); fseek(stream, 4, SEEK_CUR);}
#define CHANNELS 4
#define BYTE_SIZE_B 8
#define NIBBLE_SIZE_B 4

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

typedef enum t_file_status
{
	NOERR,
	READ_PATH_ERROR,
	WRITE_PATH_ERROR
}file_status;

typedef struct t_color
{
	unsigned char r, g, b, reserved;
}color;

typedef struct t_info_header
{
	int width, height, total, padding, padded_total, colors;
	short bits_per_pixel;
	compression_type compression;

	wchar_t* summary;
}info_header;

typedef struct t_pixel
{
	int r, g, b, a;
	int x, y;
}pixel;

typedef struct t_image
{
	header header;
	info_header info;
	file_status status;
	pixel** pixels;
	color* color_table;
}image;

//image creation functions
image bitmap(char* path);
int pixel_array(unsigned char* pixel_info, info_header* header, color* color_table, pixel** image_pixels);

//bitmap file functions
int bitmap_header(char* path, header* output);
int bitmap_info(char* path, info_header* output);
int map_pixels(char* path, header* image_header, char* raw_data, info_header* image_info);
int map_color_table(char* path, color* table, info_header* image_info);

//data formatting functions
void set_header_summary(header* file_header);
void set_info_summary(info_header* file_header);
int integer_digits(int value);
