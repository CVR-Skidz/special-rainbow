# Special Rainbow

A Windows platform library to read Bitmap images into C structures

Special Rainbow is a library made for the windows platform that reads in bitmap images into structs for the C language. It was created to learn about reading binary data and provide a very simple altenrative to the native Windows functions for use with the Windows API.

The source code is split over four files - two headers and two c files. rl_decoder for decoding run length encoded imagesand bm_reader for reading in image data.

# Install
To use the `rainbow.lib` in your projects add the library to the external references in your solution explorer (you can drag and drop if preffered) and then include the header files in your source, make sure the header files are either specified in your linker settings or in working directory. 

If you perfer control over the library location I recommend placing the `include` and `lib directories` in the root of your project directory and link the library at the beggining of your source file with ```#pragma comment(lib,"lib/rainbow")```
