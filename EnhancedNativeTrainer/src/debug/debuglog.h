/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include <string>

#pragma once

#ifdef _DEBUG
const bool DEBUG_LOG_ENABLED = true;
#endif
#ifndef _DEBUG
const bool DEBUG_LOG_ENABLED = false;
#endif

/**向日志文件追加一行文本。除非调试常量已设置，否则不执行任何操作*/
void write_text_to_log_file(const std::string &text);

/**清空调试日志文件。每次训练器启动时调用*/
void clear_log_file();
