#define _CRT_SECURE_NO_WARNINGS

#include "Execution.h"

static const WCHAR* GetErrorMessage(int code) {
    switch (code) {
    case ERR_COMMAND: return L"잘못된 명령어입니다.";
    case ERR_ALLOC: return L"메모리 할당에 실패했습니다.";
    case ERR_VAR_EXISTS: return L"변수가 이미 존재하거나 찾을 수 없습니다.";
    case ERR_ENTRY: return L"메모리 크기를 입력하거나 범위를 확인하세요.";
    default: return L"알 수 없는 오류입니다.";
    }
}

void HandleExecutionError(HWND hwnd, int errorCode) {
    if (errorCode == OK) return;

    MessageBox(hwnd, GetErrorMessage(errorCode), L"실행 오류", MB_ICONERROR);
    SetUIEnabled(TRUE);
    RefreshMemoryWindows();
}

void SetUIEnabled(BOOL enabled) {
    AppContext* ctx = GetAppContext();
    EnableWindow(ctx->combo, enabled);
    SendMessage(ctx->edit, EM_SETREADONLY, !enabled, 0);
    SendMessage(ctx->entry, EM_SETREADONLY, !enabled, 0);
}

void RefreshMemoryWindows() {
    AppContext* ctx = GetAppContext();
    RedrawWindow(ctx->firstMem, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    RedrawWindow(ctx->bestMem, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

static int IsNumber(const WCHAR* str) {
    if (!str || *str == L'\0') return 0;

    // 앞쪽 공백 건너뛰기
    while (iswspace(*str)) str++;

    // optional + or -
    if (*str == L'+' || *str == L'-') str++;

    // 숫자 없으면 실패
    if (!iswdigit(*str)) return 0;

    // 모든 글자가 숫자인지 확인
    while (*str) {
        if (!iswdigit(*str)) return 0;
        str++;
    }

    return 1;
}

static int ExecuteLine(WCHAR* line) {
    WCHAR* seps = L" \t\n";
    WCHAR* context = NULL;
    WCHAR* token;

    WCHAR command[16] = L"";
    WCHAR name[16] = L"";
    WCHAR size[16] = L"";
    WCHAR* tokens[3] = { command, name, size };

    int i = 0;
    token = wcstok(line, seps, &context);
    while (token && i < 3) {
        size_t len = wcslen(token);
        if (len >= 16) len = 15;

        wcscpy_s(tokens[i], 16, token);

        i++;
        token = wcstok(NULL, seps, &context);
    }

    if (wcscmp(command, L"ALLOC") == 0) {
        if (!IsNumber(size)) return ERR_COMMAND;
        int sz = _wtoi(size);
        int* addrs = my_alloc(sz);
        if (!addrs || addrs[0] == -1 || addrs[1] == -1) return ERR_ALLOC;

        return AddVar(name, addrs) ? OK : ERR_VAR_EXISTS;
    }
    else if (wcscmp(command, L"FREE") == 0) {
        int* addrs = FindVarAddrsByName(name);
        if (!addrs) return ERR_VAR_EXISTS;
        my_free(addrs);

        return DeleteVar(name) ? OK : ERR_VAR_EXISTS;
    }
    else return ERR_COMMAND;
}

int InitExecution(HWND hwnd) {
    AppContext* ctx = GetAppContext();
    // 전체 메모리 크기 설정
    int entryLength = Edit_GetTextLength(ctx->entry);
    if (entryLength <= 0) return ERR_ENTRY;
    if (entryLength > 16) entryLength = 15;

    WCHAR buffer[16];
    Edit_GetText(ctx->entry, buffer, entryLength + 1);
    buffer[entryLength] = L'\0';
    int memSize = _wtoi(buffer);

    if (memSize <= 0 || memSize > MAX_MEM_SIZE) return ERR_ENTRY;
    ctx->userMemSize = memSize;

    // 초기화 작업
    init_memory(2, ctx->userMemSize);
    ClearVars();

    SCROLLINFO si = { sizeof(si) };
    RECT rc;
    GetClientRect(ctx->firstMem, &rc);

    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = max(0, ctx->userMemSize * UNIT_BLOCK_WIDTH);
    si.nPage = rc.right - rc.left;
    si.nPos = 0;

    SetScrollInfo(ctx->firstMem, SB_HORZ, &si, TRUE);
    SetScrollInfo(ctx->bestMem, SB_HORZ, &si, TRUE);

    return OK;
}

int StepExecute(HWND hwnd) {
    AppContext* ctx = GetAppContext();
    if (ctx->currentLineIdx == -1) {
        int initRes = InitExecution(hwnd);
        if (initRes != OK) return initRes;
    }
    if (ctx->currentLineIdx >= Edit_GetLineCount(ctx->edit)) {
        ctx->lastExecutedLineIdx = ctx->currentLineIdx - 1;
        ctx->currentLineIdx = -1;
        InvalidateRect(hwnd, NULL, TRUE);
        return OK;
    }

    WCHAR buffer[256];
    int textLen = Edit_GetLine(ctx->edit, ctx->currentLineIdx, buffer, 255);
    buffer[textLen] = L'\0';

    if (textLen == 0 || buffer[0] == L'\0') {
        ctx->currentLineIdx++;
        InvalidateRect(hwnd, NULL, TRUE);
        return OK;
    }

    int result = ExecuteLine(buffer);
    ctx->lastExecutedLineIdx = ctx->currentLineIdx;
    ctx->currentLineIdx++;
    InvalidateRect(hwnd, NULL, TRUE);
    return result;
}

int ExecuteAllLines(HWND hwnd) {
    AppContext* ctx = GetAppContext();
    int initRes = InitExecution(hwnd);
    if (initRes != OK) return initRes;
    ctx->currentLineIdx = 0;
    InvalidateRect(hwnd, NULL, TRUE);

    int totalLineCnt = Edit_GetLineCount(ctx->edit);
    while (ctx->currentLineIdx < totalLineCnt) {
        int result = StepExecute(hwnd);
        if (result == OK) {
            SetUIEnabled(TRUE);
            RefreshMemoryWindows();
        }
        else {
            SetUIEnabled(TRUE);
            ctx->currentLineIdx = -1;
            InvalidateRect(hwnd, NULL, TRUE);
            return result;
        }
    }
    ctx->currentLineIdx = -1;
    InvalidateRect(hwnd, NULL, TRUE);
    return OK;
}
