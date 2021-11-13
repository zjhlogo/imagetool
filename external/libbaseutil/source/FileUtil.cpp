/*!
 * \file FileUtil.cpp
 * \date 7-18-2012
 *
 *
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "FileUtil.h"
#include "LogUtil.h"
#include "StringUtil.h"

#include <File.h>
#include <sys/stat.h>

#include <algorithm>

#if defined(_WIN32)
#    include <direct.h>
#    include <dirent_win32.h>
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#    include <dirent.h>
#endif

NS_BEGIN

const tstring FileUtil::CURRENT_DIR = ".";
const tstring FileUtil::PARENT_DIR = "..";

tstring FileUtil::m_writableDir;

bool FileUtil::readFileIntoString(tstring& stringOut, const tstring& filePath)
{
    BufferData bufferData;
    File file(filePath);
    if (file.read(bufferData, file.fileSize()) != file.fileSize()) return false;
    bufferData.push_back('\0');

    stringOut = (char*)bufferData.data();
    return true;
}

bool FileUtil::writeStringIntoFile(tstring& stringIn, const tstring& filePath)
{
    File file;
    if (!file.open(filePath, File::AM_WRITE)) return false;

    file.write(stringIn.c_str(), stringIn.size());
    file.close();

    return true;
}

void FileUtil::setWritableDir(const tstring& writableDir)
{
    m_writableDir = formatFilePath(writableDir);
}

const tstring& FileUtil::getWritablePath()
{
    return m_writableDir;
}

size_t FileUtil::listFilesInDirectory(StringSet& filesOut,
                                      const tstring& dir,
                                      const StringSet& includeExts /* = TS_STRING() */,
                                      const StringSet& excludeExts /* = TS_STRING() */)
{
    enumlateFiles(filesOut, dir);
    LOGI("enumlate files: {}, count = {}", dir, filesOut.size());

    filterFilesByExtension(filesOut, includeExts, excludeExts);
    LOGI("filter files by extension: count = {}", dir, filesOut.size());

    return filesOut.size();
}

size_t FileUtil::listFilesInDirectory(StringSet& filesOut, const tstring& dir, const std::regex& rg)
{
    enumlateFiles(filesOut, dir, rg);
    return filesOut.size();
}

size_t FileUtil::filterFilesByExtension(StringSet& filesInOut,
                                        const StringSet& includeExts /* = TS_STRING() */,
                                        const StringSet& excludeExts /* = TS_STRING() */)
{
    if (includeExts.empty() && !excludeExts.empty())
    {
        StringSet oldFiles = filesInOut;
        filesInOut.clear();
        for (const auto& filePath : oldFiles)
        {
            if (isFileExtension(filePath, excludeExts)) continue;
            filesInOut.insert(filePath);
        }
    }
    else if (!includeExts.empty() && excludeExts.empty())
    {
        StringSet oldFiles = filesInOut;
        filesInOut.clear();
        for (const auto& filePath : oldFiles)
        {
            if (isFileExtension(filePath, includeExts))
            {
                filesInOut.insert(filePath);
            }
        }
    }
    else if (!includeExts.empty() && !excludeExts.empty())
    {
        StringSet newIncludeExt = includeExts;
        for (const auto& ext : excludeExts)
        {
            newIncludeExt.erase(ext);
        }

        StringSet oldFiles = filesInOut;
        filesInOut.clear();
        for (const auto& filePath : oldFiles)
        {
            if (isFileExtension(filePath, newIncludeExt))
            {
                filesInOut.insert(filePath);
            }
        }
    }

    return filesInOut.size();
}

bool FileUtil::isFileExtension(const tstring& filePath, const StringSet& exts, bool caseSensitive /* = false */)
{
    if (exts.size() <= 0) return true;

    tstring fileExt = getFileExtension(filePath);
    if (!caseSensitive)
    {
        tstring lowerFileExt = fileExt;
        std::transform(lowerFileExt.begin(), lowerFileExt.end(), lowerFileExt.begin(), ::tolower);

        StringSet lowerExts;
        for (const auto& ext : exts)
        {
            tstring lowerExt = ext;
            std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
            lowerExts.insert(lowerExt);
        }

        if (lowerExts.find(lowerFileExt) != lowerExts.end()) return true;
    }
    else
    {
        if (exts.find(fileExt) != exts.end()) return true;
    }

    return false;
}

bool FileUtil::makeDir(const tstring& strDir)
{
    if (strDir.empty() || strDir == "/") return false;
    if (isDirectory(strDir)) return true;

    StringList arrDir;
    int numDirs = StringUtil::splitString(arrDir, strDir, "/");
    if (numDirs <= 0) return false;

    tstring strSpliter;
    tstring strFileDirToMake;
    for (int i = 0; i < numDirs; ++i)
    {
        strFileDirToMake += strSpliter;
        strFileDirToMake += arrDir[i];
#if defined(_WIN32)
        _mkdir(strFileDirToMake.c_str());
#else
        mkdir(strFileDirToMake.c_str(), 0777);
#endif
        strSpliter = "/";
    }

    return true;
}

bool FileUtil::isFile(const tstring& filePath)
{
    struct stat sb;
    if (stat(filePath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
    {
        return true;
    }

    return false;
}

bool FileUtil::isDirectory(const tstring& filePath)
{
    struct stat sb;
    if (stat(filePath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }

    return false;
}

size_t FileUtil::getFileSize(const tstring& filePath)
{
    struct stat sb;
    if (stat(filePath.c_str(), &sb) == 0)
    {
        return sb.st_size;
    }

    return 0;
}

tstring FileUtil::getFileExtension(const tstring& strIn)
{
    size_t nPos = strIn.rfind('.');
    if (nPos != tstring::npos)
    {
        return strIn.substr(nPos + 1, tstring::npos);
    }

    return EMPTY_STRING;
}

tstring FileUtil::replaceFileExtension(const tstring& strIn, const tstring& newExt)
{
    size_t nPos = strIn.rfind('.');
    if (nPos != tstring::npos)
    {
        if (*newExt.begin() == '.')
            return strIn.substr(0, nPos) + newExt;
        else
            return strIn.substr(0, nPos) + '.' + newExt;
    }

    return strIn;
}

tstring FileUtil::getFileDirectory(const tstring& strIn)
{
    size_t nPos = strIn.rfind('/');
    if (nPos != tstring::npos)
    {
        return formatFileDir(strIn.substr(0, nPos));
    }

    nPos = strIn.rfind('\\');
    if (nPos != tstring::npos)
    {
        return formatFileDir(strIn.substr(0, nPos));
    }

    return EMPTY_STRING;
}

tstring FileUtil::getFileName(const tstring& strIn)
{
    size_t nPosBegin = strIn.rfind('/');
    if (nPosBegin == tstring::npos)
    {
        nPosBegin = strIn.rfind('\\');
        if (nPosBegin == tstring::npos) nPosBegin = -1;
    }
    ++nPosBegin;

    size_t nPosEnd = strIn.length();
    return strIn.substr(nPosBegin, nPosEnd);
}

tstring FileUtil::getFileNameWithoutExtension(const tstring& strIn)
{
    size_t nPosBegin = strIn.rfind('/');
    if (nPosBegin == tstring::npos)
    {
        nPosBegin = strIn.rfind('\\');
        if (nPosBegin == tstring::npos) nPosBegin = -1;
    }
    ++nPosBegin;

    size_t nPosEnd = strIn.rfind('.');
    if (nPosEnd == tstring::npos) nPosEnd = strIn.length();

    return strIn.substr(nPosBegin, nPosEnd - nPosBegin);
}

bool FileUtil::removeFile(const tstring& strFile)
{
    return (remove(strFile.c_str()) == 0);
}

bool FileUtil::renameFile(const tstring& strOldFile, const tstring& strNewFile)
{
    return (rename(strOldFile.c_str(), strNewFile.c_str()) == 0);
}

tstring FileUtil::formatFilePath(const tstring& strPath)
{
    tstring strOut = strPath;
    for (auto& ch : strOut)
    {
        if (ch == '\\') ch = '/';
    }

    return strOut;
}

tstring FileUtil::formatFileDir(const tstring& strDir)
{
    tstring strOut = formatFilePath(strDir);

    if (strOut.rbegin() != strOut.rend() && (*strOut.rbegin()) != '/')
    {
        strOut += "/";
    }

    return strOut;
}

tstring FileUtil::getRelativePath(const tstring& src, const tstring& dest)
{
    tstring formatedSrc = formatFilePath(src);
    tstring formatedDest = formatFilePath(dest);

    StringList dirSrcArray;
    StringUtil::splitString(dirSrcArray, formatedSrc, "/");

    StringList dirDestArray;
    StringUtil::splitString(dirDestArray, formatedDest, "/");

    while (dirSrcArray.size() > 0 && dirDestArray.size() > 0 && dirSrcArray[0] == dirDestArray[0])
    {
        dirSrcArray.erase(dirSrcArray.begin());
        dirDestArray.erase(dirDestArray.begin());
    }

    for (size_t i = 0; i < dirSrcArray.size() - 1; ++i)
    {
        dirDestArray.insert(dirDestArray.begin(), "..");
    }

    return StringUtil::joinString(dirDestArray, "/");
}

tstring FileUtil::getAbsolutePath(const tstring& basePath, const tstring& relativePath)
{
    tstring formatedBasePath = formatFilePath(basePath);
    tstring formatedRelative = formatFilePath(relativePath);

    StringList basePathArray;
    StringUtil::splitString(basePathArray, formatedBasePath, "/");

    StringList relativePathArray;
    StringUtil::splitString(relativePathArray, formatedRelative, "/");

    basePathArray.pop_back();
    while (basePathArray.size() > 0 && relativePathArray.size() > 0 && relativePathArray[0] == "..")
    {
        basePathArray.pop_back();
        relativePathArray.erase(relativePathArray.begin());
    }

    for (const auto& dir : relativePathArray)
    {
        basePathArray.push_back(dir);
    }

    return StringUtil::joinString(basePathArray, "/");
}

int FileUtil::enumlateFiles(StringSet& filesOut, const tstring& dir)
{
    std::regex rg(".*");
    return enumlateFiles(filesOut, dir, rg);
}

int FileUtil::enumlateFiles(StringSet& filesOut, const tstring& dir, const std::regex& rg)
{
    int numFilesFound = 0;

    DIR* pDir = opendir(dir.c_str());
    if (pDir)
    {
        /* print all the files and directories within directory */
        dirent* pEnt = nullptr;
        while ((pEnt = readdir(pDir)) != nullptr)
        {
            if (pEnt->d_type == DT_DIR)
            {
                if (CURRENT_DIR != pEnt->d_name && PARENT_DIR != pEnt->d_name)
                {
                    tstring currDir = dir + "/" + pEnt->d_name;
                    numFilesFound += enumlateFiles(filesOut, currDir, rg);
                }
            }
            else if (pEnt->d_type == DT_REG)
            {
                tstring currFilePath = dir + "/" + pEnt->d_name;
                if (std::regex_match(currFilePath, rg))
                {
                    filesOut.insert(currFilePath);
                }
                ++numFilesFound;
            }
        }
        closedir(pDir);
    }

    return numFilesFound;
}

NS_END
