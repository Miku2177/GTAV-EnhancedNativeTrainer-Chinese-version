/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "file_dialog.h"
#include <Windows.h>
#include <commdlg.h>

#include "..\utils.h"
#include "..\debug\debuglog.h"

#include <sstream>

static HWND parentWindow = 0;

struct SaveDialogThreadContext
{
	std::string title;
	SaveFileDialogCallback* request;
};

struct LoadDialogThreadContext
{
	std::string title;
	LoadFileDialogCallback* request;
};


void show_save_dialog_in_thread(std::string title, SaveFileDialogCallback* request)
{
	DWORD myThreadID;
	SaveDialogThreadContext* ctx = new SaveDialogThreadContext{ title, request };
	HANDLE myHandle = CreateThread(0, 0, show_save_dialog_thread_call, ctx, 0, &myThreadID);
	CloseHandle(myHandle);
}

void show_load_dialog_in_thread(std::string title, LoadFileDialogCallback* request)
{
	DWORD myThreadID;
	LoadDialogThreadContext* ctx = new LoadDialogThreadContext{ title, request };
	HANDLE myHandle = CreateThread(0, 0, show_open_dialog_thread_call, ctx, 0, &myThreadID);
	CloseHandle(myHandle);
}

void find_parent_window()
{
	EnumWindows(EnumWindowsProc, NULL);
}

DWORD WINAPI show_save_dialog_thread_call(LPVOID lpParameter)
{
	SaveDialogThreadContext *ctx = static_cast<SaveDialogThreadContext*>(lpParameter);
	show_file_save_dialog(ctx->title, ctx->request);
	delete ctx;
	return 0;
}

DWORD WINAPI show_open_dialog_thread_call(LPVOID lpParameter)
{
	LoadDialogThreadContext *ctx = static_cast<LoadDialogThreadContext*>(lpParameter);
	show_file_open_dialog(ctx->title, ctx->request);
	delete ctx;
	return 0;
}

void show_file_open_dialog(std::string title, LoadFileDialogCallback* callback)
{
	if (parentWindow == 0)
	{
		find_parent_window();
	}

	OPENFILENAME ofn;       // 通用对话框结构
	char szFile[MAX_PATH];  // 用于存储文件名的缓冲区
	HANDLE hf;              // 文件句柄

	DWORD procID = GetCurrentProcessId();

	EnumWindows(EnumWindowsProc, NULL);

	// 初始化 OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parentWindow;
	ofn.lpstrFile = szFile;
	// 将 lpstrFile[0] 设置为 '\0'，以便 GetOpenFileName 不会
	// 使用 szFile 的内容来初始化自身。
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "ENT XML Files\0*.XML\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrTitle = title.c_str();
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// 显示“打开”对话框。

	if (GetOpenFileName(&ofn) == TRUE)
	{
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (hf == INVALID_HANDLE_VALUE)
		{
			write_text_to_log_file("CreateFile 返回了无效句柄");
			std::ostringstream ss;
			ss << "选择的文件是: " << ofn.lpstrFile << " 以及错误 " << GetLastError();
			write_text_to_log_file(ss.str());
			callback->success = false;
		}
		else
		{
			CloseHandle(hf);
			callback->filePath = ofn.lpstrFile;
			callback->success = true;
		}
	}
	else
	{
		write_text_to_log_file("GetOpenFileName 返回 false");
		callback->success = false;
	}

	callback->complete = true;
}

void show_file_save_dialog(std::string title, SaveFileDialogCallback* callback)
{
	if (parentWindow == 0)
	{
		find_parent_window();
	}

	OPENFILENAME sfn;       // 通用对话框结构
	char szFile[MAX_PATH];  // 用于存储文件名的缓冲区
	HANDLE hf;              // 文件句柄

	DWORD procID = GetCurrentProcessId();

	EnumWindows(EnumWindowsProc, NULL);

	// 初始化 OPENFILENAME
	ZeroMemory(&sfn, sizeof(sfn));
	sfn.lStructSize = sizeof(sfn);
	sfn.hwndOwner = parentWindow;
	sfn.lpstrFile = szFile;
	// 将 lpstrFile[0] 设置为 '\0'，以便 GetOpenFileName 不会
	// 使用 szFile 的内容来初始化自身。
	sfn.lpstrFile[0] = '\0';
	sfn.nMaxFile = sizeof(szFile);
	sfn.lpstrFilter = "ENT XML Files\0*.XML\0\0";
	sfn.nFilterIndex = 1;
	sfn.lpstrFileTitle = NULL;
	sfn.lpstrTitle = title.c_str();
	sfn.nMaxFileTitle = 0;
	sfn.lpstrInitialDir = NULL;
	sfn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// 显示“打开”对话框。

	if (GetSaveFileName(&sfn) == TRUE)
	{
		hf = CreateFile(sfn.lpstrFile,
			GENERIC_WRITE,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (hf == INVALID_HANDLE_VALUE)
		{
			callback->success = false;
		}
		else
		{
			CloseHandle(hf);
			callback->filePath = sfn.lpstrFile;
			callback->success = true;
		}
	}
	else
	{
		callback->success = false;
	}

	callback->complete = true;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD procID = GetCurrentProcessId();
	DWORD windowID;
	if (GetWindowThreadProcessId(hwnd, &windowID))
	{
		if (procID == windowID)
		{
			char class_name[80];
			char title[80];
			GetClassName(hwnd, class_name, sizeof(class_name));
			GetWindowText(hwnd, title, sizeof(title));

			std::ostringstream ss;
			ss << "窗口标题: " << title << " 以及类结构: " << class_name;
			write_text_to_log_file(ss.str());

			if (strcmp(class_name, "DIEmWin") == 0)
			{
				parentWindow = hwnd;
				return FALSE;
			}
		}
	}
	return TRUE;
}