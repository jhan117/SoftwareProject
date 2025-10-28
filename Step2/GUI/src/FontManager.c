#include "FontManager.h"
#include "resource.h"

static HANDLE g_hAddFont = NULL;

HFONT LoadEmbeddedFont(int height) {
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    if (!hRes) return NULL;

    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData) return NULL;

    void* pData = LockResource(hData);
    DWORD dataSize = SizeofResource(hModule, hRes);
    if (!pData || dataSize == 0) return NULL;

    DWORD cFonts = 0;
    g_hAddFont = AddFontMemResourceEx(pData, dataSize, 0, &cFonts);

    return CreateFontW(
        height, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        HANGEUL_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Noto Sans KR"
    );
}

void SetFontAll(HWND hwnd, HFONT hFont) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        SetFontAll(child, hFont);
        child = GetWindow(child, GW_HWNDNEXT);
    }
}

void ReleaseFont(HFONT* hFont, HANDLE* hAddFont) {
    if (hFont && *hFont) {
        DeleteObject(*hFont);
        *hFont = NULL;
    }
    if (hAddFont && *hAddFont) {
        RemoveFontMemResourceEx(*hAddFont);
        *hAddFont = NULL;
    }
}
