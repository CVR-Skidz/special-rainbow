#include "rl_decoder.h"

rl_packet* map_packets(char* path, header* image_header, info_header* image_info)
{
	FILE* image_file;

	if (fopen_s(&image_file, path, "rb"))
	{
		return 0;
	}

	int packet_count = image_header->size - image_header->offset;
	int packet_size = 2 / (BYTE_SIZE_B / image_info->bits_per_pixel);

	if (image_file)
	{
		ftell(image_file);
		fseek(image_file, image_header->offset, SEEK_CUR);

		rl_packet* packets = malloc(packet_count / packet_size);

		for (int packet = 0; packet < packet_count / packet_size; ++packet)
		{
			unsigned char packet_buffer = 0;

			fread(&packet_buffer, packet_size, 1, image_file);
			packets[packet].run_count = packet_buffer >> image_info->bits_per_pixel;

			packets[packet].run = packet_buffer << image_info->bits_per_pixel;
			packets[packet].run >>= image_info->bits_per_pixel;
		}
	}
}