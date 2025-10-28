#pragma once

#include <windows.h>
#include "AppContext.h"
#include "Controls.h"
#include "memory.h"
#include <wchar.h>

extern HFONT g_hFont;

void DrawArrow(HWND hwnd, HDC hdc);
void DrawMemory(HWND hwnd, HDC hdc);