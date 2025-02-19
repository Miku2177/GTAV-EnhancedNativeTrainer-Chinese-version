/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#pragma once

#include <string>
#include <windows.h>
#include <Shlwapi.h>

static int LAST_REQUEST = 0;

class SaveFileDialogCallback
{
public:
	int requestID = LAST_REQUEST++;
	void* data;
	bool complete = false;
	bool success = false;
	bool processed = false;
	std::string filePath;
};

class LoadFileDialogCallback
{
public:
	int requestID = LAST_REQUEST++;
	bool complete = false;
	bool processed = false;
	bool success = false;
	std::string filePath;
};

// 公共调用
void show_save_dialog_in_thread(std::string title, SaveFileDialogCallback* request);
void show_load_dialog_in_thread(std::string title, LoadFileDialogCallback* request);

DWORD WINAPI show_save_dialog_thread_call(LPVOID lpParameter);
DWORD WINAPI show_open_dialog_thread_call(LPVOID lpParameter);

void show_file_open_dialog(std::string title, LoadFileDialogCallback* callback);
void show_file_save_dialog(std::string title, SaveFileDialogCallback* callback);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

void find_parent_window();