#include "file_reader.h"

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

		fread(&output->bits_per_pixel, sizeof(char) * 2, 1, image_file);
		fread(&output->compression, sizeof(int), 1, image_file);

		set_info_summary(output);

		fclose(image_file);
	}

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
	wchar_t* format = L"resolution:\t%d px x %d px\nbits per channel:\t%d\ncompression type:\t%d";
	file_header->summary = malloc(wcslen(format) + compression_length + width_length + height_length + colors_length);

	//set summary
	if (file_header->summary)
		wsprintf(file_header->summary, format, file_header->width, file_header->height, file_header->bits_per_pixel/CHANNELS, file_header->compression);
}

int integer_digits(int value)
{
	for (int index = 6; index >= 0; --index)
	{
		if ((value - (int)pow(10, index)) > 0)
			return index + 1;
	}
}