#include "file_reader.h"

image bitmap(char* path)
{
	header image_header;
	info_header image_info;

	if (!bitmap_header(path, &image_header))
		return (image) { 0, 0, READ_PATH_ERROR };

	if (!bitmap_info(path, &image_info))
		return (image) { 0, 0, READ_PATH_ERROR };
	
	char* pixel_info = malloc(image_info.total);
	map_pixels(path, &image_header, pixel_info, &image_info);

	return (image) { image_header, image_info, NOERR };
}

/*
pixel** pixel_array(int* pixel_info, info_header* header)
{
	pixel** image_pixels = malloc(header->total*sizeof(pixel));

	for (int r_pixel = 0; r_pixel < header->height; ++r_pixel)
	{
		for(int c_pixel = 0; c_pixel < header->width; ++c_pixel)
		{
			pixel pixel_buffer;
			pixel_buffer.x = c_pixel + 1;
			pixel_buffer.y = r_pixel + 1;

			split_channels(&pixel_buffer, c_pixel, r_pixel * header->width, pixel_info);

			image_pixels[r_pixel][c_pixel] = pixel_buffer;
		}
	}
}
*/

/*
void split_channels(pixel* pixel, int pixel_row, int column_offset, int* pixel_info)
{
	for (int color_channel = column_offset + pixel_row * CHANNELS; color_channel < CHANNELS; ++color_channel)
	{
		switch (color_channel)
		{
		case RED:
			pixel->r = pixel_info[color_channel] * 0xff;
			break;
		case GREEN:
			pixel->g = pixel_info[color_channel] * 0xff;
			break;
		case BLUE:
			pixel->b = pixel_info[color_channel] * 0xff;
			break;
		case ALPHA:
			pixel->a = pixel_info[color_channel] * 0xff;
			break;
		}
	}

}
*/

int bitmap_header(char* path, header* output)
{
	//open file at path
	FILE* image_file;
	
	if (fopen_s(&image_file, path, "rb"))
		return 0;

	if (image_file)
	{
		//get header data
		fread(output->signature, sizeof(char), 2, image_file);
		fread(&output->size, sizeof(int), 1, image_file);

		//skip reserved info
		FILE_OFFSET_32(image_file);
		fread(&output->offset, sizeof(int), 1, image_file);

		set_header_summary(output);

		fclose(image_file);
	}

	return 1;
}

int bitmap_info(char* path, info_header* output)
{
	//get image file
	FILE* image_file;

	if (fopen_s(&image_file, path, "rb"))
		return 0;

	if (image_file)
	{
		//skip image header
		ftell(image_file);
		fseek(image_file, 14, SEEK_CUR);

		//skip info_header size
		FILE_OFFSET_32(image_file);

		//get resolution
		fread(&output->width, sizeof(int), 1, image_file);
		fread(&output->height, sizeof(int), 1, image_file);

		//skip planes
		FILE_OFFSET_16(image_file);

		fread(&output->bits_per_pixel, sizeof(short), 1, image_file);
		fread(&output->compression, sizeof(int), 1, image_file);

		output->total = output->width * output->height;

		set_info_summary(output);

		fclose(image_file);
	}

	return 1;
}

int map_pixels(char* path, header* image_header, char* raw_buffer, info_header* image_info)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "rb"))
		return 0;

	if (image_file)
	{
		//set position to pixel data
		ftell(image_file);
		fseek(image_file, image_header->offset, SEEK_CUR);

		for (int pixel = 0; pixel < image_info->total; pixel+=2)
		{
			//TODO fix loop to loop 1 byte at a time - storing two pixels color
			for (int colour = pixel * CHANNELS; colour < (CHANNELS*2) * (pixel + 1); ++colour)
			{
				//TODO split hexadecimal value of 1 byte into two
				//then convert that to color
				raw_buffer[colour] = 0;
				fread(&raw_buffer[colour], image_info->bits_per_pixel / CHANNELS, 1, image_file);
			}
				
		}
	}

	fclose(image_file);

	return 1;
}

void set_header_summary(header* file_header)
{
	//find addtional size of buffer needed for file properties
	size_t size_length = integer_digits(file_header->size);
	size_t offset_length = integer_digits(file_header->offset);

	//set size of buffer
	wchar_t* format = L"size:\t%d bytes\noffset:\t%d bytes\n";
	file_header->summary = malloc(wcslen(format) + offset_length + size_length);

	//set summary
	if(file_header->summary)
		wsprintf(file_header->summary, format, file_header->size, file_header->offset);
}

void set_info_summary(info_header* file_header)
{
	//find addtional size of buffer needed for file properties
	size_t compression_length = integer_digits(file_header->compression);
	size_t width_length = integer_digits(file_header->width);
	size_t height_length = integer_digits(file_header->height);
	size_t colors_length = integer_digits(file_header->bits_per_pixel);

	//set size of buffer
	wchar_t* format = L"resolution:\t%d px x %d px\nbits per channel:\t%d\ncompression type:\t%d\n";
	file_header->summary = malloc(wcslen(format) + compression_length + width_length + height_length + colors_length);

	//set summary
	if (file_header->summary)
		wsprintf(file_header->summary, format, file_header->width, file_header->height, file_header->bits_per_pixel/CHANNELS, file_header->compression);
}

int integer_digits(int value)
{
	for (int index = 6; index >= 0; --index)
	{
		if ((value - (int)pow(10, index)) >= 0)
			return index + 1;
	}
}