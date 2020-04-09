/*!
 * \file Serialize.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/08/2016
 *
 *
 */
#pragma once

#include "../LogUtil.h"
#include "SerializeField.h"
#include "SerializeOp.h"

#include <map>

NS_BEGIN

class Serialize
{
public:
    typedef std::map<std::type_index, ISerializeOp*> TM_SERIALIZE_OPERATOR;
    typedef std::vector<ISerializeField*> TV_SERIALIZE_FIELD;

    typedef std::function<bool(const tstring& dir)> CALLBACK_BOOL_TSTRING;

public:
    Serialize();
    ~Serialize();

    bool fromXml(tinyxml2::XMLElement* pXmlElement);
    bool toXml(tinyxml2::XMLElement* pXmlElement);

    void setFromXmlFinalizeCallback(const CALLBACK_BOOL_TSTRING& cb) { m_cbFromXmlFinalize = cb; };
    void setToXmlFinalizeCallback(const CALLBACK_BOOL_TSTRING& cb) { m_cbToXmlFinalize = cb; };

    bool fromXmlFinalize(const tstring& dir);
    bool toXmlFinalize(const tstring& dir);

    template <typename T>
    bool addFieldBuildIn(const tstring& name,
                         const std::function<bool(const T& value)>& setter,
                         const std::function<const T&()>& getter,
                         const IFieldAttribute* attr = nullptr)
    {
        std::type_index classId(typeid(T));

        TM_SERIALIZE_OPERATOR::iterator itSerializer = m_serializeOpMap.find(classId);
        if (itSerializer == m_serializeOpMap.end())
        {
            LOGE("can not find serializer for type {0}", classId.name());
            return false;
        }

        m_serializeFieldList.push_back(new SerializeFieldBuildIn<T>(itSerializer->second, name, setter, getter, attr));
        return true;
    }

    bool addFieldBoolean(const tstring& name,
                         const std::function<bool(bool value)>& setter,
                         const std::function<bool()>& getter,
                         const IFieldAttribute* attr = nullptr)
    {
        std::type_index classId(typeid(bool));

        TM_SERIALIZE_OPERATOR::iterator itSerializer = m_serializeOpMap.find(classId);
        if (itSerializer == m_serializeOpMap.end())
        {
            LOGE("can not find serializer for type {0}", classId.name());
            return false;
        }

        m_serializeFieldList.push_back(new SerializeFieldBoolean(itSerializer->second, name, setter, getter, attr));
        return true;
    }

    template <typename T>
    bool addFieldEnum(const tstring& name, const std::function<bool(T value)>& setter, const std::function<T()>& getter, const IFieldAttribute* attr = nullptr)
    {
        std::type_index classId(typeid(T));

        TM_SERIALIZE_OPERATOR::iterator itSerializer = m_serializeOpMap.find(classId);
        if (itSerializer == m_serializeOpMap.end())
        {
            addSerializer(new SerializeOpEnum<T>());

            itSerializer = m_serializeOpMap.find(classId);
            if (itSerializer == m_serializeOpMap.end())
            {
                LOGE("can not find serializer for enum {0}", classId.name());
                return false;
            }
        }

        m_serializeFieldList.push_back(new SerializeFieldEnum<T>(itSerializer->second, name, setter, getter, attr));
        return true;
    }

    bool addFieldFlag(const tstring& name,
                      const std::function<bool(uint value)>& setter,
                      const std::function<bool(uint value)>& clear,
                      const std::function<uint()>& getter,
                      const IFieldAttribute* attr = nullptr)
    {
        std::type_index classId(typeid(SerializeFieldFlag));

        TM_SERIALIZE_OPERATOR::iterator itSerializer = m_serializeOpMap.find(classId);
        if (itSerializer == m_serializeOpMap.end()) return false;

        m_serializeFieldList.push_back(new SerializeFieldFlag(itSerializer->second, name, setter, clear, getter, attr));
        return true;
    }

    template <typename T> bool addFieldObject(const tstring& name, T* pObj, const IFieldAttribute* attr = nullptr)
    {
        std::type_index classId(typeid(T));

        TM_SERIALIZE_OPERATOR::iterator itSerializer = m_serializeOpMap.find(classId);
        if (itSerializer == m_serializeOpMap.end())
        {
            addSerializer(new SerializeOpObject<T>());

            itSerializer = m_serializeOpMap.find(classId);
            if (itSerializer == m_serializeOpMap.end())
            {
                LOGE("can not find serializer for object {0}", classId.name());
                return false;
            }
        }

        m_serializeFieldList.push_back(new SerializeFieldObject<T>(itSerializer->second, name, pObj, attr));
        return true;
    }

    const TV_SERIALIZE_FIELD& getSerializeFieldList() const { return m_serializeFieldList; };

    static bool addSerializer(ISerializeOp* serOp);
    static bool initialize();
    static void terminate();

private:
    TV_SERIALIZE_FIELD m_serializeFieldList;
    CALLBACK_BOOL_TSTRING m_cbFromXmlFinalize;
    CALLBACK_BOOL_TSTRING m_cbToXmlFinalize;

    static bool m_initialized;
    static TM_SERIALIZE_OPERATOR m_serializeOpMap;
};

NS_END
