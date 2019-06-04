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

