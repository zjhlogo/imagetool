/*!
 * \file File_Win32.cpp
 *
 * \author zjhlogo
 * \date 24/04/2016
 *
 *
 */
#include "../../LogUtil.h"
#include "FileUtil.h"

#include <File.h>

NS_BEGIN

File::File()
{
}

File::File(const tstring& filePath, int flag /*= AM_READ*/)
{
    open(filePath, flag);
}

File::~File()
{
    close();
}

bool File::open(const tstring& filePath, int flag /*= AM_READ*/)
{
    tstring strMode;
    if (flag & AM_READ) strMode += "r";
    if (flag & AM_WRITE) strMode += "w";
    strMode += "b";

    FILE* pFile = nullptr;
    fopen_s(&pFile, filePath.c_str(), strMode.c_str());
    if (!pFile)
    {
        LOGE("open file failed {0}", filePath);
        return false;
    }

    m_mode = flag;
    m_fileHandler = pFile;
    m_fileSize = FileUtil::getFileSize(filePath);

    return true;
}

void File::close()
{
    if (m_fileHandler)
    {
        fclose((FILE*)m_fileHandler);
        m_fileHandler = nullptr;
    }
}

int64 File::seek(int64 offset, SeekType seekType /* = SeekType::Current */)
{
    if (!m_fileHandler) return 0;

    int seekFrom = SEEK_CUR;
    if (seekType == SeekType::Begin)
        seekFrom = SEEK_SET;
    else if (seekType == SeekType::End)
        seekFrom = SEEK_END;

    return fseek((FILE*)m_fileHandler, static_cast<size_t>(offset), seekFrom);
}

int64 File::tell()
{
    if (!m_fileHandler) return 0;

    return ftell((FILE*)m_fileHandler);
}

int64 File::read(void* dataOut, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_READ) == 0) return 0;
    if (size <= 0) return 0;

    return fread(dataOut, 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::read(BufferData& dataOut, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_READ) == 0) return 0;
    if (size <= 0) return 0;

    dataOut.resize(static_cast<size_t>(size));
    return fread(dataOut.data(), 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::write(const void* data, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_WRITE) == 0) return 0;
    if (size <= 0) return 0;

    return fwrite(data, 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::write(const BufferData& data)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_WRITE) == 0) return 0;

    return fwrite(data.data(), 1, data.size(), (FILE*)m_fileHandler);
}

NS_END
