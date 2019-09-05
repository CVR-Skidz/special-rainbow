# Special Rainbow
Special rainbow is a static library for reading Bitmap image data for C/C++. A homebrew alternative to the some of the Windows SDK functions. Anything can be done with the data, <shameless plug> you could use the library to draw the image on screen even! </shameless plug>

<img src = "https://github.com/CVR-Skidz/special-rainbow/blob/master/example_outputs/lena512.png">

# Installstion
Download the latest release and add it to your external refernces.

## Visual Studio

To use the `rainbow.lib` in your projects add the library to the external references in your solution explorer (you can drag and drop if preffered) and then include the header files in your source, make sure the header files are either specified in your linker settings or working directory. 

Alternatively you could place the `include` and `lib directories` in the root of your project directory and link the library at the beggining of your source file with `#pragma comment(lib,"lib/rainbow")`

# Usage
To read bitmap data you only need one function:
`bitmap(path\to\image)`

To draw the image you should use the following in your paint context:
`draw_image(HDC context, image* image)`

# Authors
Callum Van Rooyen [CVR-Skidz](https://github.com/CVR-Skidz)
