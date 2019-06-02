#ifndef UNICODE //UNICODE
#define UNICODE
#endif //UNICODE
#include "entry.h"

image bitmap_image;

int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE p_instance, wchar_t* arguments, int display_flag)
{
	//window class for application
	WNDCLASS window_class = { 0 };

	window_class.lpfnWndProc = WindowProc;			//function pointer to window behaviour
	window_class.hInstance = h_instance;			//appplication handle
	window_class.lpszClassName = L"Sample Window Class";

	RegisterClass(&window_class);					//apply class properties

	bitmap_image = bitmap("test_image_small.bmp");

	//create window
	HWND window_handle = CreateWindowEx(			//handle for window
		0,								//optional styles
		window_class.lpszClassName,		//window class
		L"Special Rainbow",				//window title
		WS_OVERLAPPEDWINDOW,			//window style

		//size and coords
		CW_USEDEFAULT, CW_USEDEFAULT, bitmap_image.info.width+20, bitmap_image.info.height+45,

		NULL,							//parent
		NULL,							//menu
		h_instance,						//instance handle
		NULL							//addtitional application data
	);

	if (!window_handle)
		return 0;

	ShowWindow(window_handle, display_flag);

	#ifdef DEBUG
	//set debug points
	debug_init(debug);
	#endif

	//message loop with OS
	MSG message = { 0 };

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

/*
* Function: WindowProc
* --------------------
*
* Handle Window Events
*/
LRESULT CALLBACK WindowProc(HWND window_handle, UINT u_message, WPARAM w_param, LPARAM l_param)
{
	switch (u_message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window_handle, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			draw_image(hdc);

			EndPaint(window_handle, &ps);
		}
		return 0;
	}

	return DefWindowProc(window_handle, u_message, w_param, l_param);
}

void draw_image(HDC device_context)
{
	for (int r_pixel = 0; r_pixel < bitmap_image.info.height; ++r_pixel)
	{
		for (int c_pixel = 0; c_pixel < bitmap_image.info.width; ++c_pixel)
		{
				pixel curr_pixel = bitmap_image.pixels[r_pixel][c_pixel];
				SetPixel(device_context, curr_pixel.x, curr_pixel.y, RGB(curr_pixel.r, curr_pixel.g, curr_pixel.b));
		}
	}
}