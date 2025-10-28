#include "LeftPanel.h"

#include "AppContext.h"
#include "Controls.h"
#include "Execution.h"

static WinBox lTop, lMid, lBottom;

void InitLeft(HWND parent, WinBox* parentBox) {
    AppContext* ctx = GetAppContext();

    // 상단 박스 (시나리오 관련)
    WinBoxNew(&lTop, 1, 0);
    HWND label = LabelNew(parent, L"시나리오");
    ctx->combo = ComboBoxNew(parent, IDC_SCENARIO_COMBO);
    ctx->currentLineIdx = -1;
    WinBoxPackHWND(&lTop, label, 0, 20, 0);
    WinBoxPackHWND(&lTop, ctx->combo, 0, 0, 1);

    // 중간 박스
    WinBoxNew(&lMid, 1, 0);
    HWND labelCmd = LabelNew(parent,
        L"입력 안내:\n"
        L"ALLOC [NAME] [SIZE]\n"
        L"FREE [NAME]\n"
        L"- NAME: 변수명은 영어로만, SIZE: 정수만\n"
        L"- STEP시 오류 발생해도 계속 진행 가능\n"
        L"- 실행 버튼은 계속 진행 불가능"
    );
    ctx->edit = TextViewNew(parent, IDC_SCENARIO_EDIT);
    WinBoxPackHWND(&lMid, labelCmd, 0, 100, 0);
    WinBoxPackHWND(&lMid, ctx->edit, 0, 0, 1);

    // 하단 박스 (버튼 관련)
    WinBoxNew(&lBottom, 0, 8);
    ctx->btnStep = ButtonNew(parent, IDC_STEP_BUTTON, L"Step");
    ctx->btnRun = ButtonNew(parent, IDC_RUN_BUTTON, L"실행");
    WinBoxPackHWND(&lBottom, ctx->btnStep, 0, 0, 1);
    WinBoxPackHWND(&lBottom, ctx->btnRun, 0, 0, 1);

    // 배치
    WinBoxPackBox(parentBox, &lTop, 0, 50, 0);
    WinBoxPackBox(parentBox, &lMid, 0, 0, 1);
    WinBoxPackBox(parentBox, &lBottom, 0, 30, 0);
}

void HandleLeftCommand(HWND hwnd, int wmId, int wmEvent) {
    AppContext* ctx = GetAppContext();
    // 실행 버튼 클릭
    if (wmId == IDC_RUN_BUTTON && wmEvent == BN_CLICKED) {
        if (Edit_GetTextLength(ctx->edit) <= 0) return;

        SetUIEnabled(FALSE);
        int result = ExecuteAllLines(hwnd);
        HandleExecutionError(hwnd, result);
    }
    // step 버튼 클릭
    else if (wmId == IDC_STEP_BUTTON && wmEvent == BN_CLICKED) {
        if (ctx->currentLineIdx == -1) {
            int initRes = InitExecution(hwnd);
            if (initRes != OK) {
                HandleExecutionError(hwnd, initRes);
                return;
            }
            ctx->currentLineIdx = 0;
            InvalidateRect(hwnd, NULL, TRUE);
        }

        int result = StepExecute(hwnd);
        HandleExecutionError(hwnd, result);

        int totalLineCnt = Edit_GetLineCount(ctx->edit);
        if (ctx->currentLineIdx >= totalLineCnt) {
            ctx->currentLineIdx = -1;
            SetUIEnabled(TRUE);
        }
        else {
            SetUIEnabled(FALSE);
        }
        RefreshMemoryWindows();
    }
    else if (wmId == IDC_SCENARIO_COMBO && wmEvent == CBN_SELCHANGE) {
        int idx = ComboBox_GetCurSel(ctx->combo);
        Edit_SetText(ctx->edit, GetScenario(idx));
        ctx->currentLineIdx = -1;
        ctx->lastExecutedLineIdx = 0;
        InvalidateRect(hwnd, NULL, TRUE);
    } else if (wmId == IDC_SCENARIO_EDIT && wmEvent == EN_CHANGE) {
        ctx->currentLineIdx = -1;
        ctx->lastExecutedLineIdx = 0;
    }
}