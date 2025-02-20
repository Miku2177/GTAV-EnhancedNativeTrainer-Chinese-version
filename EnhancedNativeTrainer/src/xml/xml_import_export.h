/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include <string>
#include <sstream>
#include <iostream>

#include "filestream.hpp"
#include "../storage/database.h"

#import <msxml6.dll> //请阅读 GitHub 项目中的 README 文件，了解如何使其正常运行

bool generate_xml_for_propset(SavedPropSet* props, std::string outputFile);

bool parse_xml_for_propset(std::string inputFile, SavedPropSet* set);

void handle_error(IXMLDOMDocumentPtr doc);

bool format_dom_document(IXMLDOMDocument *pDoc, IStream *pStream);