/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "xml_import_export.h"
#include "..\version.h"
#include "..\features\script.h"

#include "..\debug\debuglog.h"

// 一个全局的 Windows "基本字符串"。实际内存是由 MSXML 使用的 COM 方法分配的，
// 这些方法接收 &keyconf_bstr。我们必须在后续使用之前调用 SysFreeString() 
// 来释放这块内存，以防止内存泄漏。
BSTR xmlParser_bstr;

bool generate_xml_for_propset(SavedPropSet* props, std::string outputFile)
{
	// 创建 XML
	IXMLDOMDocumentPtr pXMLDoc;
	HRESULT hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		write_text_to_log_file("创建 XML 类的实例失败！");
		return false;
	}

	VARIANT_BOOL bIsSuccessful;
	if (FAILED(pXMLDoc->loadXML(L"<object-set></object-set>", &bIsSuccessful)))
	{
		write_text_to_log_file("根节点创建失败！");
		handle_error(pXMLDoc);
		return false;
	}

	IXMLDOMProcessingInstructionPtr pXMLProcessingNode;
	pXMLDoc->createProcessingInstruction(L"xml", L" version=\"1.0\" encoding=\"UTF-8\"", &pXMLProcessingNode);

	//获取刚刚创建的根元素    
	IXMLDOMElementPtr pXMLRootElem;
	pXMLDoc->get_documentElement(&pXMLRootElem);

	_variant_t vtObject;
	vtObject.vt = VT_DISPATCH;
	vtObject.pdispVal = pXMLRootElem;
	vtObject.pdispVal->AddRef();
	pXMLDoc->insertBefore(pXMLProcessingNode, vtObject, 0);

	//添加一个属性
	pXMLRootElem->setAttribute(L"set-name", _variant_t(props->saveName.c_str()));
	pXMLRootElem->setAttribute(L"ent-version", _variant_t(VERSION_STRING.c_str()));

	for each (SavedPropDBRow* row in props->items)
	{
		//创建子元素
		IXMLDOMElementPtr objectNode;
		pXMLDoc->createElement(L"object", &objectNode);

		objectNode->setAttribute(L"title", _variant_t(row->title.c_str()));
		objectNode->setAttribute(L"model", _variant_t((long)(row->model)));

		objectNode->setAttribute(L"posX", _variant_t(row->posX));
		objectNode->setAttribute(L"posY", _variant_t(row->posY));
		objectNode->setAttribute(L"posZ", _variant_t(row->posZ));

		objectNode->setAttribute(L"roll", _variant_t(row->roll));
		objectNode->setAttribute(L"pitch", _variant_t(row->pitch));
		objectNode->setAttribute(L"yaw", _variant_t(row->yaw));

		objectNode->setAttribute(L"isImmovable", _variant_t(row->isImmovable));
		objectNode->setAttribute(L"isInvincible", _variant_t(row->isInvincible));
		objectNode->setAttribute(L"hasGravity", _variant_t(row->hasGravity));

		objectNode->setAttribute(L"alpha", _variant_t(row->alpha));
		objectNode->setAttribute(L"counter", _variant_t(row->counter));

		pXMLRootElem->appendChild(objectNode, 0);
	}

	FileStream* output;
	std::wstring ws;
	ws.assign(outputFile.begin(), outputFile.end());
	BSTR bs = SysAllocStringLen(ws.data(), ws.size());

	bool result = true;
	if (FAILED(FileStream::OpenFile(bs, &output, true)))
	{
		write_text_to_log_file("打开输出失败了！");
		result = false;
	}
	else
	{
		if (!format_dom_document(pXMLDoc, output))
		{
			write_text_to_log_file("保存失败！");
			write_text_to_log_file(outputFile);
			handle_error(pXMLDoc);
			result = false;
		}
		else
		{
			write_text_to_log_file("保存完成！");
			write_text_to_log_file(outputFile);
			result = true;
		}

		int count = 0;
		do
		{
			count = output->Release();
		} while (count > 0);

		if (count == 0)
		{
			write_text_to_log_file("文件已关闭，计数为零。");
		}
		else
		{
			write_text_to_log_file("文件已关闭，计数为非零。");
		}
	}
	return result;
}

bool parse_xml_for_propset(std::string inputFile, SavedPropSet* set)
{
	CoInitialize(NULL);

	//读取 XML
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (!spXMLDoc->load(inputFile.c_str()))
	{
		write_text_to_log_file("未能找到 XML 文件！");
		return false;
	}

	IXMLDOMNodeListPtr topNodes = spXMLDoc->selectNodes(L"//object-set");
	{
		IXMLDOMNode *node;
		topNodes->get_item(0, &node);

		IXMLDOMNamedNodeMap *attribs;
		node->get_attributes(&attribs);
		long length_attribs;
		attribs->get_length(&length_attribs);

		for (long j = 0; j < length_attribs; j++)
		{
			IXMLDOMNode *attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&xmlParser_bstr);
			if (wcscmp(xmlParser_bstr, L"set-name") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				set->saveName = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}

			SysFreeString(xmlParser_bstr);
			attribNode->Release();
		}

		attribs->Release();
		node->Release();
	}

	IXMLDOMNodeListPtr nodes = spXMLDoc->selectNodes(L"//object-set/object");
	long length;
	nodes->get_length(&length);
	for (int i = 0; i < length; i++)
	{
		WAIT(0);
		make_periodic_feature_call();

		IXMLDOMNode *node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap *attribs;
		node->get_attributes(&attribs);

		SavedPropDBRow* row = new SavedPropDBRow();

		long length_attribs;
		attribs->get_length(&length_attribs);

		for (long j = 0; j < length_attribs; j++)
		{
			IXMLDOMNode *attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&xmlParser_bstr);
			if (wcscmp(xmlParser_bstr, L"title") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				row->title = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if (wcscmp(xmlParser_bstr, L"model") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_I8);
				row->model = var.intVal;
			}
			else if (wcscmp(xmlParser_bstr, L"posX") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->posX = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"posY") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->posY = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"posZ") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->posZ = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"roll") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->roll = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"pitch") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->pitch = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"yaw") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_R4);
				row->yaw = var.fltVal;
			}
			else if (wcscmp(xmlParser_bstr, L"isImmovable") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_INT);
				row->isImmovable = var.intVal;
			}
			else if (wcscmp(xmlParser_bstr, L"isInvincible") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_INT);
				row->isInvincible = var.intVal;
			}
			else if (wcscmp(xmlParser_bstr, L"hasGravity") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_INT);
				row->hasGravity = var.intVal;
			}
			else if (wcscmp(xmlParser_bstr, L"alpha") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				VariantChangeType(&var, &var, 0, VT_INT);
				row->alpha = var.intVal;
			}

			SysFreeString(xmlParser_bstr);
			attribNode->Release();
		}

		row->counter = 0;
		set->items.push_back(row);

		attribs->Release();
		node->Release();
	}

	set->dbSize = set->items.size();

	//nodes->Release(); //不要执行此操作，它会在退出时崩溃
	spXMLDoc.Release();
	CoUninitialize();

	return true;
}

void handle_error(IXMLDOMDocumentPtr doc)
{
	std::ostringstream ss;
	IXMLDOMParseError* pError;
	ss << "XML 错误: ";
	doc->get_parseError(&pError);
	if (pError)
	{
		BSTR reasonStr;
		pError->get_reason(&reasonStr);
		ss << reasonStr;
	}
	else
	{
		ss << "未知";
	}
	write_text_to_log_file(ss.str());
}

bool format_dom_document(IXMLDOMDocument *pDoc, IStream *pStream)
{
	// 创建写入器
	MSXML2::IMXWriterPtr pMXWriter;
	
	if (FAILED(pMXWriter.CreateInstance(__uuidof(MSXML2::MXXMLWriter60))))
	{
		return false;
	}
	MSXML2::ISAXContentHandler* pISAXContentHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXContentHandler)))
	{
		return false;
	}
	MSXML2::ISAXErrorHandler* pISAXErrorHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXErrorHandler)))
	{
		return false;
	}
	MSXML2::ISAXDTDHandler* pISAXDTDHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXDTDHandler)))
	{
		return false;
	}

	if (FAILED(pMXWriter->put_omitXMLDeclaration(VARIANT_FALSE)) ||
		FAILED(pMXWriter->put_standalone(VARIANT_TRUE)) ||
		FAILED(pMXWriter->put_indent(VARIANT_TRUE)) ||
		FAILED(pMXWriter->put_encoding(L"UTF-8")))
	{
		return false;
	}

	// 创建 SAX 读取器
	MSXML2::ISAXXMLReaderPtr pSAXReader;
	if (FAILED(pSAXReader.CreateInstance(__uuidof (MSXML2::SAXXMLReader60))))
	{
		return false;
	}

	wchar_t lexical_handler[] = L"http://xml.org/sax/properties/lexical-handler";
	wchar_t declaration_handler[] = L"http://xml.org/sax/properties/declaration-handler";

	if (FAILED(pSAXReader->putContentHandler(pISAXContentHandler)) ||
		FAILED(pSAXReader->putDTDHandler(pISAXDTDHandler)) ||
		FAILED(pSAXReader->putErrorHandler(pISAXErrorHandler)) ||
		FAILED(pSAXReader->putProperty(reinterpret_cast<unsigned short*>(lexical_handler), _variant_t(pMXWriter.GetInterfacePtr()))) ||
		FAILED(pSAXReader->putProperty(reinterpret_cast<unsigned short*>(declaration_handler), _variant_t(pMXWriter.GetInterfacePtr()))))
	{
		return false;
	}

	// 执行写入
	bool success1 = SUCCEEDED(pMXWriter->put_output(_variant_t(pStream)));
	bool success2 = SUCCEEDED(pSAXReader->parse(pDoc));

	return success1 && success2;
}