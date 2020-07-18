/*!
 * \file LogUtil_Linux.cpp
 * \date 10-11-2014
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "../../LogUtil.h"
#include <sstream>
#include <iostream>
#include <mutex>

NS_BEGIN

void LogUtil::print(const char* location, int line, LogPriority prio, const tstring& strMsg)
{
	static std::mutex s_mutex;

	static const char* PRIORITY_MAP[static_cast<int>(LogPriority::NumPriority)] =
	{
		"Info",
		"Debug",
		"Warning",
		"Error",
	};

	s_mutex.lock();

//	std::ostringstream oss;
//	oss << location << "(" << line << "): " << PRIORITY_MAP[static_cast<int>(prio)] << ": " << strMsg << std::endl;
    std::cout << PRIORITY_MAP[static_cast<int>(prio)] << ": " << strMsg << std::endl;

	s_mutex.unlock();
}

bool LogUtil::openLogFile(const tstring& logFile)
{
	// TODO: 
	return true;
}

void LogUtil::closeLogFile()
{
	// TODO: 
}

NS_END
