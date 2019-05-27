#include "file_reader.h"

int bitmap_header(char* path, header* output)
{
	//open file at path
	FILE* image;
	
	if (fopen_s(&image, path, "rb"))
		return 0;

	if (image)
	{
		//get header data
		fread(output->signature, sizeof(char), 2, image);
		fread(&output->size, sizeof(int), 1, image);
		FILE_OFFSET_16(image);
		fread(&output->offset, sizeof(int), 1, image);

		fclose(image);
	}

	return 1;
}