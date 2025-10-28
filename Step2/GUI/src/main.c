#include "resource.h"
#include "FontManager.h"
#include "Controls.h"
#include "WinBox.h"
#include "LeftPanel.h"
#include "RightPanel.h"
#include "AppContext.h"
#include "Draw.h"

WinBox mainBox;
HFONT g_hFont;
static WinBox leftBox, rightBox;

LRESULT CALLBACK MemoryProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        AppContext* ctx = GetAppContext();

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        if (hwnd == ctx->firstMem || hwnd == ctx->bestMem) {
            DrawMemory(hwnd, hdc);
        }

        EndPaint(hwnd, &ps);

        break;
    }
    case WM_HSCROLL: {
        SCROLLINFO si = { sizeof(si) };
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        GetScrollInfo(hwnd, SB_HORZ, &si);

        int pos = si.nPos;
        switch (LOWORD(wParam)) {
        case SB_LINELEFT: pos -= 10; break;
        case SB_LINERIGHT: pos += 10; break;
        case SB_PAGELEFT: pos -= si.nPage; break;
        case SB_PAGERIGHT: pos += si.nPage; break;
        case SB_THUMBTRACK: pos = HIWORD(wParam); break;
        }

        pos = max(si.nMin, min(pos, si.nMax - (int)si.nPage));
        si.nPos = pos;
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        RECT rc;
        GetClientRect(hwnd, &rc);

        WinBoxNew(&mainBox, 0, 8);
        WinBoxNew(&leftBox, 1, 8);
        WinBoxNew(&rightBox, 1, 8);

        InitLeft(hwnd, &leftBox);
        InitRight(hwnd, &rightBox);
        WinBoxPackBox(&mainBox, &leftBox, 200, 0, 0);
        WinBoxPackBox(&mainBox, &rightBox, 0, 0, 1);

        g_hFont = LoadEmbeddedFont(16);
        if (g_hFont) SetFontAll(hwnd, g_hFont);

        break;
    }
    case WM_SIZE: {
        AppContext* ctx = GetAppContext();

        RECT rc;
        GetClientRect(hwnd, &rc);
        int padding = 16;

        rc.left += padding;
        rc.top += padding;
        rc.right -= padding;
        rc.bottom -= padding;

        WinBoxLayout(hwnd, &mainBox, rc);
        InvalidateRect(hwnd, NULL, TRUE);

        if (ctx->userMemSize != 0) {
            HWND memHwnds[] = { ctx->firstMem, ctx->bestMem };
            for (int i = 0; i < 2; i++) {
                if (!memHwnds[i]) continue;
                RECT memRc;
                GetClientRect(memHwnds[i], &memRc);

                SCROLLINFO si = { sizeof(si) };
                si.fMask = SIF_RANGE | SIF_PAGE;
                si.nMin = 0;
                si.nMax = ctx->userMemSize * UNIT_BLOCK_WIDTH;
                si.nPage = memRc.right - memRc.left;
                SetScrollInfo(memHwnds[i], SB_HORZ, &si, TRUE);
            }
        }
        break;
    }
    case WM_VSCROLL: {
        InvalidateRect(hwnd, NULL, TRUE);
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        DrawArrow(hwnd, hdc);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);

        HandleRightCommand(hwnd, wmId, wmEvent);
        HandleLeftCommand(hwnd, wmId, wmEvent);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // 윈도우 클래스 등록
    const wchar_t CLASS_NAME[] = L"Main Window Class";
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 메모리 클래스 등록
    WNDCLASS mc = { 0 };

    mc.lpfnWndProc = MemoryProc;
    mc.hInstance = hInstance;
    mc.lpszClassName = L"Memory Window Class";

    RegisterClass(&mc);

    // 메인 윈도우 생성
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,                             // 창 클래스
        L"메모리 관리자-SWP 2단계 과제",          // 창 타이틀
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,      // 창 스타일: 기본 윈도우 + 생성 후 바로 생성
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // 사이즈랑 위치: 기본값 설정
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) return 0;

    // 메시지 루프 실행
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseFont(&g_hFont, NULL);
    return 0;
}
