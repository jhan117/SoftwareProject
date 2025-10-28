#pragma once

#include "WinBox.h"
#include "Controls.h"

extern WinBox mainBox;

void InitRight(HWND parent, WinBox* parentBox);
void HandleRightCommand(HWND hwnd, int wmId, int wmEvent);