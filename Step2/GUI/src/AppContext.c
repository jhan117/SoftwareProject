#include "AppContext.h"

static AppContext context;
static Variable vars[MAX_VARS];

static const WCHAR* ComboBoxItems[] = { L"시나리오1", L"시나리오2", L"시나리오3" };
static const WCHAR* TextViewItems[] = {
    L"ALLOC A 10\r\n"
    L"ALLOC B 20\r\n"
    L"ALLOC C 10\r\n"
    L"FREE A\r\n"
    L"ALLOC A 30\r\n"
    L"FREE C\r\n"
    L"ALLOC D 20\r\n"
    L"FREE B\r\n", // 시나리오 1

    L"ALLOC A 0\r\n"
    L"ALLOC B 100\r\n"
    L"ALLOC C 10\r\n"
    L"FREE C\r\n"
    L"FREE B\r\n"
    L"ALLOC D 200\r\n"
    L"ALLOC A 20\r\n"
    L"FREE B\r\n"
    L"FREE A\r\n"
    L"FREE A\r\n", // 시나리오 2

    L"ALLOC A 20\r\n"
    L"ALLOC B 20\r\n"
    L"ALLOC C 20\r\n"
    L"ALLOC D 10\r\n"
    L"FREE B\r\n"
    L"FREE D\r\n"
    L"ALLOC F 5\r\n" // 시나리오 3
};

AppContext* GetAppContext() {
    return &context;
}

const WCHAR* GetScenario(int index) {
    if (index < 0 || index >= _countof(TextViewItems)) {
        return L"";
    }
    return TextViewItems[index];
}

const WCHAR* GetCombo(int index) {
    if (index < 0 || index >= _countof(ComboBoxItems)) {
        return L"";
    }
    return ComboBoxItems[index];
}

int* FindVarAddrsByName(const WCHAR* name) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (vars[i].inUse && wcscmp(vars[i].name, name) == 0) {
            return vars[i].addrs;
        }
    }
    return NULL;
}

static int FindVarIdxByName(const WCHAR* name) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (vars[i].inUse && wcscmp(vars[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int AddVar(const WCHAR* name, int* addrs) {
    if (FindVarIdxByName(name) >= 0) return 0; // 이미 존재함

    for (int i = 0; i < MAX_VARS; i++) {
        if (!vars[i].inUse) {
            wcscpy_s(vars[i].name, 32, name);
            memcpy(vars[i].addrs, addrs, sizeof(int) * 2);
            vars[i].inUse = 1;
            return 1;
        }
    }
    return 0; // 모든 슬롯 사용 중
}

int DeleteVar(const WCHAR* name) {
    int idx = FindVarIdxByName(name);
    if (idx < 0) return 0; // 변수 찾을 수 없음

    vars[idx].name[0] = L'\0';
    vars[idx].addrs[0] = -1;
    vars[idx].addrs[1] = -1;
    vars[idx].inUse = 0;
    return 1;
}

void ClearVars() {
    for (int i = 0; i < MAX_VARS; i++) {
        vars[i].name[0] = L'\0';
        vars[i].addrs[0] = -1;
        vars[i].addrs[1] = -1;
        vars[i].inUse = 0;
    }
}