/*!
 * \file LogUtil_Android.cpp
 * \date 10-11-2014
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "../../LogUtil.h"
#include <android/log.h>
#include <sstream>
#include <iostream>

NS_BEGIN

void LogUtil::print(const char* location, int line, LogPriority prio, const tstring& strMsg)
{
	static const int PRIORITY_MAP[static_cast<int>(LogPriority::NumPriority)] =
	{
		ANDROID_LOG_INFO,
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_WARN,
		ANDROID_LOG_ERROR,
	};

	std::ostringstream oss;
	oss << location << "(" << line << "): " << PRIORITY_MAP[static_cast<int>(prio)] << ": " << strMsg << std::endl;
	__android_log_write(PRIORITY_MAP[static_cast<int>(prio)], "tag", oss.str().c_str());
}

bool LogUtil::openLogFile(const tstring& logFile)
{
	return true;
}

void LogUtil::closeLogFile()
{
	// TODO: 
}

NS_END
