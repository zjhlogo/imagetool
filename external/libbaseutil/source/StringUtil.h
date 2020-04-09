/*!
 * \file StringUtil.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 06/07/2016
 *
 *
 */
#pragma once

#include "BaseType.h"

#include <format.h>

#include <sstream>

NS_BEGIN

class StringUtil
{
public:
    static int splitString(StringList& arrOut, const tstring& strIn, const tstring& strSplit);
    static int replaceString(tstring& strInOut, const tstring& strSearch, const tstring& strReplace);
    static int stringHammingDistance(const tstring& str1, const tstring& str2);
    static tstring getStringCommonPart(const tstring& str1, const tstring& str2);

    template <typename CONTAINER, typename TYPE> static int splitString(CONTAINER& arrOut, const tstring& strIn, const tstring& strSplit)
    {
        StringList arrString;
        if (splitString(arrString, strIn, strSplit) <= 0) return 0;

        for (const auto& strValue : arrString)
        {
            std::stringstream ss(strValue);
            TYPE value;
            ss >> value;
            arrOut.push_back(value);
        }

        return arrOut.size();
    }

    template <typename T> static tstring joinString(const T& arrIn, const tstring& strConnect, bool reverse = false)
    {
        tstring strOut;

        int nLength = int(arrIn.size());
        if (nLength <= 0) return strOut;

        if (reverse)
        {
            auto it = arrIn.rbegin();
            strOut += fmt::format("{0}", *it++);

            for (; it != arrIn.rend(); ++it)
            {
                strOut += strConnect;
                strOut += fmt::format("{0}", *it);
            }
        }
        else
        {
            auto it = arrIn.begin();
            strOut += fmt::format("{0}", *it++);

            for (; it != arrIn.end(); ++it)
            {
                strOut += strConnect;
                strOut += fmt::format("{0}", *it);
            }
        }

        return strOut;
    }

    static bool convertUtf16ToUtf8(tstring& utf8Out, const std::vector<uint16>& utf16);
    static bool convertUtf8ToUtf16(std::vector<uint16>& utf16Out, const tstring& utf8);
};

NS_END
