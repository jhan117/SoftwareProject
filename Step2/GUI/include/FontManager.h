#pragma once

#include <windows.h>

HFONT LoadEmbeddedFont(int height);
void SetFontAll(HWND hwnd, HFONT hFont);
void ReleaseFont(HFONT* hFont, HANDLE* hAddFont);
