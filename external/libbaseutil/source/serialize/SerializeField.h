/*!
 * \file SerializeField.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/11/2016
 *
 *
 */
#pragma once

#include "FieldAttributes.h"

#include <functional>
#include <typeindex>

NS_BEGIN

class ISerializeOp;

class ISerializeField
{
public:
    ISerializeField(ISerializeOp* serOp, const std::type_index& type, const tstring& n, const IFieldAttribute* attr)
        : m_serOperator(serOp)
        , m_typeIndex(type)
        , m_name(n)
        , m_fieldAttribute(attr){};
    virtual ~ISerializeField(){};

public:
    ISerializeOp* m_serOperator{};
    std::type_index m_typeIndex;
    tstring m_name;
    const IFieldAttribute* m_fieldAttribute{};
};

template <typename T> class SerializeFieldBuildIn : public ISerializeField
{
public:
    typedef std::function<bool(const T& value)> CallbackSetter;
    typedef std::function<const T&()> CallbackGetter;

public:
    SerializeFieldBuildIn(ISerializeOp* ser, const tstring& n, const CallbackSetter& s, const CallbackGetter& g, const IFieldAttribute* attr)
        : ISerializeField(ser, typeid(T), n, attr)
        , m_setter(s)
        , m_getter(g){};
    virtual ~SerializeFieldBuildIn(){};

public:
    CallbackSetter m_setter;
    CallbackGetter m_getter;
};

class SerializeFieldBoolean : public ISerializeField
{
public:
    typedef std::function<bool(bool value)> CallbackSetter;
    typedef std::function<bool()> CallbackGetter;

public:
    SerializeFieldBoolean(ISerializeOp* ser, const tstring& n, const CallbackSetter& s, const CallbackGetter& g, const IFieldAttribute* attr)
        : ISerializeField(ser, typeid(bool), n, attr)
        , m_setter(s)
        , m_getter(g){};
    virtual ~SerializeFieldBoolean(){};

public:
    CallbackSetter m_setter;
    CallbackGetter m_getter;
};

template <typename T> class SerializeFieldEnum : public ISerializeField
{
public:
    typedef std::function<bool(T value)> CallbackSetter;
    typedef std::function<T()> CallbackGetter;

public:
    SerializeFieldEnum(ISerializeOp* ser, const tstring& n, const CallbackSetter& s, const CallbackGetter& g, const IFieldAttribute* attr)
        : ISerializeField(ser, typeid(T), n, attr)
        , m_setter(s)
        , m_getter(g){};
    virtual ~SerializeFieldEnum(){};

public:
    CallbackSetter m_setter;
    CallbackGetter m_getter;
};

class SerializeFieldFlag : public ISerializeField
{
public:
    typedef std::function<bool(uint value)> CallbackSetter;
    typedef std::function<uint()> CallbackGetter;

public:
    SerializeFieldFlag(ISerializeOp* ser,
                       const tstring& n,
                       const CallbackSetter& s,
                       const CallbackSetter& c,
                       const CallbackGetter& g,
                       const IFieldAttribute* attr)
        : ISerializeField(ser, typeid(SerializeFieldFlag), n, attr)
        , m_setter(s)
        , m_cleaner(c)
        , m_getter(g){};
    virtual ~SerializeFieldFlag(){};

public:
    CallbackSetter m_setter;
    CallbackGetter m_getter;
    CallbackSetter m_cleaner;
};

template <typename T> class SerializeFieldObject : public ISerializeField
{
public:
    SerializeFieldObject(ISerializeOp* ser, const tstring& n, T* pObj, const IFieldAttribute* attr)
        : ISerializeField(ser, typeid(T), n, attr)
        , m_pObject(pObj){};
    virtual ~SerializeFieldObject(){};

public:
    T* m_pObject{};
};

NS_END
