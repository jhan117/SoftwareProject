#include "WinBox.h"

#include <stdlib.h>
#include "Controls.h"

// gtk_box_new(GtkOrientation orientation, gint spacing)
void WinBoxNew(WinBox* box, int orientation, int spacing) {
    box->childCount = 0;
    box->orientation = orientation;
    box->spacing = spacing;
}

// gtk_box_pack_start(GtkBox* box, GtkWidget* child, gboolean expand, gboolean fill, guint padding)
void WinBoxPackBox(WinBox* box, WinBox* childBox, int fixedW, int fixedH, int expand) {
    int idx = box->childCount;
    box->childCount++;

    box->children[idx] = childBox;
    box->hwnds[idx] = NULL;
    box->fixedWidth[idx] = fixedW;
    box->fixedHeight[idx] = fixedH;
    box->isExpand[idx] = expand;
}
void WinBoxPackHWND(WinBox* box, HWND* hwnd, int fixedW, int fixedH, int expand) {
    int idx = box->childCount;
    box->childCount++;

    box->hwnds[idx] = hwnd;
    box->children[idx] = NULL;
    box->fixedWidth[idx] = fixedW;
    box->fixedHeight[idx] = fixedH;
    box->isExpand[idx] = expand;
}

// 배치 계산
void WinBoxLayout(HWND hwnd, WinBox* box, RECT rc) {
    int totalExpand = 0;
    int fixedSum = 0;
    int totalSpacing = (box->childCount - 1) * box->spacing;

    // 1. 고정 크기 합산 및 expand 개수 세기
    for (int i = 0; i < box->childCount; i++) {
        if (box->isExpand[i]) totalExpand++;
        else fixedSum += (box->orientation == 0 ? box->fixedWidth[i] : box->fixedHeight[i]);
    }

    int available = (box->orientation == 0 ? (rc.right - rc.left) : (rc.bottom - rc.top));
    available -= fixedSum + totalSpacing;
    int offset = (box->orientation == 0 ? rc.left : rc.top);

    // 2. child 배치
    for (int i = 0; i < box->childCount; i++) {
        int size = box->isExpand[i]
            ? (totalExpand ? available / totalExpand : 0)
            : (box->orientation == 0 ? box->fixedWidth[i] : box->fixedHeight[i]);

        RECT childRc = rc;
        if (box->orientation == 0) {
            childRc.left = offset;
            childRc.right = offset + size;
        }
        else {
            childRc.top = offset;
            childRc.bottom = offset + size;
        }

        if (box->children[i]) {
            WinBoxLayout(hwnd, box->children[i], childRc);
        }
        else if (box->hwnds[i]) {
            int childH = childRc.bottom - childRc.top;
            // combo box 드롭다운 안보임... 문제 해결
            WCHAR className[16];
            GetClassName(box->hwnds[i], className, 16);
            if (_wcsicmp(className, L"COMBOBOX") == 0) {
                childH = 100;
            }

            MoveWindow(box->hwnds[i],
                childRc.left, childRc.top,
                childRc.right - childRc.left,
                childH,
                TRUE);
        }

        offset += size + box->spacing;
    }
}