#pragma once

#include "WinBox.h"

typedef struct {
    wchar_t name[32];
    int addrs[2];
} Vars;

void InitLeft(HWND parent, WinBox* parentBox);
void HandleLeftCommand(HWND hwnd, int wmId, int wmEvent);