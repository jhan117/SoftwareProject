#include "Draw.h"

void DrawArrow(HWND hwnd, HDC hdc) {
    AppContext* ctx = GetAppContext();

    int lineToDraw = (ctx->currentLineIdx >= 0) ? ctx->currentLineIdx - 1 : ctx->lastExecutedLineIdx;
    if (lineToDraw < 0) return;

    RECT editRect;
    GetWindowRect(ctx->edit, &editRect);
    POINT editTopLeft = { editRect.left, editRect.top };
    ScreenToClient(hwnd, &editTopLeft);

    // 줄 높이 계산
    HDC hdcEdit = GetDC(ctx->edit);
    HFONT hFont = (HFONT)SendMessage(ctx->edit, WM_GETFONT, 0, 0);
    HFONT hOldFont = (HFONT)SelectObject(hdcEdit, hFont);

    TEXTMETRIC tm;
    GetTextMetrics(hdcEdit, &tm);
    int lineHeight = tm.tmHeight + tm.tmExternalLeading;

    SelectObject(hdcEdit, hOldFont);
    ReleaseDC(ctx->edit, hdcEdit);

    // 스크롤 반영된 줄 위치 계산
    RECT rc;
    GetClientRect(ctx->edit, &rc);
    int clientHeight = rc.bottom - rc.top;
    int maxVisibleLines = clientHeight / lineHeight;
    int firstVisibleLine = SendMessage(ctx->edit, EM_GETFIRSTVISIBLELINE, 0, 0);
    int visibleLineIndex = lineToDraw - firstVisibleLine;
    if (visibleLineIndex < 0 || visibleLineIndex >= maxVisibleLines)
        return;

    int triangleHeight = 10;
    int y = editTopLeft.y + visibleLineIndex * lineHeight + (lineHeight - triangleHeight) / 2 + 3;
    int x = editTopLeft.x - 12;

    // 삼각형 좌표
    POINT arrow[3] = {
        { x + 2, y },
        { x + 2, y + 10 },
        { x + 10, y + 5 }
    };

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Polygon(hdc, arrow, 3);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void DrawMemory(HWND hwnd, HDC hdc) {
    if (!IsWindow(hwnd)) return;
    AppContext* ctx = GetAppContext();

    HFONT hOldFont = NULL;
    if (g_hFont) {
        hOldFont = (HFONT)SelectObject(hdc, g_hFont);
    }

    RECT rc;
    GetClientRect(hwnd, &rc);
    HRGN hRgn = CreateRectRgnIndirect(&rc); // 영역 설정

    // 스크롤 위치 가져와서 거기부터 그리기
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    GetScrollInfo(hwnd, SB_HORZ, &si);

    int x = 0 - si.nPos;
    int y = 20; // 인덱스 쓸 공간 남기기
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    const int BLOCK_HEIGHT = height - 20;

    MemoryInfo infos[MAX_MEM_SIZE * 2];
    int blockCount = get_memory_infos(infos);
    for (int i = 0; i < blockCount; i++) {
        MemoryInfo* info = &infos[i];

        if ((hwnd == ctx->firstMem && info->is_best) ||
            (hwnd == ctx->bestMem && !info->is_best))
            continue;

        int blockWidth = (info->size + sizeof(Header)) * UNIT_BLOCK_WIDTH;
        int headerWidth = sizeof(Header) * UNIT_BLOCK_WIDTH;

        // 블록 그리기
        int right = x + blockWidth;
        int bottom = y + BLOCK_HEIGHT;
        // 브러시 설정 (내부 채움)
        HBRUSH hBrush = CreateSolidBrush(info->used ? RGB(0, 128, 255) : RGB(200, 200, 200));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, x, y, right, bottom);

        MoveToEx(hdc, x + headerWidth, y, NULL);
        LineTo(hdc, x + headerWidth, bottom);

        // 인덱스 정보 caption
        WCHAR buf[16];
        RECT textRect;
        SetRect(&textRect, x, y - 20, x + headerWidth, y);
        swprintf(buf, 16, L"idx = %d", info->start);
        DrawText(hdc, buf, -1, &textRect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);

        // 헤더 정보 caption
        SetBkMode(hdc, TRANSPARENT);
        RECT tempRect = { 0, 0, 1000, 1000 };

        swprintf(buf, 32, L"size: %d", info->size);
        int textHeight1 = DrawText(hdc, buf, -1, &tempRect, DT_CALCRECT | DT_LEFT | DT_TOP);

        swprintf(buf, 32, L"next: %d", info->next);
        int textHeight2 = DrawText(hdc, buf, -1, &tempRect, DT_CALCRECT | DT_LEFT | DT_TOP);

        int totalHeight = textHeight1 + textHeight2 + 4;
        int startY = y + (BLOCK_HEIGHT - totalHeight) / 2;

        RECT headerRect;
        SetRect(&headerRect, x, startY, x + headerWidth, bottom);
        swprintf(buf, 32, L"size: %d", info->size);
        textHeight1 = DrawText(hdc, buf, -1, &headerRect, DT_CENTER | DT_TOP);

        headerRect.top += textHeight1 + 4;
        swprintf(buf, 32, L"next: %d", info->next);
        DrawText(hdc, buf, -1, &headerRect, DT_CENTER | DT_TOP);

        x += blockWidth;

        DeleteObject(hBrush);
    }
    if (hOldFont) {
        SelectObject(hdc, hOldFont);
    }
    DeleteObject(hRgn);
}