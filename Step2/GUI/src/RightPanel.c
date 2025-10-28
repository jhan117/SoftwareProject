#include "RightPanel.h"

#include "AppContext.h"
#include "WinBox.h"
#include "Controls.h"

static WinBox rTop, rTopLeft, rBottom,
firstBox, bestBox;

void InitRight(HWND parent, WinBox* parentBox) {
    AppContext* ctx = GetAppContext();

    // 상단 박스 (총 메모리 공간 입력과 할당 방식 체크박스)
    WinBoxNew(&rTop, 0, 8);
    WinBoxNew(&rTopLeft, 1, 0);
    HWND label = LabelNew(parent, L"헤더 포함 메모리 공간 (최대 512)");
    ctx->entry = EntryNew(parent, IDC_TOTAL_MEM_ENTRY);
    WinBoxPackHWND(&rTopLeft, label, 0, 20, 0);
    WinBoxPackHWND(&rTopLeft, ctx->entry, 100, 0, 1);
    ctx->checkBtn = CheckButtonNew(parent, IDC_COMPARE_CHECK, L"할당 방식 비교 여부");
    WinBoxPackBox(&rTop, &rTopLeft, 180, 0, 0);
    WinBoxPackHWND(&rTop, ctx->checkBtn, 0, 104, 1);

    // 하단 박스 (firt-fit, best-fit 메모리 시각화)
    WinBoxNew(&rBottom, 1, 16);
    WinBoxNew(&firstBox, 1, 0);
    HWND labelFirst = LabelNew(parent, L"First-Fit Memory");
    ctx->firstMem = ChildWindowNew(parent, IDC_FIRST_MEM);
    WinBoxPackHWND(&firstBox, labelFirst, 0, 20, 0);
    WinBoxPackHWND(&firstBox, ctx->firstMem, 0, 0, 1);

    WinBoxNew(&bestBox, 1, 0);
    HWND labelBest = LabelNew(parent, L"Best-Fit Memory");
    ctx->bestMem = ChildWindowNew(parent, IDC_BEST_MEM);
    WinBoxPackHWND(&bestBox, labelBest, 0, 20, 0);
    WinBoxPackHWND(&bestBox, ctx->bestMem, 0, 0, 1);

    WinBoxPackBox(&rBottom, &firstBox, 0, 0, 1);
    WinBoxPackBox(&rBottom, &bestBox, 0, 0, 1);

    // 배치
    WinBoxPackBox(parentBox, &rTop, 0, 50, 0);
    WinBoxPackBox(parentBox, &rBottom, 0, 0, 1);

    ShowWindow(labelBest, SW_HIDE);
    ShowWindow(ctx->bestMem, SW_HIDE);
}

void HandleRightCommand(HWND hwnd, int wmId, int wmEvent) {
    AppContext* ctx = GetAppContext();

    if (wmId == IDC_COMPARE_CHECK && wmEvent == BN_CLICKED) {
        BOOL checked = (SendMessage(ctx->checkBtn, BM_GETCHECK, 0, 0) == BST_CHECKED);
        for (int i = 0; i < bestBox.childCount; i++) {
            HWND child = bestBox.hwnds[i];
            ShowWindow(child, checked ? SW_SHOW : SW_HIDE);

        }
        InvalidateRect(ctx->bestMem, NULL, TRUE);
        UpdateWindow(ctx->bestMem);
    }
}

