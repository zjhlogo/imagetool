/*!
 * \file XmlUtil.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 05/13/2016
 *
 *
 */
#pragma once

#include "BaseType.h"
#include "StringUtil.h"

#include <tinyxml2.h>

#include <functional>
#include <sstream>

NS_BEGIN

class XmlUtil
{
public:
    template <typename T> static bool getAttribute(T& valueOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        std::stringstream ss(pszValue);
        ss >> valueOut;

        return true;
    };

    static bool getAttribute(tstring& valueOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        valueOut = pszValue;

        return true;
    };

    template <typename T> static bool getAttributeEnum(T& valueOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        int nValue = 0;
        std::stringstream ss(pszValue);
        ss >> nValue;

        valueOut = (T)nValue;
        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getAttributeVector2(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, pszValue, ",") != 2) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getAttributeVector3(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, pszValue, ",") != 3) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];
        vecOut.z = arrValue[2];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getAttributeVector4(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, pszValue, ",") != 4) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];
        vecOut.z = arrValue[2];
        vecOut.w = arrValue[3];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static int getAttributeArray(CONTAINER& valueOut, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        const char* pszValue = xmlElement->Attribute(attrName);
        if (!pszValue) return false;

        return StringUtil::splitString<CONTAINER, TYPE>(valueOut, pszValue, ",");
    };

    template <typename T> static bool setAttribute(const T& value, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        std::stringstream ss;
        ss << value;

        xmlElement->SetAttribute(attrName, ss.str().c_str());

        return true;
    };

    template <typename T> static bool setAttributeEnum(const T& value, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        int nValue = (int)value;
        std::stringstream ss;
        ss << nValue;

        xmlElement->SetAttribute(attrName, ss.str().c_str());

        return true;
    };

    template <typename T> static bool setAttributeVector2(const T& v, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        tstring strValue = fmt::format("{0},{1}", v.x, v.y);
        xmlElement->SetAttribute(attrName, strValue.c_str());

        return true;
    };

    template <typename T> static bool setAttributeVector3(const T& v, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        tstring strValue = fmt::format("{0},{1},{2}", v.x, v.y, v.z);
        xmlElement->SetAttribute(attrName, strValue.c_str());

        return true;
    };

    template <typename T> static bool setAttributeVector4(const T& v, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        tstring strValue = fmt::format("{0},{1},{2},{3}", v.x, v.y, v.z, v.w);
        xmlElement->SetAttribute(attrName, strValue.c_str());

        return true;
    };

    template <typename T> static bool setAttributeArray(const std::vector<T>& value, tinyxml2::XMLElement* xmlElement, const char* attrName)
    {
        if (!xmlElement || !attrName) return false;

        tstring strValue = StringUtil::joinString<std::vector<T>>(value, ",");
        xmlElement->SetAttribute(attrName, strValue.c_str());

        return true;
    };

    template <typename T> static bool getElement(T& valueOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLElement* xmlSubElement = xmlElement->FirstChildElement(elementName);
        if (!xmlSubElement) return false;

        const char* pszValue = xmlSubElement->GetText();
        if (!pszValue) return false;

        std::stringstream ss(pszValue);
        ss >> valueOut;

        return true;
    };

    template <typename T> static bool getElementEnum(T& valueOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLElement* xmlSubElement = xmlElement->FirstChildElement(elementName);
        if (!xmlSubElement) return false;

        const char* pszValue = xmlSubElement->GetText();
        if (!pszValue) return false;

        int nValue = 0;
        std::stringstream ss(pszValue);
        ss >> nValue;

        valueOut = (T)nValue;

        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getElementVector2(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        tstring strValue;
        if (!getElement<tstring>(strValue, xmlElement, elementName)) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, strValue, ",") != 2) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getElementVector3(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        tstring strValue;
        if (!getElement<tstring>(strValue, xmlElement, elementName)) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, strValue, ",") != 3) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];
        vecOut.z = arrValue[2];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static bool getElementVector4(CONTAINER& vecOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        tstring strValue;
        if (!getElement<tstring>(strValue, xmlElement, elementName)) return false;

        std::vector<TYPE> arrValue;
        if (StringUtil::splitString<std::vector<TYPE>, TYPE>(arrValue, strValue, ",") != 4) return false;

        vecOut.x = arrValue[0];
        vecOut.y = arrValue[1];
        vecOut.z = arrValue[2];
        vecOut.w = arrValue[3];

        return true;
    };

    template <typename CONTAINER, typename TYPE> static int getElementArray(CONTAINER& valueOut, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        tstring strValue;
        if (!getElement<tstring>(strValue, xmlElement, elementName)) return false;

        return StringUtil::splitString<CONTAINER, TYPE>(valueOut, strValue, ",");
    };

    template <typename T> static bool setElement(const T& value, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        std::stringstream ss;
        ss << value;

        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(ss.str().c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    template <typename T> static bool setElementEnum(const T& value, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        int nValue = (int)value;
        std::stringstream ss;
        ss << nValue;

        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(ss.str().c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    template <typename T> static bool setElementVector2(const T& v, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        tstring strValue = fmt::format("{0},{1}", v.x, v.y);
        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(strValue.c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    template <typename T> static bool setElementVector3(const T& v, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        tstring strValue = fmt::format("{0},{1},{2}", v.x, v.y, v.z);
        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(strValue.c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    template <typename T> static bool setElementVector4(const T& v, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        tstring strValue = fmt::format("{0},{1},{2},{3}", v.x, v.y, v.z, v.w);
        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(strValue.c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    template <typename T> static bool setElementArray(const std::vector<T>& v, tinyxml2::XMLElement* xmlElement, const char* elementName)
    {
        if (!xmlElement || !elementName) return false;

        tinyxml2::XMLDocument* pXmlDoc = xmlElement->GetDocument();
        if (!pXmlDoc) return false;

        tinyxml2::XMLElement* pXmlSubElement = pXmlDoc->NewElement(elementName);
        xmlElement->LinkEndChild(pXmlSubElement);

        tstring strValue = StringUtil::joinString<T>(v, ",");
        tinyxml2::XMLText* pXmlText = pXmlDoc->NewText(strValue.c_str());
        pXmlSubElement->LinkEndChild(pXmlText);

        return true;
    };

    static bool loadXmlFile(tinyxml2::XMLDocument& doc, const tstring& filePath);
    static bool saveXmlFile(tinyxml2::XMLDocument& doc, const tstring& filePath);
};

NS_END
