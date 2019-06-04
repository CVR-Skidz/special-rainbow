#include "bm_writer.h"

int write_bitmap(image* bitmap, char* path)
{
	if (!write_header(&bitmap->header, path))
		return 0;
	if (!write_info(&bitmap->info, path))
		return 0;
	if (!write_color_table(bitmap->color_table, path, bitmap->info.colors))
		return 0;
	if (!write_pixel_data(bitmap->pixels, &bitmap->info, bitmap->color_table, path))
		return 0;
}

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

	return 1;
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
		fseek(image_file, 14, SEEK_CUR);

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

		for (int color = 0; color < count; ++color)
		{
			fwrite(&colors[color].b, 1, 1, image_file);
			fwrite(&colors[color].g, 1, 1, image_file);
			fwrite(&colors[color].r, 1, 1, image_file);
			fwrite(&reserved, 1, 1, image_file);
		}
	}

	fclose(image_file);
	image_file = 0;

	return 1;
}

int write_pixel_data(pixel** pixels, info_header* header, color* colors, char* path)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "ab"))
		return 0;

	if (image_file)
	{
		unsigned char padded_value = 0;

		ftell(image_file);
		fseek(image_file, 0x36 + header->colors * NIBBLE_SIZE_B, SEEK_CUR);

		for (int r_pixel = header->height - 1; r_pixel >= 0; --r_pixel)
		{
			for (int c_pixel = 0; c_pixel < header->width; ++c_pixel)
			{
				unsigned char table_index = 0;

				//store indexes according to amount of pixels per byte
				for (int color = 0; color < BYTE_SIZE_B / header->bits_per_pixel; ++color)
				{
					table_index |= color_index(pixels[r_pixel][c_pixel], colors, header->colors);
					table_index <<= (BYTE_SIZE_B - header->bits_per_pixel) * ~color;
				}

				fwrite(&table_index, 1, 1, image_file);
			}

			for (int padding = 0; padding < header->padding; ++padding)
			{
				fwrite(&padded_value, 1, 1, image_file);
			}
		}
	}

	return 1;
}

