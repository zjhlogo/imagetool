/*!
 * \file BufferStream.cpp
 * \date 10-17-2014 13:41:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BufferStream.h"
#include <cstring>

NS_BEGIN

BufferStreamReader::BufferStreamReader(const BufferData& bufferData)
{
	m_pBuffer = bufferData.data();
	m_bufferSize = bufferData.size();
}

BufferStreamReader::~BufferStreamReader()
{
	// TODO: 
}

bool BufferStreamReader::read(void* pDataOut, std::size_t size)
{
	if (!pDataOut || size + m_readPos > m_bufferSize) return false;
	if (size <= 0) return true;

	memcpy(pDataOut, m_pBuffer+m_readPos, size);
	m_readPos += size;

	return true;
}

bool BufferStreamReader::skip(int size)
{
	if (size + m_readPos > m_bufferSize) return false;
	m_readPos += size;
	return true;
}


BufferStreamWriter::BufferStreamWriter(std::size_t initSize, std::size_t stepSize)
	:m_stepSize(stepSize)
{
	if (initSize > 0) m_bufferData.resize(initSize);
}

BufferStreamWriter::~BufferStreamWriter()
{

}

bool BufferStreamWriter::write(const void* pData, std::size_t size)
{
	if (m_currentPos + size > m_bufferData.size())
	{
		std::size_t adjustStep = ((size + m_stepSize - 1) / m_stepSize) * m_stepSize;
		m_bufferData.resize(m_bufferData.size() + adjustStep);
		if (m_currentPos + size > m_bufferData.size()) return false;
	}

	memcpy(&m_bufferData[m_currentPos], pData, size);
	m_currentPos += size;

	return true;
}

void BufferStreamWriter::reset()
{
	m_stepSize = 0;
	m_bufferData.clear();
	m_currentPos = 0;
}

NS_END
