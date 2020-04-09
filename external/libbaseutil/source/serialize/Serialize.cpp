/*!
 * \file Serialize.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/08/2016
 *
 *
 */
#include "Serialize.h"
#include "SerializeOp.h"

NS_BEGIN

bool Serialize::m_initialized = false;
Serialize::TM_SERIALIZE_OPERATOR Serialize::m_serializeOpMap;

Serialize::Serialize()
{
}

Serialize::~Serialize()
{
    for (auto& pField : m_serializeFieldList)
    {
        SAFE_DELETE(pField);
    }
    m_serializeFieldList.clear();
}

bool Serialize::fromXml(tinyxml2::XMLElement* pXmlElement)
{
    for (auto& pField : m_serializeFieldList)
    {
        pField->m_serOperator->fromXml(pXmlElement, pField);
    }

    return true;
}

bool Serialize::toXml(tinyxml2::XMLElement* pXmlElement)
{
    for (auto& pField : m_serializeFieldList)
    {
        if (!pField->m_serOperator->toXml(pField, pXmlElement)) return false;
    }

    return true;
}

bool Serialize::fromXmlFinalize(const tstring& dir)
{
    if (m_cbFromXmlFinalize) return m_cbFromXmlFinalize(dir);
    return true;
}

bool Serialize::toXmlFinalize(const tstring& dir)
{
    if (m_cbToXmlFinalize) return m_cbToXmlFinalize(dir);
    return true;
}

bool Serialize::addSerializer(ISerializeOp* serOp)
{
    if (m_serializeOpMap.find(serOp->getClassId()) != m_serializeOpMap.end())
    {
        SAFE_DELETE(serOp);
        return false;
    }

    m_serializeOpMap.insert(std::make_pair(serOp->getClassId(), serOp));
    return true;
}

bool Serialize::initialize()
{
    if (m_initialized) return true;

    addSerializer(new SerializeOpBuildIn<int8>());
    addSerializer(new SerializeOpBuildIn<uint8>());
    addSerializer(new SerializeOpBuildIn<int16>());
    addSerializer(new SerializeOpBuildIn<uint16>());
    addSerializer(new SerializeOpBuildIn<int32>());
    addSerializer(new SerializeOpBuildIn<uint32>());
    addSerializer(new SerializeOpBuildIn<int64>());
    addSerializer(new SerializeOpBuildIn<uint64>());
    addSerializer(new SerializeOpBuildIn<float>());
    addSerializer(new SerializeOpBuildIn<double>());
    addSerializer(new SerializeOpString());
    addSerializer(new SerializeOpBoolean());
    addSerializer(new SerializeOpFlag());
    addSerializer(new SerializeOpVec2<glm::vec2, float>());
    addSerializer(new SerializeOpVec2<glm::ivec2, int>());
    addSerializer(new SerializeOpVec3<glm::vec3, float>());
    addSerializer(new SerializeOpVec3<glm::ivec3, int>());
    addSerializer(new SerializeOpVec4<glm::vec4, float>());
    addSerializer(new SerializeOpVec4<glm::ivec4, int>());
    addSerializer(new SerializeOpVec4<glm::quat, float>());

    m_initialized = true;
    return true;
}

void Serialize::terminate()
{
    for (auto it : m_serializeOpMap)
    {
        SAFE_DELETE(it.second);
    }
    m_serializeOpMap.clear();

    m_initialized = false;
}

NS_END
