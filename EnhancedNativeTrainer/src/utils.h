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
#include <stdio.h>
#include <windows.h>

// 返回模块加载路径，并带有尾部斜杠
std::string GetCurrentModulePath();

HMODULE GetENTModuleHandle();

bool does_file_exist(const char* name);

bool StringEndsWith(const std::string& a, const std::string& b);

bool StringStartsWith(const std::string& a, const std::string& b);

std::wstring ConvertFromUtf8ToUtf16(const std::string& str);

float degToRad(float degs);

float radToDeg(float rads);

uintptr_t FindPattern(const char *pattern, const char *mask, const char* startAddress, size_t size);

uintptr_t FindPattern(const char *pattern, const char *mask);

bool CompareMemory(const uint8_t* pData, const uint8_t* bMask, const char* sMask);

int RegisterFile(const std::string& fullPath, const std::string& fileName);

bool bittest(int data, unsigned char index);