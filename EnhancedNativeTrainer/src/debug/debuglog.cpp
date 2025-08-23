/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include <fstream>
#include <sstream>
#include <time.h>
#include <iostream>
#include <direct.h>
#include <sys/stat.h>

#include "debuglog.h"

#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21
#define LOG_FILE_NAME "ent-log.txt"

void clear_log_file(){
	remove(LOG_FILE_NAME);
}

static char *getDtTm(char *buff){
	time_t t = time(0);
	strftime(buff, DTTMSZ, DTTMFMT, localtime(&t));
	return buff;
}

void write_text_to_log_file(const std::string &text){
	//if(!DEBUG_LOG_ENABLED){
	//	返回;
	//}

	// 确保目录存在
	struct stat info;
	if (stat("Enhanced Native Trainer", &info) != 0) {
		_mkdir("Enhanced Native Trainer");
	}
	if (stat("Enhanced Native Trainer/Logs", &info) != 0) {
		_mkdir("Enhanced Native Trainer/Logs");
	}

	char tbuff[DTTMSZ];
	std::ofstream log_file("Enhanced Native Trainer/Logs/ent - log.txt", std::ios_base::out | std::ios_base::app);
	log_file << getDtTm(tbuff) << text << std::endl;
	log_file.close();
}
