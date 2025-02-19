/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "utils.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>

#include "debug\debuglog.h"
#include "features/misc.h"

extern "C" IMAGE_DOS_HEADER __ImageBase; // MSVC 专用，其他编译器请使用 DllMain 中的 HMODULE

std::string cachedModulePath;

std::string GetCurrentModulePath()
{
	if (cachedModulePath.empty())
	{
		// 获取模块路径
		char modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));
		GetModuleFileNameA((HMODULE)&__ImageBase, modPath, sizeof(modPath));
		for (size_t i = strlen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				modPath[i] = 0;
				break;
			}
		}
		cachedModulePath = modPath;
	}
	return cachedModulePath;
}

HMODULE GetENTModuleHandle()
{
	HMODULE hMod = NULL;
	GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&GetENTModuleHandle),
		&hMod);

	return hMod;
}

bool does_file_exist(const char* name)
{
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}

bool StringEndsWith(const std::string& a, const std::string& b)
{
	if (b.size() > a.size()) return false;
	return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

bool StringStartsWith(const std::string& a, const std::string& b)
{
	if (b.size() > a.size()) return false;
	return std::equal(a.begin(), a.begin() + b.size(), b.begin());
}

std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
}

//将弧度转换为角度
float degToRad(float degs)
{
	return degs*(float)3.141592653589793 / (float)180.0;
}

//将角度转换为弧度
float radToDeg(float rads)
{
	return rads * ((float)180.0 / (float)3.141592653589793);
}

uintptr_t FindPattern(const char *pattern, const char *mask, const char* startAddress, size_t size)
{
	const char* address_end = startAddress + size;
	const auto mask_length = static_cast<size_t>(strlen(mask) - 1);

	for (size_t i = 0; startAddress < address_end; startAddress++)
	{
		if (*startAddress == pattern[i] || mask[i] == '?')
		{
			if (mask[i + 1] == '\0')
			{
				return reinterpret_cast<uintptr_t>(startAddress) - mask_length;
			}

			i++;
		}
		else
		{
			i = 0;
		}
	}

	return 0;
}

uintptr_t FindPattern(const char *pattern, const char *mask)
{
	MODULEINFO module = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &module, sizeof(MODULEINFO));

	return FindPattern(pattern, mask, reinterpret_cast<const char *>(module.lpBaseOfDll), module.SizeOfImage);
}

bool CompareMemory(const uint8_t* pData, const uint8_t* bMask, const char* sMask)
{
	for (; *sMask; ++sMask, ++pData, ++bMask)
		if (*sMask == 'x' && *pData != *bMask)
			return false;

	return *sMask == NULL;
}

int RegisterFile(const std::string& fullPath, const std::string& fileName)
{
	int textureID = -1;
	std::string path = fullPath.c_str();
	static uint32_t* (*pRegisterFile)(int*, const char*, bool, const char*, bool) = reinterpret_cast<decltype(pRegisterFile)>(FindPatternJACCO("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x7C\x24\x00\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x50\x48\x8B\xEA\x4C\x8B\xFA\x48\x8B\xD9\x4D\x85\xC9", "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxxxxxxxx"));

	if (pRegisterFile(&textureID, fullPath.c_str(), true, fileName.c_str(), false))
	{
		return textureID;
	}

	write_text_to_log_file("注册失败 " + path);
	return 0;
}

bool bittest(int data, unsigned char index)
{
	return (data & (1 << index)) != 0;
}
