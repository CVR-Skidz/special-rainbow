#include "bm_reader.h"

image bitmap(char* path)
{
	header image_header;
	info_header image_info;

	if (!bitmap_header(path, &image_header))
	{
		#ifdef DEBUG
		debug.set_output(&debug, L"Error Opening File\n");
		debug_log(debug);
		#endif

		return (image) { 0, 0, READ_PATH_ERROR };
	}
		
	if (!bitmap_info(path, &image_info))
	{
		#ifdef DEBUG
		debug.set_output(&debug, L"Error Opening File\n");
		debug_log(debug);
		#endif

		return (image) { 0, 0, READ_PATH_ERROR };
	}

	if (image_info.bits_per_pixel == 24)
	{
		return map_pixels_24(path, &image_info, &image_header);
	}

	//read color pallete
	color* color_table = malloc((size_t)image_info.colors * CHANNELS);
	map_color_table(path, color_table, &image_info);

	//read in color_indexes of pixels
	unsigned char* pixel_info = malloc(image_info.padded_total);
	map_pixels(path, &image_header, pixel_info, &image_info);

	//pixels of image
	pixel** image_pixels = malloc(image_info.total * sizeof(pixel));
	pixel_array(pixel_info, &image_info, color_table, image_pixels);

	free(pixel_info);
	pixel_info = 0;

	return (image) { image_header, image_info, NOERR, image_pixels, color_table};
}

int pixel_array(unsigned char* pixel_info, info_header* header, color* color_table,pixel** image_pixels)
{
	//support for upto 16 bit color
	int end = (header->width + header->padding) * header->height - 1;

	for (int r_pixel = 0; r_pixel < header->height; ++r_pixel)
	{
		image_pixels[r_pixel] = malloc(sizeof(pixel)*header->width);
		for (int c_pixel = 0; c_pixel < header->width; ++c_pixel)
		{
			//TODO rework end of data and method of moving index backwards
			int table_index = pixel_info[end - (header->width + header->padding)*(r_pixel+1) + (c_pixel+1)];
			image_pixels[r_pixel][c_pixel].r = color_table[table_index].r;
			image_pixels[r_pixel][c_pixel].g = color_table[table_index].g;
			image_pixels[r_pixel][c_pixel].b = color_table[table_index].b;
			image_pixels[r_pixel][c_pixel].a = 1;
			image_pixels[r_pixel][c_pixel].x = c_pixel+1;
			image_pixels[r_pixel][c_pixel].y = r_pixel+1;
		}
	}

	return 1;
}

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

		//set_header_summary(output);

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
		float bytes_per_scan_line = output->width / ((float)BYTE_SIZE_B / output->bits_per_pixel);
		output->padding = ((float)nibble_ceil(bytes_per_scan_line) - bytes_per_scan_line) * BYTE_SIZE_B / output->bits_per_pixel;
		output->padded_total = output->total + output->height * output->padding;
		output->colors = (int)powf(2, output->bits_per_pixel);

		//set_info_summary(output);

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

		int pixels_per_byte = BYTE_SIZE_B / image_info->bits_per_pixel;

		for (int pixel = 0; pixel < image_info->padded_total; pixel += pixels_per_byte)
		{
			unsigned char byte_stream_buffer = 0;
			fread(&byte_stream_buffer, ceilf((float)1/pixels_per_byte), 1, image_file);

			for (int color = pixel; color < pixels_per_byte + pixel; ++color)
			{
				//set value to byte containing all pixel colors
				raw_buffer[color] = byte_stream_buffer;
				//push wrong pixel data out of value
				raw_buffer[color] <<= (BYTE_SIZE_B / (int)pixels_per_byte) * (color - pixel);
				//revert original position of value
				(unsigned char)raw_buffer[color] >>= BYTE_SIZE_B - image_info->bits_per_pixel;
			}
		}
	}

	fclose(image_file);

	return 1;
}

image map_pixels_24(char* path, info_header* image_info, header* image_header)
{
	FILE* image_file;
	
	if (fopen_s(&image_file, path, "rb"))
		return (image) { 0, 0, READ_PATH_ERROR, 0, 0 };

	pixel** image_pixels = malloc(image_info->total * sizeof(pixel));

	ftell(image_file);
	fseek(image_file, image_header->offset, SEEK_CUR);

	for (int r_pixel = image_info->height - 1; r_pixel >= 0; --r_pixel)
	{
		image_pixels[r_pixel] = malloc(image_info->width * sizeof(pixel));

		for (int c_pixel = 0; c_pixel < image_info->width; ++c_pixel)
		{
			ZeroMemory(&image_pixels[r_pixel][c_pixel], sizeof(pixel));

			fread(&image_pixels[r_pixel][c_pixel].b, 1, 1,image_file);
			fread(&image_pixels[r_pixel][c_pixel].g, 1, 1,image_file);
			fread(&image_pixels[r_pixel][c_pixel].r, 1, 1,image_file);
			image_pixels[r_pixel][c_pixel].a = 1;
			image_pixels[r_pixel][c_pixel].x = c_pixel + 1;
			image_pixels[r_pixel][c_pixel].y = r_pixel + 1;
		}

		ftell(image_file);
		fseek(image_file, image_info->padding / BYTE_SIZE_B, SEEK_CUR);
	}

	return (image) { *image_header, * image_info, NOERR, image_pixels, 0 };
}

int map_color_table(char* path, color* table, info_header* image_info)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "rb"))
		return 0;

	if (image_file)
	{
		//position stream to color data
		ftell(image_file);
		fseek(image_file, 54, SEEK_CUR);

		for (int color = 0; color < image_info->colors; ++color)
		{
			fread(&table[color].b, 1, 1, image_file);
			fread(&table[color].g, 1, 1, image_file);
			fread(&table[color].r, 1, 1, image_file);
			fread(&table[color].reserved, 1, 1, image_file);
		}

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
	wchar_t* format = L"size:%d bytes\noffset:%d bytes\n";
	size_t format_length = wcslen(format) + offset_length + size_length + 1;
	file_header->summary = malloc(format_length);

	//set summary
	if (file_header->summary)
		StringCbPrintf(file_header->summary, format_length, format, file_header->size, file_header->offset);
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
	size_t format_length = wcslen(format) + compression_length + width_length + height_length + colors_length;
	file_header->summary = malloc(format_length);

	//set summary
	if (file_header->summary)
	{
		StringCbPrintf(file_header->summary, format_length, format,
			file_header->width, file_header->height, file_header->bits_per_pixel / CHANNELS, file_header->compression);
	}
}

int integer_digits(int value)
{
	for (int index = 6; index >= 0; --index)
	{
		if ((value - (int)pow(10, index)) >= 0)
			return index + 1;
	}

	return 0;
}

int nibble_ceil(float bytes)
{
	if (bytes)
	{
		int ceil = 0;

		while (ceil < bytes)
		{
			ceil += NIBBLE_SIZE_B;
		}

		return ceil;
	}
	
	return 0;
}