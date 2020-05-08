/*!
 * \file ImageTool.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 四月 2015
 *
 *
 */
#pragma once

#include "PackingUtil.h"

#include <FreeImagePlus/FreeImagePlus.h>
#include <baseutil.h>

#include <bitset>
#include <cmath>
#include <iomanip>
#include <list>
#include <map>
#include <vector>

#define HASH_LENGTH 64
#define IMAGE_SIZE 32

class ImageTool
{
public:
    enum TEXTURES
    {
        TEX_DIFFUSE = 0,
        TEX_NORMAL,
        TEX_SPECULAR,
        NUM_TEXTURES,
    };

    class SubImageInfo
    {
    public:
        spank::tstring strId;
        spank::tstring textures[NUM_TEXTURES];
    };

    typedef std::map<spank::tstring, SubImageInfo> SubImageInfoMap;

    class AtlasInfo
    {
    public:
        PackingUtil::PieceFileInfoMap pieceFileInfoMap;
        spank::tstring filePath;
        int width{};
        int height{};
    };

    class FileInfo
    {
    public:
        spank::tstring fileName;
        std::bitset<HASH_LENGTH> pHash;
        int width{};
        int height{};
    };

    typedef std::vector<FileInfo> FileInfoList;
    typedef std::list<FileInfo> FileInfoLinkList;

    class FileInfoGroup
    {
    public:
        FileInfoList fileList;
    };

    typedef std::vector<FileInfoGroup> FileInfoGroupList;

    class BlenderIconHead
    {
    public:
        unsigned int icon_w, icon_h;
        unsigned int orig_x, orig_y;
        unsigned int canvas_w, canvas_h;
    };

public:
    ImageTool();
    ~ImageTool();

    bool resizeImageByPixel(const spank::StringList& files, const spank::StringList& exts, int pixelWidth, int pixelHeight);
    bool resizeImageByPercent(const spank::StringList& files, const spank::StringList& exts, float scale);
    bool stretchImageByPixel(const spank::StringList& files, const spank::StringList& exts, int pixelWidth, int pixelHeight);
    bool cropImage(const spank::StringList& files, const spank::StringList& exts, int width, int height);
    bool convert(const spank::StringList& files, const spank::StringList& exts, const spank::tstring& destType, int depth = -1, bool deleteSrc = false);
    bool convertBlenderIconFile(const spank::StringList& files,
                                const spank::StringList& exts,
                                const spank::tstring& destType,
                                int depth = -1,
                                bool deleteSrc = false);
    bool optiPng(const spank::StringList& files);
    bool replaceChannel(const spank::tstring& strFile1,
                        FREE_IMAGE_COLOR_CHANNEL eChannel1,
                        const spank::tstring& strFile2,
                        FREE_IMAGE_COLOR_CHANNEL eChannel2,
                        const spank::tstring& strOutFile);
    bool binPack(const spank::StringList& files,
                 const spank::StringList& exts,
                 const spank::tstring& outputImageType,
                 const spank::tstring& strOutFile,
                 int packingFlag = 0);
    bool advBinPack(const spank::tstring& strFile, const spank::tstring& strOutFile);
    bool detectRed(const spank::StringList& files, const spank::StringList& exts, const spank::tstring& strOutFile);
    bool splitByCount(const spank::StringList& files,
                      const spank::StringList& exts,
                      int rows,
                      int cols,
                      const spank::tstring& outputType,
                      const spank::tstring& dirOut);
    bool splitBySize(const spank::StringList& files,
                     const spank::StringList& exts,
                     int subWidth,
                     int subHeight,
                     const spank::tstring& outputType,
                     const spank::tstring& dirOut);
    bool subtract(const spank::tstring& file1, const spank::tstring& file2, const spank::tstring& strOutFile);
    bool groupByFingerPrint(const spank::StringList& files,
                            const spank::StringList& exts,
                            const spank::tstring& outputType,
                            const spank::tstring& commandFormat,
                            int distance);
    bool groupByName(const spank::StringList& files,
                     const spank::StringList& exts,
                     const spank::tstring& outputType,
                     const spank::tstring& commandFormat,
                     int distance);
    bool removeAlpha(const spank::StringList& files, const spank::tstring& reg, int threshold);

private:
    void collectFiles(spank::StringSet& fileSet, const spank::StringList& files, const spank::StringList& exts);
    void collectFiles(spank::StringSet& fileSet, const spank::StringList& files, const spank::tstring& reg);
    const spank::tstring& getFilePath(TEXTURES eTexture);
    void printGroupInfo(const FileInfoGroupList& fileInfoGroupList, const spank::tstring& outputType, const spank::tstring& commandFormat);

    bool convertToType(fipImage& image, const spank::tstring& destFilePath, FREE_IMAGE_FORMAT dstFif, int depth);
    void DCT(const fipImage& image, const int& n, double** iMatrix);
    void coefficient(const int& n, double** quotient, double** quotientT);
    void matrixMultiply(double** A, double** B, int n, double** result);
    double calcAverage(double** iMatrix, int size);
    void fingerPrint(double** iMatrix, int size, std::bitset<HASH_LENGTH>& phash, double averagePix);
    int hammingDistance(const std::bitset<HASH_LENGTH>& query, const std::bitset<HASH_LENGTH>& target);
    spank::tstring bitTohex(const std::bitset<HASH_LENGTH>& target);

    bool getTrimedImageInfo(PackingUtil::PieceFileInfo& fileInfo, const fipImage& image);
    bool isEmptyRow(int y, int width, int height, BYTE* pixel);
    bool isEmptyCol(int x, int width, int height, BYTE* pixel);
    bool isAlphaEqual(fipImage& image, int threshold);

    void saveAtlasInfoXml(const spank::tstring& id,
                          const spank::tstring& imageFile,
                          const spank::tstring& configFile,
                          int width,
                          int height,
                          const PackingUtil::PieceFileInfoMap& pieceFileInfoMap);
    void saveAtlasInfoCpp(const spank::tstring& strOutDir,
                          const spank::tstring& className,
                          const spank::tstring& imageType,
                          float width,
                          float height,
                          const PackingUtil::PieceFileInfoMap& pieceFileInfoMap);
};
