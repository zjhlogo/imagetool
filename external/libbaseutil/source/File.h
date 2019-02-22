/*!
 * \file File.h
 *
 * \author zjhlogo
 * \date 24/04/2016
 *
 * 
 */
#pragma once
#include "BaseType.h"

NS_BEGIN

class File
{
public:
	// access mode
	static const int AM_READ = 1 << 0;
	static const int AM_WRITE = 1 << 1;

	enum class SeekType
	{
		Current = 0,
		Begin,
		End,
	};

public:
	File();
	File(const tstring& filePath, int flag = AM_READ);
	~File();

	bool open(const tstring& filePath, int flag = AM_READ);
	void close();

	int64 seek(int64 offset, SeekType seekType = SeekType::Current);
	int64 tell();

	int64 read(void* dataOut, int64 size);
	int64 read(BufferData& dataOut, int64 size);

	int64 write(const void* data, int64 size);
	int64 write(const BufferData& data);

	int64 fileSize() const { return m_fileSize; };

private:
	void* m_fileHandler{};
	int64 m_fileSize{};
	int m_mode{ AM_READ };

};

NS_END
