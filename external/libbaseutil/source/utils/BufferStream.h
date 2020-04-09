/*!
 * \file BufferStream.h
 * \date 10-17-2014 13:41:43
 *
 *
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#pragma once

#include "../BaseType.h"

NS_BEGIN

class BufferStreamReader
{
public:
    BufferStreamReader(const BufferData& bufferData);
    virtual ~BufferStreamReader();

    bool read(void* pDataOut, std::size_t size);
    bool skip(int size);

    void reset() { m_readPos = 0; };
    std::size_t getSize() const { return m_bufferSize; };
    std::size_t getPosition() const { return m_readPos; };
    const uint8* getData() const { return m_pBuffer + m_readPos; }

private:
    const uint8* m_pBuffer{};
    std::size_t m_bufferSize{};
    std::size_t m_readPos{};
};

class BufferStreamWriter
{
public:
    BufferStreamWriter(std::size_t initSize = 0, std::size_t stepSize = 1);
    virtual ~BufferStreamWriter();

    bool write(const void* pData, std::size_t size);
    void reset();

    const void* getData() const { return m_bufferData.data(); };
    std::size_t getSize() const { return m_currentPos; };
    const BufferData& getBuffData() const { return m_bufferData; }

private:
    std::size_t m_stepSize{};
    BufferData m_bufferData;
    std::size_t m_currentPos{};
};

NS_END
