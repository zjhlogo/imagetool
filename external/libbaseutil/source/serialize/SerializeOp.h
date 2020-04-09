/*!
 * \file SerializeOp.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/11/2016
 *
 *
 */
#pragma once

#include "../XmlUtil.h"
#include "SerializeField.h"

#include <typeindex>

NS_BEGIN

class ISerializeField;

class ISerializeOp
{
public:
    ISerializeOp(const std::type_index& classId)
        : m_classId(classId){};
    virtual ~ISerializeOp(){};

    const std::type_index& getClassId() const { return m_classId; };

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) = 0;
    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) = 0;

private:
    std::type_index m_classId;
};

template <typename T> class SerializeOpBuildIn : public ISerializeOp
{
public:
    SerializeOpBuildIn()
        : ISerializeOp(typeid(T)){};
    virtual ~SerializeOpBuildIn(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBuildIn<T>* pField = dynamic_cast<SerializeFieldBuildIn<T>*>(pFieldBase);
        if (!pField) return false;

        T value;
        if (!XmlUtil::getAttribute<T>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBuildIn<T>* pField = dynamic_cast<SerializeFieldBuildIn<T>*>(pFieldBase);
        if (!pField) return false;

        const T& value = pField->m_getter();
        return XmlUtil::setAttribute<T>(value, pXmlElement, pField->m_name.c_str());
    }
};

class SerializeOpString : public ISerializeOp
{
public:
    SerializeOpString()
        : ISerializeOp(typeid(tstring)){};
    virtual ~SerializeOpString(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBuildIn<tstring>* pField = dynamic_cast<SerializeFieldBuildIn<tstring>*>(pFieldBase);
        if (!pField) return false;

        tstring value;
        if (!XmlUtil::getAttribute(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBuildIn<tstring>* pField = dynamic_cast<SerializeFieldBuildIn<tstring>*>(pFieldBase);
        if (!pField) return false;

        const tstring& value = pField->m_getter();
        return XmlUtil::setAttribute<tstring>(value, pXmlElement, pField->m_name.c_str());
    }
};

class SerializeOpBoolean : public ISerializeOp
{
public:
    SerializeOpBoolean()
        : ISerializeOp(typeid(bool)){};
    virtual ~SerializeOpBoolean(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBoolean* pField = dynamic_cast<SerializeFieldBoolean*>(pFieldBase);
        if (!pField) return false;

        int value = 0;
        if (!XmlUtil::getAttribute<int>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter((value != 0));
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBoolean* pField = dynamic_cast<SerializeFieldBoolean*>(pFieldBase);
        if (!pField) return false;

        bool value = pField->m_getter();
        return XmlUtil::setAttribute<int>((int)value, pXmlElement, pField->m_name.c_str());
    }
};

template <typename CONTAINER, typename TYPE> class SerializeOpVec2 : public ISerializeOp
{
public:
    SerializeOpVec2()
        : ISerializeOp(typeid(CONTAINER)){};
    virtual ~SerializeOpVec2(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        CONTAINER value;
        if (!XmlUtil::getAttributeVector2<CONTAINER, TYPE>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        const CONTAINER& value = pField->m_getter();
        return XmlUtil::setAttributeVector2<CONTAINER>(value, pXmlElement, pField->m_name.c_str());
    }
};

template <typename CONTAINER, typename TYPE> class SerializeOpVec3 : public ISerializeOp
{
public:
    SerializeOpVec3()
        : ISerializeOp(typeid(CONTAINER)){};
    virtual ~SerializeOpVec3(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        CONTAINER value;
        if (!XmlUtil::getAttributeVector3<CONTAINER, TYPE>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        const CONTAINER& value = pField->m_getter();
        return XmlUtil::setAttributeVector3<CONTAINER>(value, pXmlElement, pField->m_name.c_str());
    }
};

template <typename CONTAINER, typename TYPE> class SerializeOpVec4 : public ISerializeOp
{
public:
    SerializeOpVec4()
        : ISerializeOp(typeid(CONTAINER)){};
    virtual ~SerializeOpVec4(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        CONTAINER value;
        if (!XmlUtil::getAttributeVector4<CONTAINER, TYPE>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldBuildIn<CONTAINER>* pField = dynamic_cast<SerializeFieldBuildIn<CONTAINER>*>(pFieldBase);
        if (!pField) return false;

        const CONTAINER& value = pField->m_getter();
        return XmlUtil::setAttributeVector4<CONTAINER>(value, pXmlElement, pField->m_name.c_str());
    }
};

template <typename T> class SerializeOpEnum : public ISerializeOp
{
public:
    SerializeOpEnum()
        : ISerializeOp(typeid(T)){};
    virtual ~SerializeOpEnum(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldEnum<T>* pField = dynamic_cast<SerializeFieldEnum<T>*>(pFieldBase);
        if (!pField) return false;

        int value;
        if (!XmlUtil::getAttribute<int>(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter((T)value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldEnum<T>* pField = dynamic_cast<SerializeFieldEnum<T>*>(pFieldBase);
        if (!pField) return false;

        T value = pField->m_getter();
        return XmlUtil::setAttribute<int>((int)value, pXmlElement, pField->m_name.c_str());
    }
};

class SerializeOpFlag : public ISerializeOp
{
public:
    SerializeOpFlag()
        : ISerializeOp(typeid(SerializeFieldFlag)){};
    virtual ~SerializeOpFlag(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldFlag* pField = dynamic_cast<SerializeFieldFlag*>(pFieldBase);
        if (!pField) return false;

        uint value;
        if (!XmlUtil::getAttribute(value, pXmlElement, pField->m_name.c_str())) return false;

        return pField->m_setter(value);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldFlag* pField = dynamic_cast<SerializeFieldFlag*>(pFieldBase);
        if (!pField) return false;

        uint value = pField->m_getter();
        return XmlUtil::setAttribute(value, pXmlElement, pField->m_name.c_str());
    }
};

template <typename T> class SerializeOpObject : public ISerializeOp
{
public:
    SerializeOpObject()
        : ISerializeOp(typeid(T)){};
    virtual ~SerializeOpObject(){};

    virtual bool fromXml(tinyxml2::XMLElement* pXmlElement, ISerializeField* pFieldBase) override
    {
        SerializeFieldObject<T>* pField = dynamic_cast<SerializeFieldObject<T>*>(pFieldBase);
        if (!pField) return false;

        return pField->fromXml(pXmlElement);
    }

    virtual bool toXml(ISerializeField* pFieldBase, tinyxml2::XMLElement* pXmlElement) override
    {
        SerializeFieldObject<T>* pField = dynamic_cast<SerializeFieldObject<T>*>(pFieldBase);
        if (!pField) return false;

        return pField->toXml(pXmlElement);
    }
};

NS_END
