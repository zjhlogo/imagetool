/*!
 * \file StringUtil.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 06/07/2016
 *
 * 
 */
#include "StringUtil.h"
#include "ConvertUTF.h"

NS_BEGIN

int StringUtil::splitString(StringList& arrOut, const tstring& strIn, const tstring& strSplit)
{
	int totalSplit = 0;
	size_t startIndex = 0;
	size_t endIndex = 0;

	endIndex = strIn.find(strSplit, startIndex);
	while (endIndex != tstring::npos)
	{
		arrOut.push_back(strIn.substr(startIndex, endIndex - startIndex));
		++totalSplit;

		startIndex = endIndex;
		startIndex += strSplit.length();
		endIndex = strIn.find(strSplit, startIndex);
	}

	if (totalSplit <= 0)
	{
		arrOut.push_back(strIn);
		++totalSplit;
		return totalSplit;
	}

	arrOut.push_back(strIn.substr(startIndex));
	++totalSplit;

	return totalSplit;
}

int StringUtil::replaceString(tstring& strInOut, const tstring& strSearch, const tstring& strReplace)
{
	StringList arrSubString;
	splitString(arrSubString, strInOut, strSearch);

	int numSubString = (int)arrSubString.size();
	if (numSubString > 1)
	{
		strInOut = joinString<std::vector<tstring>>(arrSubString, strReplace);
		return numSubString;
	}

	return 0;
}

int spank::StringUtil::stringHammingDistance(const tstring& str1, const tstring& str2)
{
	int len1 = str1.length();
	int len2 = str2.length();

	int minLen = len1 < len2 ? len1 : len2;
	int totalDistance = abs(len1 - len2);

	for (int i = 0; i < minLen; ++i)
	{
		if (str1[i] != str2[i])
		{
			++totalDistance;
		}
	}

	return totalDistance;
}

tstring StringUtil::getStringCommonPart(const tstring& str1, const tstring& str2)
{
	int len1 = str1.length();
	int len2 = str2.length();

	int minLen = len1 < len2 ? len1 : len2;
	tstring commonPart;

	for (int i = 0; i < minLen; ++i)
	{
		if (str1[i] == str2[i])
		{
			commonPart.push_back(str1[i]);
		}
	}

	return commonPart;
}

bool StringUtil::convertUtf8ToUtf16(std::vector<uint16>& utf16Out, const tstring& utf8)
{
	const UTF8* sourceStart = (const UTF8*)utf8.data();
	const UTF8* sourceEnd = (const UTF8*)(utf8.data() + utf8.size());

	std::vector<UTF16> target;
	target.resize(utf8.size() + 1);

	UTF16* targetStart = target.data();
	UTF16* targetEnd = target.data() + target.size();

	if (ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion) != conversionOK)
	{
		return false;
	}

	utf16Out.clear();
	for (UTF16* pItem = target.data(); pItem < targetStart; ++pItem)
	{
		utf16Out.push_back(*pItem);
	}

	return true;
}

bool StringUtil::convertUtf16ToUtf8(tstring& utf8Out, const std::vector<uint16>& utf16)
{
	const UTF16* sourceStart = (const UTF16*)utf16.data();
	const UTF16* sourceEnd = (const UTF16*)(utf16.data() + utf16.size());

	std::vector<UTF8> target;
	target.resize((utf16.size() + 1) * 3);

	UTF8* targetStart = target.data();
	UTF8* targetEnd = target.data() + target.size();

	if (ConvertUTF16toUTF8(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion) != conversionOK)
	{
		return false;
	}

	utf8Out.clear();
	for (UTF8* pItem = target.data(); pItem < targetStart; ++pItem)
	{
		utf8Out.push_back(*pItem);
	}

	return true;
}

NS_END
