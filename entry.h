#pragma once
#include <windows.h>
#include "file_reader.h"
#include "debug.h"

LRESULT CALLBACK WindowProc(HWND window_handle, UINT u_message, WPARAM w_param, LPARAM l_param);
void draw_image(HDC device_context);

