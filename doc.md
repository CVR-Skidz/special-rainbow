# Special Rainbow
## A Windows platform library to read Bitmap images into C structures
### Callum Van Rooyen | [CVR-Skidz](https://github.com/cvr-skidz)

Special Rainbow is a library made for the windows platform that reads in bitmap images into structs for the C language.
It was created to learn about reading binary data and provide a very simple altenrative to the native Windows functions.
**The source code is split over four files - two headers and two c files. [rl_decoder](#rl_decoder) for decoding run length encoded images
and [bm_reader](#bm_reader) for reading in image data**

> TL;DR <br>
To read in a bitmap images data call the `struct t_image bitmap(char* path)` function, this returns an image structure you can use to draw the image and access its information. `void draw_image(HDC device_context, image* bitmap)` will draw the image, this is intended for use with the Windows API.

## bm_reader

## rl_decoder

