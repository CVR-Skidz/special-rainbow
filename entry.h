#pragma once
#include <windows.h>
#include "bm_reader.h"
#include "debug.h"
#include "rl_encoder.h"

LRESULT CALLBACK WindowProc(HWND window_handle, UINT u_message, WPARAM w_param, LPARAM l_param);
void draw_image(HDC device_context);

