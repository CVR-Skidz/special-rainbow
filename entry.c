#ifndef UNICODE //UNICODE
#define UNICODE
#endif //UNICODE

#include "entry.h"

program_debug debug;

LRESULT CALLBACK WindowProc(HWND window_handle, UINT u_message, WPARAM w_param, LPARAM l_param);
void read_image(char* path);

int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE p_instance, wchar_t* arguments, int display_flag)
{
	//window class for application
	WNDCLASS window_class = { 0 };

	window_class.lpfnWndProc = WindowProc;			//function pointer to window behaviour
	window_class.hInstance = h_instance;			//appplication handle
	window_class.lpszClassName = L"Sample Window Class";

	RegisterClass(&window_class);					//apply class properties

	//create window
	HWND window_handle = CreateWindowEx(						//handle for window
		0,								//optional styles
		window_class.lpszClassName,		//window class
		L"Special Rainbow",				//window title
		WS_OVERLAPPEDWINDOW,			//window style

		//size and coords
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,

		NULL,							//parent
		NULL,							//menu
		h_instance,						//instance handle
		NULL							//addtitional application data
	);

	if (!window_handle)
		return 0;

	ShowWindow(window_handle, display_flag);

	//set debug points
	debug_init(debug);

	//message loop with OS
	MSG message = { 0 };

	read_image("test_image.bmp");

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

			EndPaint(window_handle, &ps);
		}
		return 0;
	}

	return DefWindowProc(window_handle, u_message, w_param, l_param);
}

void read_image(char* path)
{
	header image_header = { 0 };
	info_header image_info = { 0 };

	if (!bitmap_header(path, &image_header))
	{
		debug.set_output(&debug, L"Error Opening File\n");
		WriteConsole(debug.console_handle, debug.output_buffer, debug.output_length, debug.output_result, 0);
	}
	else
	{
		debug.set_output(&debug, image_header.summary);
		WriteConsole(debug.console_handle, debug.output_buffer, debug.output_length, debug.output_result, 0);
	}

	if (!bitmap_info(path, &image_info))
	{
		debug.set_output(&debug, L"Error Opening File\n");
		WriteConsole(debug.console_handle, debug.output_buffer, debug.output_length, debug.output_result, 0);
	}
	else
	{
		debug.set_output(&debug, image_info.summary);
		WriteConsole(debug.console_handle, debug.output_buffer, debug.output_length, debug.output_result, 0);
	}
}