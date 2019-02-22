/*!
 * \file XmlUtil.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 05/13/2016
 *
 * 
 */
#include "XmlUtil.h"
#include "File.h"
#include "FileUtil.h"
#include "LogUtil.h"

NS_BEGIN

bool XmlUtil::loadXmlFile(tinyxml2::XMLDocument& doc, const tstring& filePath)
{
	tstring strXmlData;
	if (!FileUtil::readFileIntoString(strXmlData, filePath))
	{
		LOGE(("load file failed, {0}"), filePath);
		return false;
	}

	if (doc.Parse(strXmlData.c_str()) != tinyxml2::XML_SUCCESS)
	{
		LOGE(("parse xml file failed, file={0}, error={1}"), filePath, doc.ErrorID());
		return false;
	}

	return true;
}

bool XmlUtil::saveXmlFile(tinyxml2::XMLDocument& doc, const tstring& filePath)
{
	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);

	File file;
	if (!file.open(filePath, File::AM_WRITE)) return false;

	file.write(printer.CStr(), printer.CStrSize() - 1);
	file.close();

	return true;
}

NS_END
