#pragma once

#include <windows.h>

#define MAX_VARS 100

typedef struct {
    HWND combo;
    HWND edit;
    HWND btnStep;
    HWND btnRun;
    HWND entry;
    HWND checkBtn;

    HWND firstMem;
    HWND bestMem;

    int userMemSize;
    int currentLineIdx; // 현재 읽고 있는 시나리오 라인 인덱스
    int lastExecutedLineIdx;
} AppContext;

typedef struct {
    WCHAR name[32];
    int addrs[2]; // 할당된 메모리 위치
    int inUse;
} Variable;

AppContext* GetAppContext();
const WCHAR* GetScenario(int index);
const WCHAR* GetCombo(int index);
int* FindVarAddrsByName(const WCHAR* name);
int AddVar(const WCHAR* name, int* addrs);
int DeleteVar(const WCHAR* name);
void ClearVars();