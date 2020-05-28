#pragma once
#include "type.h"
#include "lightwindows.h"

LRESULT CALLBACK win32_windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

HWND win32_createWindow(HINSTANCE instance, WNDPROC windowProcedure, s32 width, s32 height, const char* title, void* dataPtr);
