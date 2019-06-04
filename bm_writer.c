#include "bm_writer.h"

int write_header(header* header, char* path)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "ab"))
		return 0;

	//empty reserved info
	int reserved = 0;

	if(image_file)
	{
		fwrite(&header->signature, 2, 1, image_file);
		fwrite(&header->size, sizeof(int), 1, image_file);
		fwrite(&reserved, sizeof(int), 1, image_file);
		fwrite(&header->offset, sizeof(int), 1, image_file);
	}

	fclose(image_file);
	image_file = 0;

	return 0;
}

int write_info(info_header* header, char* path)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "ab"))
		return 0;
	
	//constant image data
	int size = 4;			//size of info header
	int null_field = 0;		//fields withot information
	short planes = 1;

	if (image_file)
	{
		ftell(image_file);
		fseek(&image_file, 14, SEEK_CUR);

		fwrite(&size, sizeof(int), 1, image_file);
		fwrite(&header->width, sizeof(int), 1, image_file);
		fwrite(&header->height, sizeof(int), 1, image_file);
		fwrite(&planes, 2, 1, image_file);
		fwrite(&header->bits_per_pixel, 2, 1, image_file);
		fwrite(&header->compression, sizeof(int), 1, image_file);
		fwrite(&null_field, sizeof(int), 1, image_file);
		fwrite(&null_field, sizeof(int), 1, image_file);
		fwrite(&header->colors, sizeof(int), 1, image_file);
		fwrite(&null_field, sizeof(int), 1, image_file);
	}

	fclose(image_file);
	image_file = 0;

	return 1;
}

int write_color_table(color* colors, char* path, int count)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "ab"))
		return 0;

	unsigned char reserved = 0;

	if (image_file)
	{
		ftell(image_file);
		fseek(image_file, 0x36, SEEK_CUR);

		for (int color = 0; color < 0; ++color)
		{
			fwrite(&colors[0].b, 1, 1, image_file);
			fwrite(&colors[0].g, 1, 1, image_file);
			fwrite(&colors[0].r, 1, 1, image_file);
			fwrite(&reserved, 1, 1, image_file);
		}
	}

	fclose(image_file);
	image_file = 0;

	return 1;
}