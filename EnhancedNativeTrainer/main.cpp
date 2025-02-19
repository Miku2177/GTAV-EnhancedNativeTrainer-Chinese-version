/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "inc\main.h"
#include "src\features\script.h"
#include "src\io\keyboard.h"
#include "src\io\config_io.h"
#include "src\debug\debuglog.h"
#include "src\utils.h"

#include <thread>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved){
	switch(reason){
		case DLL_PROCESS_ATTACH:
			scriptRegister(hInstance, ScriptMain);
			keyboardHandlerRegister(OnKeyboardMessage);
			break;
		case DLL_PROCESS_DETACH:
			scriptUnregister(ScriptMain);
			keyboardHandlerUnregister(OnKeyboardMessage);
			ScriptTidyUp();
			break;
	}
	return TRUE;
}
