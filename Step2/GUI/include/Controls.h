#pragma once

#include <windows.h>
#include <windowsx.h>
#include "memory.h"

#define IDC_SCENARIO_COMBO 101
#define IDC_SCENARIO_EDIT  102
#define IDC_STEP_BUTTON    103
#define IDC_RUN_BUTTON     104

#define IDC_TOTAL_MEM_ENTRY 201
#define IDC_COMPARE_CHECK  202
#define IDC_FIRST_MEM       203
#define IDC_BEST_MEM       204

#define UNIT_BLOCK_WIDTH 8

HWND LabelNew(HWND parent, LPCWSTR text);
HWND EntryNew(HWND parent, int id);
HWND TextViewNew(HWND parent, int id);
HWND ButtonNew(HWND parent, int id, LPCWSTR text);
HWND CheckButtonNew(HWND parent, int id, LPCWSTR text);
HWND ComboBoxNew(HWND parent, int id);
HWND ChildWindowNew(HWND parent, int id);