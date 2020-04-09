/*!
 * \file LogUtil_Win32.cpp
 * \date 10-11-2014
 *
 *
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "../../File.h"
#include "../../LogUtil.h"

#include <windows.h>

#include <iostream>
#include <mutex>
#include <sstream>

NS_BEGIN

class LogFileInstance
{
public:
    LogFileInstance(){};

    ~LogFileInstance() { closeLogFile(); }

    bool isReady() const { return (m_pFile != nullptr); };

    bool openLogFile(const tstring& logFile)
    {
        if (m_pFile) return false;

        m_pFile = new File(logFile, File::AM_WRITE);
        if (!m_pFile) return false;

        m_logFile = logFile;
        return true;
    }

    void closeLogFile()
    {
        SAFE_DELETE(m_pFile);
        m_logFile.clear();
    }

    const tstring& getLogFilePath() const { return m_logFile; };

    bool logString(const tstring& strLog)
    {
        if (!m_pFile) return false;

        m_pFile->write((const int8*)strLog.data(), strLog.length());

        return true;
    }

private:
    File* m_pFile{};
    tstring m_logFile;
};

static LogFileInstance g_logFile;

void LogUtil::print(const char* location, int line, LogPriority prio, const tstring& strMsg)
{
    static std::mutex s_mutex;

    static const char* PRIORITY_MAP[static_cast<int>(LogPriority::NumPriority)] = {
        "Info",
        "Debug",
        "Warning",
        "Error",
    };

    s_mutex.lock();

    std::ostringstream oss;
    oss << location << "(" << line << "): " << PRIORITY_MAP[static_cast<int>(prio)] << ": " << strMsg << std::endl;
    g_logFile.logString(oss.str());

#if defined(_DEBUG)
    OutputDebugString(oss.str().c_str());
#endif

    s_mutex.unlock();
}

bool LogUtil::openLogFile(const tstring& logFile)
{
    return g_logFile.openLogFile(logFile);
}

void LogUtil::closeLogFile()
{
    g_logFile.closeLogFile();
}

NS_END
