#include "rl_encoder.h"

rl_image rl_bitmap(image* bitmap)
{
	rl_packet* run_packets = 0;
	int packets = encode_pixels(*bitmap, run_packets);

	return (rl_image) { bitmap->header, bitmap->info, EIGHT_BIT_RLE, bitmap->color_table, run_packets, packets};
}

int encode_pixels(image bitmap, rl_packet* output)
{
	pixel** pixels = bitmap.pixels;

	output = malloc(sizeof(rl_packet));

	//run data
	int packet_count = 0;
	int run_count = 0;
	unsigned char run = 0;

	for (int r_pixel = 0; r_pixel < bitmap.info.height; ++r_pixel)
	{
		int c_pixel = 0;

		while (c_pixel < bitmap.info.width)
		{
			//reset run data
			run_count = 0;
			run = 0;

			//calculate run
			do
			{
				run = color_index(pixels[r_pixel][c_pixel], bitmap.color_table, bitmap.info.colors);
				++run_count;
			} while (++c_pixel < bitmap.info.width && color_index( pixels[r_pixel][c_pixel], bitmap.color_table, bitmap.info.colors) == run);
		}
		
		output[packet_count++] = (rl_packet){ run_count, run };
		output = realloc(output, (packet_count + 1) * sizeof(rl_packet));
	}

	return packet_count;
}

unsigned char color_index(pixel pixel, color* colors, int count)
{
	for (unsigned char color = 0; color < count; ++color)
	{
		if (pixel.r == colors[color].r && pixel.g == colors[color].g && pixel.b == colors[color].b)
		{
			return color;
		}
	}

	return 0;
}

unsigned char pack_channels_8(pixel pixel)
{
	unsigned char packed_color = 0;

	packed_color = pixel.b;
	packed_color <<= 2;
	packed_color |= pixel.g;
	packed_color <<= 2;
	packed_color |= pixel.r;
	packed_color <<= 2;

	return packed_color;
}