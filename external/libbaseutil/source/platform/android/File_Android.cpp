/*!
 * \file File_Android.cpp
 *
 * \author zjhlogo
 * \date 24/04/2016
 *
 * 
 */
#include <File.h>
#include "FileUtil.h"
#include "../../LogUtil.h"
#include <android/asset_manager_jni.h>

AAssetManager* g_pAsssetsMgr = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_spank_static_1baseutil_FileUtil_initNativeAssetsManager(JNIEnv* env, jclass cls, jobject assetManager)
{
	LOGI("initNativeAssetsManager");
	g_pAsssetsMgr = AAssetManager_fromJava(env, assetManager);
}
#ifdef __cplusplus
}
#endif

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
	AAsset* pAsset = AAssetManager_open(g_pAsssetsMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
	if (!pAsset)
	{
		LOGE("open file failed {0}", filePath);
		return false;
	}

	m_mode = flag;
	m_fileHandler = pAsset;
	m_fileSize = AAsset_getLength(pAsset);

	return true;
}

void File::close()
{
	if (m_fileHandler)
	{
		AAsset_close((AAsset*)m_fileHandler);
		m_fileHandler = nullptr;
	}
}

int64 File::seek(int64 offset, SeekType seekType /*= SeekType::Current*/)
{
	if (!m_fileHandler) return 0;

	int seekFrom = SEEK_CUR;
	if (seekType == SeekType::Begin) seekFrom = SEEK_SET;
	else if (seekType == SeekType::End) seekFrom = SEEK_END;

	return AAsset_seek((AAsset*)m_fileHandler, static_cast<size_t>(offset), seekFrom);
}

int64 File::tell()
{
	if (!m_fileHandler) return 0;

	return AAsset_seek((AAsset*)m_fileHandler, 0, SEEK_CUR);
}

int64 File::read(void* dataOut, int64 size)
{
	if (!m_fileHandler) return 0;
	if ((m_mode & AM_READ) == 0) return 0;
	if (size <= 0) return 0;

	return AAsset_read((AAsset*)m_fileHandler, dataOut, static_cast<size_t>(size));
}

int64 File::read(BufferData& dataOut, int64 size)
{
	if (!m_fileHandler) return 0;
	if ((m_mode & AM_READ) == 0) return 0;
	if (size <= 0) return 0;

	dataOut.resize(static_cast<size_t>(size));
	return AAsset_read((AAsset*)m_fileHandler, dataOut.data(), static_cast<size_t>(size));
}

int64 File::write(const void* data, int64 size)
{
	return 0;
}

int64 File::write(const BufferData& data)
{
	return 0;
}

NS_END
