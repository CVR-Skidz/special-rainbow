# Special Rainbow
## A Windows platform library to read Bitmap images into C structures
### Callum Van Rooyen | [CVR-Skidz](https://github.com/cvr-skidz)

Special Rainbow is a library made for the windows platform that reads bitmap images into structs for the C language.
It was created to learn about reading binary data and provide a very simple altenrative to the native Windows functions.
**The source code is split over four files - two header and c files. [rl_decoder](#rl_decoder) for decoding run length encoded images and [bm_reader](#bm_reader) for reading in image data**

> TL;DR <br>
To read in a bitmap images data call the `struct t_image bitmap(char* path)` function, this returns an image structure you can use to draw the image and access its information. `void draw_image(HDC device_context, image* bitmap)` will draw the image, this is intended for use with the Windows API in the paint event.

## bm_reader

### Defines
`FILE_OFFSET_16(stream)` <br>
Offsets the position to read from in a file forward 16 bits.
This is useful for skipping unused header information that is 2 bytes long. <br>
**Params**
+ `stream` - the file strem to offset <br>

resolves to `ftell(stream); fseek(stream, 2, SEEK_CUR);`

`FILE_OFFSET_32(stream)` <br>
Offsets the position to read from in a file forward 32 bits.
This is useful for skipping unused header information that is 4 bytes long. <br>
**Params**
+ `stream` - the file strem to offset <br>

resolves to `ftell(stream); fseek(stream, 4, SEEK_CUR);`

`CHANNELS` <br>
The number of channels bitmap images store information for. <br>
resolves to `4`

`BYTE_SIZE_B` <br>
The size of a byte in bits. <br>
resolves to `8`

`NIBBLE_SIZE_B` <br>
The size of a nibble in bits. <br>
resolves to `4`

### Usable data structures and enumerations

`typedef struct t_header header`<br>
The image file header.<br>
**Members**
+ `char signature[2]` - signature of the file (always 'BM')
+ `int size`- the size of the image
+ `int offset` - position in file where color tables (if any) are stored.
+ `wchar_t* summary` - a summary of the header information in  a wide character string printable in WinAPI unicode GUIs.

resolves to `struct t_header` 


## rl_decoder

