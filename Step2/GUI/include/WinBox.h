#pragma once

#include <windows.h>

#define MAX_CHILD 8

typedef struct WinBox {
    // 자식 관련 정보
    int childCount;
    struct WinBox* children[MAX_CHILD];
    HWND hwnds[MAX_CHILD];
    int isExpand[MAX_CHILD];
    int fixedWidth[MAX_CHILD];
    int fixedHeight[MAX_CHILD];

    int orientation; // 수평 0, 수직 1
    int spacing;
} WinBox;

void WinBoxNew(WinBox* box, int orientation, int spacing);
void WinBoxPackBox(WinBox* box, WinBox* childBox, int fixedW, int fixedH, int expand);
void WinBoxPackHWND(WinBox* box, HWND* hwnd, int fixedW, int fixedH, int expand);
void WinBoxLayout(HWND hwnd, WinBox* box, RECT rc);
