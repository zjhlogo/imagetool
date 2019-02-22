/*!
 * \file FileUtil.h
 * \date 7-18-2012 12:55:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#pragma once

#include "BaseType.h"
#include <stdio.h>

NS_BEGIN

class FileUtil
{
public:
	static const tstring CURRENT_DIR;
	static const tstring PARENT_DIR;

public:
	static bool readFileIntoString(tstring& stringOut, const tstring& filePath);
	static bool writeStringIntoFile(tstring& stringIn, const tstring& filePath);
	static void setWritableDir(const tstring& writableDir);
	static const tstring& getWritablePath();

	static size_t listFilesInDirectory(StringSet& filesOut, const tstring& dir, const StringSet& includeExts = StringSet(), const StringSet& excludeExts = StringSet());
	static size_t filterFilesByExtension(StringSet& filesInOut, const StringSet& includeExts = StringSet(), const StringSet& excludeExts = StringSet());

	static bool isFileExtension(const tstring& filePath, const StringSet& exts, bool caseSensitive = false);

	static bool makeDir(const tstring& strDir);
	static bool isFile(const tstring& filePath);
	static bool isDirectory(const tstring& filePath);
	static size_t getFileSize(const tstring& filePath);

	static tstring getFileExtension(const tstring& strIn);
	static tstring replaceFileExtension(const tstring& strIn, const tstring& newExt);
	static tstring getFileDirectory(const tstring& strIn);
	static tstring getFileName(const tstring& strIn);
	static tstring getFileNameWithoutExtension(const tstring& strIn);

	static bool removeFile(const tstring& strFile);
	static bool renameFile(const tstring& strOldFile, const tstring& strNewFile);
	static tstring formatFilePath(const tstring& strPath);
	static tstring formatFileDir(const tstring& strDir);

	static tstring getRelativePath(const tstring& src, const tstring& dest);
	static tstring getAbsolutePath(const tstring& basePath, const tstring& relativePath);

private:
	static int enumlateFiles(StringSet& filesOut, const tstring& dir);

private:
	static tstring m_writableDir;

};

NS_END
