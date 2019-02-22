/*!
 * \file LogUtil.h
 * \date 10-11-2014 16:13:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#pragma once

#include "BaseType.h"
#include <format.h>

NS_BEGIN

class LogUtil
{
public:
	enum class LogPriority
	{
		Info = 0,
		Debug,
		Warning,
		Error,
		NumPriority,
	};

public:
	static void print(const char* location, int line, LogPriority prio, const tstring& strMsg);

	static bool openLogFile(const tstring& logFile);
	static void closeLogFile();

};

NS_END

#define LOGI(...) (NS::LogUtil::print(__FILE__, __LINE__, NS::LogUtil::LogPriority::Info, fmt::format(__VA_ARGS__)))
#define LOGD(...) (NS::LogUtil::print(__FILE__, __LINE__, NS::LogUtil::LogPriority::Debug, fmt::format(__VA_ARGS__)))
#define LOGW(...) (NS::LogUtil::print(__FILE__, __LINE__, NS::LogUtil::LogPriority::Warning, fmt::format(__VA_ARGS__)))
#define LOGE(...) (NS::LogUtil::print(__FILE__, __LINE__, NS::LogUtil::LogPriority::Error, fmt::format(__VA_ARGS__)))
