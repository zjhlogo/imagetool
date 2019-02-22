/*!
 * \file TimeUtil_Win32.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 03/31/2016
 *
 * 
 */
#include "TimeUtil_Win32.h"
#include <windows.h>

NS_BEGIN

static LARGE_INTEGER g_frequency;

float TimeUtil::now()
{
	static bool s_bInitialized = initialize();

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	return (float)counter.QuadPart / g_frequency.QuadPart;
}

bool TimeUtil::initialize()
{
	return (QueryPerformanceFrequency(&g_frequency) == TRUE);
}

NS_END