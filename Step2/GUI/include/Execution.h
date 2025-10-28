#pragma once

#define OK 1
#define ERR_COMMAND 0
#define ERR_ALLOC -1
#define ERR_FREE -2
#define ERR_VAR_EXISTS -3
#define ERR_ENTRY -4

#include <wchar.h>
#include "AppContext.h"
#include "Controls.h"

void HandleExecutionError(HWND hwnd, int errorCode);
void SetUIEnabled(BOOL enabled);
void RefreshMemoryWindows();
int InitExecution(HWND hwnd);
int StepExecute(HWND hwnd);
int ExecuteAllLines(HWND hwnd);