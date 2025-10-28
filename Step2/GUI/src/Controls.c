#include "Controls.h"

#include "AppContext.h"

// gtk_label_new
HWND LabelNew(HWND parent, LPCWSTR text) {
    return CreateWindowEx(
        0,
        L"STATIC",
        text,
        WS_CHILD | WS_VISIBLE | SS_LEFT, // 텍스트 왼쪽 정렬
        0, 0, 0, 0,
        parent,
        NULL,
        NULL,
        NULL
    );
}

// gtk_entry_new
HWND EntryNew(HWND parent, int id) {
    AppContext* ctx = GetAppContext();
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, // 테두리 음영
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | ES_NUMBER, // 숫자만 입력 가능
        0, 0, 0, 0,
        parent,
        (HMENU)id,  // control id -> hMenu에 등록
        NULL,
        NULL
    );

    Edit_SetText(hwnd, L"108"); // 기본값 설정
    ctx->userMemSize = 108;

    return hwnd;
}

// gtk_text_view_new
HWND TextViewNew(HWND parent, int id) {
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, // 수직 스크롤바, 멀티라인 포함
        0, 0, 0, 0,
        parent,
        (HMENU)id,
        NULL,
        NULL
    );
    Edit_SetText(hwnd, GetScenario(0));
    return hwnd;
}

// gtk_button_new_with_label
HWND ButtonNew(HWND parent, int id, LPCWSTR text) {
    return CreateWindowEx(
        0,
        L"BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 0, 0,
        parent,
        (HMENU)id,
        NULL,
        NULL
    );
}

// gtk_check_button_new
HWND CheckButtonNew(HWND parent, int id, LPCWSTR text) {
    return CreateWindowEx(
        0,
        L"BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, // 자동 체크 박스
        0, 0, 0, 0,
        parent,
        (HMENU)id,
        NULL,
        NULL
    );
}

// gtk_combo_box_text_new
HWND ComboBoxNew(HWND parent, int id) {
    HWND hwnd = CreateWindowEx(
        0,
        L"COMBOBOX",
        NULL,
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, // 입력 불가 콤보박스
        0, 0, 0, 0,
        parent,
        (HMENU)id,
        NULL,
        NULL
    );

    for (int i = 0; i < 3; i++)
        ComboBox_AddString(hwnd, GetCombo(i));

    ComboBox_SetCurSel(hwnd, 0);

    return hwnd;
}

HWND ChildWindowNew(HWND parent, int id) {
    AppContext* ctx = GetAppContext();
    HWND hwnd = CreateWindowEx(
        0,
        L"Memory Window Class",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL, // 가로 스크롤바 포함하여 자식 창 생성
        0, 0, 0, 0,
        parent,
        (HMENU)id,
        NULL,
        NULL
    );

    // 스크롤 정보 설정
    SCROLLINFO si = { sizeof(si) };
    RECT rc;
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS; // 가져올 배개변수 지정
    si.nMin = 0;
    si.nMax = 0;
    GetClientRect(hwnd, &rc);
    si.nPage = rc.right - rc.left;
    si.nPos = 0;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

    return hwnd;
}
