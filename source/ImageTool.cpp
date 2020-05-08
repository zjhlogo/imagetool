/*!
 * \file ImageTool.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 四月 2015
 *
 *
 */
#include "ImageTool.h"

#include <tinyxml2.h>

#include <algorithm>
#include <iostream>
#include <string>

const char* ATLAS_CPP_TEMPLATE =
#include "Atlas.cpp.template"
    ;

const char* ATLAS_H_TEMPLATE =
#include "Atlas.h.template"
    ;

ImageTool::ImageTool()
{
}

ImageTool::~ImageTool()
{
}

bool ImageTool::resizeImageByPixel(const spank::StringList& files, const spank::StringList& exts, int pixelWidth, int pixelHeight)
{
    if (pixelWidth <= 0 || pixelHeight <= 0) return false;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        int oldWidth = image.getWidth();
        int oldHeight = image.getHeight();
        if (oldWidth == pixelWidth && oldHeight == pixelHeight)
        {
            std::cout << "Skip scale image, already under size (" << pixelWidth << ", " << pixelHeight << ") " << file << std::endl;
            continue;
        }

        float scaleWidth = static_cast<float>(pixelWidth) / static_cast<float>(oldWidth);
        float scaleHeight = static_cast<float>(pixelHeight) / static_cast<float>(oldHeight);
        float scale = scaleWidth < scaleHeight ? scaleWidth : scaleHeight;

        int newWidth = static_cast<int>(oldWidth * scale + 0.5f);
        int newHeight = static_cast<int>(oldHeight * scale + 0.5f);
        if (!image.rescale(newWidth, newHeight, FILTER_BILINEAR))
        {
            std::cout << "Error, scale " << scale << " image failed: (" << oldWidth << ", " << oldHeight << ")->(" << newWidth << ", " << newHeight << ") "
                      << file << std::endl;
            continue;
        }

        if (!image.save(file.c_str()))
        {
            std::cout << "Error, save image failed: " << file << std::endl;
            continue;
        }

        std::cout << "image scale success (" << oldWidth << ", " << oldHeight << ")->(" << newWidth << ", " << newHeight << ") " << file << std::endl;
    }

    return true;
}

bool ImageTool::resizeImageByPercent(const spank::StringList& files, const spank::StringList& exts, float scale)
{
    if (scale == 1.0f) return true;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        int oldWidth = image.getWidth();
        int oldHeight = image.getHeight();
        int newWidth = static_cast<int>(oldWidth * scale + 0.5f);
        int newHeight = static_cast<int>(oldHeight * scale + 0.5f);
        if (!image.rescale(newWidth, newHeight, FILTER_BILINEAR))
        {
            std::cout << "Error, scale " << scale << " image failed: (" << oldWidth << ", " << oldHeight << ")->(" << newWidth << ", " << newHeight << ") "
                      << file << std::endl;
            continue;
        }

        if (!image.save(file.c_str()))
        {
            std::cout << "Error, save image failed: " << file << std::endl;
            continue;
        }

        std::cout << "image scale success (" << oldWidth << ", " << oldHeight << ")->(" << newWidth << ", " << newHeight << ") " << file << std::endl;
    }

    return true;
}

bool ImageTool::stretchImageByPixel(const spank::StringList& files, const spank::StringList& exts, int pixelWidth, int pixelHeight)
{
    if (pixelWidth <= 0 || pixelHeight <= 0) return false;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        int oldWidth = image.getWidth();
        int oldHeight = image.getHeight();
        if (oldWidth == pixelWidth && oldHeight == pixelHeight)
        {
            std::cout << "Skip scale image, already under size (" << pixelWidth << ", " << pixelHeight << ") " << file << std::endl;
            continue;
        }

        if (!image.rescale(pixelWidth, pixelHeight, FILTER_BILINEAR))
        {
            std::cout << "Error, scale image failed: (" << oldWidth << ", " << oldHeight << ")->(" << pixelWidth << ", " << pixelHeight << ") " << file
                      << std::endl;
            continue;
        }

        if (!image.save(file.c_str()))
        {
            std::cout << "Error, save image failed: " << file << std::endl;
            continue;
        }

        std::cout << "image scale success (" << oldWidth << ", " << oldHeight << ")->(" << pixelWidth << ", " << pixelHeight << ") " << file << std::endl;
    }

    return true;
}

bool ImageTool::cropImage(const spank::StringList& files, const spank::StringList& exts, int width, int height)
{
    if (width <= 0 || height <= 0) return false;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    fipImage zeroImage(FIT_BITMAP, width, height, 8);

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        int oldWidth = image.getWidth();
        int oldHeight = image.getHeight();

        if (oldWidth == width && oldHeight == height)
        {
            std::cout << "Skip crop image, already under size (" << oldWidth << ", " << oldHeight << ") " << file << std::endl;
            continue;
        }

        fipImage newImage(image.getImageType(), width, height, image.getBitsPerPixel());
        newImage.setChannel(zeroImage, FICC_RED);
        newImage.setChannel(zeroImage, FICC_GREEN);
        newImage.setChannel(zeroImage, FICC_BLUE);
        newImage.setChannel(zeroImage, FICC_ALPHA);

        if (width > oldWidth && height > oldHeight)
        {
            // nothing to do
        }
        else if (width < oldWidth && height < oldHeight)
        {
            if (!image.crop(0, 0, width, height))
            {
                std::cout << "Error, crop old image failed: (" << width << ", " << height << ") " << file << std::endl;
                continue;
            }
        }
        else
        {
            int cropWidth = width < oldWidth ? width : oldWidth;
            int cropHeight = height < oldHeight ? height : oldHeight;

            if (!image.crop(0, 0, cropWidth, cropHeight))
            {
                std::cout << "Error, crop old image failed: (" << cropWidth << ", " << cropHeight << ") " << file << std::endl;
                continue;
            }
        }

        if (!newImage.pasteSubImage(image, 0, 0))
        {
            std::cout << "Error, crop image failed: (" << oldWidth << ", " << oldHeight << ")->(" << width << ", " << height << ") " << file << std::endl;
            continue;
        }

        if (!newImage.save(file.c_str()))
        {
            std::cout << "Error, save image failed: " << file << std::endl;
            continue;
        }

        std::cout << "image crop success (" << oldWidth << ", " << oldHeight << ")->(" << width << ", " << height << ") " << file << std::endl;
    }

    return true;
}

bool ImageTool::convert(const spank::StringList& files,
                        const spank::StringList& exts,
                        const spank::tstring& destType,
                        int depth /* = -1 */,
                        bool deleteSrc /* = false */)
{
    FREE_IMAGE_FORMAT dstFif = fipImage::identifyFIF(destType.c_str());
    if (dstFif == FIF_UNKNOWN) return false;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        spank::tstring destFilePath = spank::FileUtil::getFileDirectory(file) + "/" + spank::FileUtil::getFileNameWithoutExtension(file) + "." + destType;

        if (!convertToType(image, destFilePath, dstFif, depth))
        {
            std::cout << "Error file: " << file << std::endl;
        }

        if (deleteSrc)
        {
            spank::FileUtil::removeFile(file);
        }

        std::cout << "convert image (" << spank::FileUtil::getFileExtension(file) << "->" << destType << ") success " << destFilePath << std::endl;
    }

    return true;
}

bool ImageTool::convertBlenderIconFile(const spank::StringList& files,
                                       const spank::StringList& exts,
                                       const spank::tstring& destType,
                                       int depth /*= -1*/,
                                       bool deleteSrc /*= false*/)
{
    FREE_IMAGE_FORMAT dstFif = fipImage::identifyFIF(destType.c_str());
    if (dstFif == FIF_UNKNOWN) return false;

    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& filePath : fileSet)
    {
        spank::File file;
        if (!file.open(filePath))
        {
            std::cout << "Error, load image failed: " << filePath << std::endl;
            continue;
        }

        BlenderIconHead iconHead;
        file.read(&iconHead, sizeof(iconHead));

        fipImage image(FIT_BITMAP, iconHead.icon_w, iconHead.icon_h, 32);
        void* pixels = image.accessPixels();
        file.read(pixels, sizeof(int) * iconHead.icon_w * iconHead.icon_h);

        spank::tstring destFilePath = spank::FileUtil::getFileDirectory(filePath) + "/" + spank::FileUtil::getFileNameWithoutExtension(filePath) + "."
                                      + destType;
        if (!convertToType(image, destFilePath, dstFif, depth))
        {
            std::cout << "Error file: " << filePath << std::endl;
        }

        if (deleteSrc)
        {
            spank::FileUtil::removeFile(filePath);
        }

        std::cout << "convert image (" << spank::FileUtil::getFileExtension(filePath) << "->" << destType << ") success " << destFilePath << std::endl;
    }

    return true;
}

bool ImageTool::optiPng(const spank::StringList& files)
{
    spank::StringSet fileSet;

    spank::StringList exts;
    exts.push_back("png");

    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        if (!image.save(file.c_str()))
        {
            std::cout << "Error, save image failed: " << file << std::endl;
            continue;
        }

        std::cout << "png image optimize success " << file << std::endl;
    }

    return true;
}

bool ImageTool::replaceChannel(const spank::tstring& strFile1,
                               FREE_IMAGE_COLOR_CHANNEL eChannel1,
                               const spank::tstring& strFile2,
                               FREE_IMAGE_COLOR_CHANNEL eChannel2,
                               const spank::tstring& strOutFile)
{
    fipImage image1;
    if (!image1.load(strFile1.c_str()))
    {
        std::cout << "Error, load image failed: " << strFile1 << std::endl;
        return false;
    }

    fipImage image2;
    if (!image2.load(strFile2.c_str()))
    {
        std::cout << "Error, load image failed: " << strFile2 << std::endl;
        return false;
    }

    fipImage channelSrc;
    if (!image2.getChannel(channelSrc, eChannel2))
    {
        BOOL retrySuccess = FALSE;
        if (image2.convertTo32Bits())
        {
            retrySuccess = image2.getChannel(channelSrc, eChannel2);
        }

        if (!retrySuccess)
        {
            std::cout << "Error, can not get channel " << eChannel2 << " from image " << strFile2 << std::endl;
            return false;
        }
    }

    if (!image1.setChannel(channelSrc, eChannel1))
    {
        BOOL retrySuccess = FALSE;
        if (image1.convertTo32Bits())
        {
            retrySuccess = image1.setChannel(channelSrc, eChannel1);
        }

        if (!retrySuccess)
        {
            std::cout << "Error, set image channel " << eChannel1 << " failed: " << strFile1 << std::endl;
            return false;
        }
    }

    if (!image1.save(strOutFile.c_str()))
    {
        std::cout << "Error, save image failed: " << strOutFile << std::endl;
        return false;
    }

    return true;
}

bool ImageTool::binPack(const spank::StringList& files,
                        const spank::StringList& exts,
                        const spank::tstring& outputImageType,
                        const spank::tstring& strOutFile,
                        int packingFlag)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    if (packingFlag & PackingUtil::PACKING_FLAG_VERBOSE) std::cout << "collecting image size, total number of images: " << fileSet.size() << std::endl;

    PackingUtil::PieceFileInfoMap pieceFileInfoMap;
    for (auto file : fileSet)
    {
        fipImage image;
        image.load(file.c_str());

        PackingUtil::PieceFileInfo fileInfo;
        fileInfo.strId = spank::FileUtil::getFileNameWithoutExtension(file);
        fileInfo.path = file;

        if (packingFlag & PackingUtil::PACKING_FLAG_TRIM_WHITE_SPACE_BORDER)
        {
            getTrimedImageInfo(fileInfo, image);
        }
        else
        {
            fileInfo.width = image.getWidth();
            fileInfo.height = image.getHeight();
            fileInfo.srcWidth = fileInfo.width;
            fileInfo.srcHeight = fileInfo.height;
        }

        pieceFileInfoMap.insert(std::make_pair(fileInfo.strId, fileInfo));
    }

    PackingUtil packUtil;
    int width = 0;
    int height = 0;
    if (packingFlag & PackingUtil::PACKING_FLAG_VERBOSE)
    {
        std::cout << "start packing images..." << std::endl;
        packUtil.setProgressCallback([](const std::string& msg) { std::cout << msg << std::endl; });
    }

    spank::tstring outFileName = spank::FileUtil::getFileNameWithoutExtension(strOutFile);
    spank::tstring outFileExt = spank::FileUtil::getFileExtension(strOutFile);
    spank::tstring outImagePath = spank::FileUtil::replaceFileExtension(strOutFile, outputImageType);

    packUtil.pack(width, height, pieceFileInfoMap, outImagePath, packingFlag);

    if (packingFlag & PackingUtil::PACKING_FLAG_VERBOSE) std::cout << "final image size: " << width << "x" << height << std::endl;

    // save pieceFileInfoMap into config file
    if (outFileExt == "atlas")
    {
        spank::tstring outConfigPath = spank::FileUtil::replaceFileExtension(strOutFile, "atlas");
        saveAtlasInfoXml(outFileName, outImagePath, outConfigPath, width, height, pieceFileInfoMap);
    }
    else if (outFileExt == "h")
    {
        spank::tstring strOutDir = spank::FileUtil::getFileDirectory(strOutFile);
        if (strOutDir.empty()) strOutDir = ".";
        saveAtlasInfoCpp(strOutDir, outFileName, outputImageType, static_cast<float>(width), static_cast<float>(height), pieceFileInfoMap);
    }

    return true;
}

bool ImageTool::advBinPack(const spank::tstring& strFile, const spank::tstring& strOutFile)
{
    spank::tstring strDir = spank::FileUtil::getFileDirectory(strFile);

    SubImageInfoMap subImageInfoMap;
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(strFile.c_str()) != tinyxml2::XML_NO_ERROR) return false;
        tinyxml2::XMLElement* pXmlRoot = doc.RootElement();
        for (auto pXmlCandy = pXmlRoot->FirstChildElement("candy"); pXmlCandy != nullptr; pXmlCandy = pXmlCandy->NextSiblingElement("candy"))
        {
            SubImageInfo subImageInfo;
            subImageInfo.strId = pXmlCandy->Attribute("id");
            subImageInfo.textures[TEX_DIFFUSE] = pXmlCandy->Attribute("diffuse");
            subImageInfo.textures[TEX_NORMAL] = pXmlCandy->Attribute("normal");
            subImageInfo.textures[TEX_SPECULAR] = pXmlCandy->Attribute("specular");

            subImageInfoMap.insert(std::make_pair(subImageInfo.strId, subImageInfo));
        }
    }

    AtlasInfo atlasInfoList[NUM_TEXTURES];

    for (int i = 0; i < NUM_TEXTURES; ++i)
    {
        AtlasInfo& atlasInfo = atlasInfoList[i];
        atlasInfo.filePath = getFilePath((TEXTURES)i);

        for (auto it : subImageInfoMap)
        {
            SubImageInfo& subImageInfo = it.second;

            //			if (atlasInfo.pieceFileInfoMap.find(subImageInfo.textures[i]) != atlasInfo.pieceFileInfoMap.end()) continue;

            spank::tstring strFilePath = strDir + "/" + subImageInfo.textures[i] + ".png";

            fipImage image;
            if (!image.load(strFilePath.c_str()))
            {
                std::cout << "Error, opening image: " << strFilePath << std::endl;
                continue;
            }

            PackingUtil::PieceFileInfo fileInfo;
            fileInfo.strId = subImageInfo.strId;
            fileInfo.path = strFilePath;
            fileInfo.width = image.getWidth();
            fileInfo.height = image.getHeight();

            atlasInfo.pieceFileInfoMap.insert(std::make_pair(fileInfo.strId, fileInfo));
        }

        PackingUtil packUtil;
        packUtil.pack(atlasInfo.width, atlasInfo.height, atlasInfo.pieceFileInfoMap, atlasInfo.filePath, false);
    }

    {
        tinyxml2::XMLDocument doc;

        tinyxml2::XMLElement* pXmlImagePiece = doc.NewElement("ImagePiece");
        doc.LinkEndChild(pXmlImagePiece);

        tinyxml2::XMLElement* pXmlImageList = doc.NewElement("ImageList");
        pXmlImagePiece->LinkEndChild(pXmlImageList);

        tinyxml2::XMLElement* pXmlPieceList = doc.NewElement("PieceList");
        pXmlImagePiece->LinkEndChild(pXmlPieceList);

        for (int i = 0; i <= TEX_DIFFUSE; ++i)
        {
            AtlasInfo& atlasInfo = atlasInfoList[i];

            tinyxml2::XMLElement* pXmlImage = doc.NewElement("Image");
            pXmlImageList->LinkEndChild(pXmlImage);

            spank::tstring strId = spank::FileUtil::getFileNameWithoutExtension(atlasInfo.filePath);
            pXmlImage->SetAttribute("id", strId.c_str());
            pXmlImage->SetAttribute("file_name", atlasInfo.filePath.c_str());
            pXmlImage->SetAttribute("width", atlasInfo.width);
            pXmlImage->SetAttribute("height", atlasInfo.height);

            for (auto it : atlasInfo.pieceFileInfoMap)
            {
                PackingUtil::PieceFileInfo& fileInfo = it.second;

                tinyxml2::XMLElement* pXmlPiece = doc.NewElement("Piece");
                pXmlPieceList->LinkEndChild(pXmlPiece);

                pXmlPiece->SetAttribute("id", fileInfo.strId.c_str());
                pXmlPiece->SetAttribute("image_id", strId.c_str());
                pXmlPiece->SetAttribute("x", fileInfo.x);
                pXmlPiece->SetAttribute("y", fileInfo.y);
                pXmlPiece->SetAttribute("width", fileInfo.width);
                pXmlPiece->SetAttribute("height", fileInfo.height);
            }
        }

        doc.SaveFile(strOutFile.c_str());
    }

    return true;
}

bool ImageTool::detectRed(const spank::StringList& files, const spank::StringList& exts, const spank::tstring& strOutFile)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    tinyxml2::XMLDocument doc;

    tinyxml2::XMLElement* pXmlRoot = doc.NewElement("piece_dock");
    doc.LinkEndChild(pXmlRoot);

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        image.convertTo32Bits();

        int width = image.getWidth();
        int height = image.getHeight();

        tinyxml2::XMLElement* pXmlImage = doc.NewElement("piece");

        spank::tstring fileName = spank::FileUtil::getFileNameWithoutExtension(file);

        pXmlImage->SetAttribute("piece_id", fileName.c_str());
        pXmlRoot->LinkEndChild(pXmlImage);

        for (int y = 0; y < height; ++y)
        {
            BYTE* pPixelData = image.getScanLine(y);
            for (int x = 0; x < width; ++x)
            {
                int value = *(int*)&pPixelData[x];
                if (value == 0xFFFF0000)
                {
                    tinyxml2::XMLElement* pXmlPoint = doc.NewElement("point");
                    spank::XmlUtil::setAttributeVector2(glm::ivec2(x - width / 2, y - height / 2), pXmlPoint, "dot");
                    pXmlImage->LinkEndChild(pXmlPoint);
                }
            }
        }
    }

    doc.SaveFile(strOutFile.c_str());
    return true;
}

bool ImageTool::splitByCount(const spank::StringList& files,
                             const spank::StringList& exts,
                             int rows,
                             int cols,
                             const spank::tstring& outputType,
                             const spank::tstring& dirOut)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    spank::tstring formatedDir = spank::FileUtil::formatFileDir(dirOut);

    for (const auto& file : fileSet)
    {
        fipImage mainImage;
        if (!mainImage.load(file.c_str()))
        {
            std::cout << "Error, opening image: " << file << std::endl;
            return false;
        }

        int width = mainImage.getWidth();
        int height = mainImage.getHeight();

        spank::tstring mainImageFileName = spank::FileUtil::getFileNameWithoutExtension(file);

        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                fipImage subImage;
                mainImage.copySubImage(subImage, x * width / cols, y * height / rows, (x + 1) * width / cols, (y + 1) * height / rows);

                spank::tstring subImageFilePath = fmt::format("{0}{1}_{2:02d}_{3:02d}.{4}", formatedDir, mainImageFileName, y, x, outputType);
                subImage.save(subImageFilePath.c_str());
            }
        }

        std::cout << file << ", done" << std::endl;
    }

    return true;
}

bool ImageTool::splitBySize(const spank::StringList& files,
                            const spank::StringList& exts,
                            int subWidth,
                            int subHeight,
                            const spank::tstring& outputType,
                            const spank::tstring& dirOut)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    spank::tstring formatedDir = spank::FileUtil::formatFileDir(dirOut);

    for (const auto& file : fileSet)
    {
        fipImage mainImage;
        if (!mainImage.load(file.c_str()))
        {
            std::cout << "Error, opening image: " << file << std::endl;
            return false;
        }

        int width = mainImage.getWidth();
        int height = mainImage.getHeight();

        spank::tstring mainImageFileName = spank::FileUtil::getFileNameWithoutExtension(file);

        int rows = height / subHeight;
        int cols = width / subWidth;
        int index = 0;
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                fipImage subImage;
                mainImage.copySubImage(subImage, x * width / cols, y * height / rows, (x + 1) * width / cols, (y + 1) * height / rows);

                spank::tstring subImageFilePath = fmt::format("{0}{1}_{2:02d}_{3:02d}.{4}", formatedDir, mainImageFileName, y, x, outputType);
                subImage.save(subImageFilePath.c_str());
            }
        }

        std::cout << file << ", done" << std::endl;
    }

    return true;
}

bool ImageTool::subtract(const spank::tstring& file1, const spank::tstring& file2, const spank::tstring& strOutFile)
{
    fipImage image1;
    if (!image1.load(file1.c_str()))
    {
        std::cout << "Error, opening image: " << file1 << std::endl;
        return false;
    }

    fipImage image2;
    if (!image2.load(file2.c_str()))
    {
        std::cout << "Error, opening image: " << file2 << std::endl;
        return false;
    }

    int width = image1.getWidth();
    int height = image1.getHeight();

    if (width != image2.getWidth() || height != image2.getHeight())
    {
        std::cout << "Error, image size mismatch, both image width and height must be the same" << std::endl;
        return false;
    }

    unsigned int bpp1 = image1.getBitsPerPixel();
    if (image1.getColorType() == FIC_PALETTE)
    {
        if (!image1.convertTo24Bits())
        {
            std::cout << "Error, convert image to 24 bits failed: " << file1 << std::endl;
            return false;
        }
        bpp1 = 24;
    }

    unsigned int bpp2 = image2.getBitsPerPixel();
    if (image2.getColorType() == FIC_PALETTE)
    {
        if (!image2.convertTo24Bits())
        {
            std::cout << "Error, convert image to 24 bits failed: " << file2 << std::endl;
            return false;
        }
        bpp2 = 24;
    }

    unsigned int maxBpp = bpp1 > bpp2 ? bpp1 : bpp2;
    if (bpp1 < maxBpp)
    {
        switch (maxBpp)
        {
        case 8:
        {
            if (!image1.convertTo8Bits())
            {
                std::cout << "Error, convert image to 8 bits failed: " << file1 << std::endl;
                return false;
            }
        }
        break;
        case 16:
        {
            if (!image1.convertTo16Bits565())
            {
                std::cout << "Error, convert image to 16 bits failed: " << file1 << std::endl;
                return false;
            }
        }
        break;
        case 24:
        {
            if (!image1.convertTo24Bits())
            {
                std::cout << "Error, convert image to 24 bits failed: " << file1 << std::endl;
                return false;
            }
        }
        break;
        case 32:
        {
            if (!image1.convertTo24Bits())
            {
                std::cout << "Error, convert image to 24 bits failed: " << file1 << std::endl;
                return false;
            }
        }
        break;
        default:
        {
            std::cout << "Error, image bits per pixel mismatch, both image bits per pixel must be the same " << std::endl;
            return false;
        }
        break;
        }
    }

    if (bpp2 < maxBpp)
    {
        switch (maxBpp)
        {
        case 8:
        {
            if (!image2.convertTo8Bits())
            {
                std::cout << "Error, convert image to 8 bits failed: " << file2 << std::endl;
                return false;
            }
        }
        break;
        case 16:
        {
            if (!image2.convertTo16Bits565())
            {
                std::cout << "Error, convert image to 16 bits failed: " << file2 << std::endl;
                return false;
            }
        }
        break;
        case 24:
        {
            if (!image2.convertTo24Bits())
            {
                std::cout << "Error, convert image to 24 bits failed: " << file2 << std::endl;
                return false;
            }
        }
        break;
        case 32:
        {
            if (!image2.convertTo24Bits())
            {
                std::cout << "Error, convert image to 24 bits failed: " << file2 << std::endl;
                return false;
            }
        }
        break;
        default:
        {
            std::cout << "Error, image bits per pixel mismatch, both image bits per pixel must be the same " << std::endl;
            return false;
        }
        break;
        }
    }

    fipImage imageOut(FIT_BITMAP, width, height, maxBpp);
    switch (maxBpp)
    {
    case 8:
    {
        for (int y = 0; y < height; ++y)
        {
            BYTE* pPixelData1 = image1.getScanLine(y);
            BYTE* pPixelData2 = image2.getScanLine(y);
            BYTE* pPixelDataOut = imageOut.getScanLine(y);

            for (int x = 0; x < width; ++x)
            {
                pPixelDataOut[x] = pPixelData1[x] - pPixelData2[x];
            }
        }
    }
    break;
    case 16:
    {
        for (int y = 0; y < height; ++y)
        {
            spank::uint16* pPixelData1 = (spank::uint16*)image1.getScanLine(y);
            spank::uint16* pPixelData2 = (spank::uint16*)image2.getScanLine(y);
            spank::uint16* pPixelDataOut = (spank::uint16*)imageOut.getScanLine(y);

            for (int x = 0; x < width; ++x)
            {
                pPixelDataOut[x] = pPixelData1[x] - pPixelData2[x];
            }
        }
    }
    break;
    case 24:
    {
        for (int y = 0; y < height; ++y)
        {
            BYTE* pPixelData1 = image1.getScanLine(y);
            BYTE* pPixelData2 = image2.getScanLine(y);
            BYTE* pPixelDataOut = imageOut.getScanLine(y);

            for (int x = 0; x < width; ++x)
            {
                int index = 3 * x;
                pPixelDataOut[index] = pPixelData1[index] - pPixelData2[index];
                pPixelDataOut[index + 1] = pPixelData1[index + 1] - pPixelData2[index + 1];
                pPixelDataOut[index + 2] = pPixelData1[index + 2] - pPixelData2[index + 2];
            }
        }
    }
    break;
    case 32:
    {
        for (int y = 0; y < height; ++y)
        {
            spank::uint32* pPixelData1 = (spank::uint32*)image1.getScanLine(y);
            spank::uint32* pPixelData2 = (spank::uint32*)image2.getScanLine(y);
            spank::uint32* pPixelDataOut = (spank::uint32*)imageOut.getScanLine(y);

            for (int x = 0; x < width; ++x)
            {
                pPixelDataOut[x] = pPixelData1[x] - pPixelData2[x];
            }
        }
    }
    break;
    default:
        return false;
    }

    imageOut.save(strOutFile.c_str(), PNG_Z_NO_COMPRESSION);

    return true;
}

bool ImageTool::groupByFingerPrint(const spank::StringList& files,
                                   const spank::StringList& exts,
                                   const spank::tstring& outputType,
                                   const spank::tstring& commandFormat,
                                   int distance)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    double** iMatrix = new double*[IMAGE_SIZE];
    for (int i = 0; i < IMAGE_SIZE; ++i)
    {
        iMatrix[i] = new double[IMAGE_SIZE];
    }

    FileInfoLinkList fileInfoList;

    // collect file pHash
    for (const auto& file : fileSet)
    {
        FileInfo fileInfo;
        fileInfo.fileName = file;

        fipImage image;
        if (!image.load(file.c_str())) continue;

        fileInfo.width = image.getWidth();
        fileInfo.height = image.getHeight();

        image.rescale(IMAGE_SIZE, IMAGE_SIZE, FILTER_BILINEAR);
        image.convertToGrayscale();

        DCT(image, IMAGE_SIZE, iMatrix);
        double averagePix = calcAverage(iMatrix, 8);
        fingerPrint(iMatrix, 8, fileInfo.pHash, averagePix);

        fileInfoList.push_back(fileInfo);
    }

    for (int i = 0; i < IMAGE_SIZE; ++i)
    {
        delete[] iMatrix[i];
    }
    delete[] iMatrix;

    // group images by hash
    FileInfoGroupList fileInfoGroupList;
    while (!fileInfoList.empty())
    {
        FileInfoGroup currFileInfoGroup;

        currFileInfoGroup.fileList.push_back(fileInfoList.front());
        fileInfoList.pop_front();

        auto it = fileInfoList.begin();
        while (it != fileInfoList.end())
        {
            const auto& testFileInfo = *it;

            if (currFileInfoGroup.fileList[0].width == testFileInfo.width && currFileInfoGroup.fileList[0].height == testFileInfo.height
                && hammingDistance(currFileInfoGroup.fileList[0].pHash, testFileInfo.pHash) <= distance)
            {
                currFileInfoGroup.fileList.push_back(testFileInfo);
                it = fileInfoList.erase(it);
            }
            else
            {
                ++it;
            }
        }

        fileInfoGroupList.push_back(currFileInfoGroup);
        currFileInfoGroup.fileList.clear();
    }

    // print out group info
    printGroupInfo(fileInfoGroupList, outputType, commandFormat);

    return true;
}

bool ImageTool::groupByName(const spank::StringList& files,
                            const spank::StringList& exts,
                            const spank::tstring& outputType,
                            const spank::tstring& commandFormat,
                            int distance)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, exts);
    if (fileSet.empty()) return true;

    // collect file info
    FileInfoList fileInfoList;
    for (const auto& file : fileSet)
    {
        FileInfo fileInfo;
        fileInfo.fileName = file;

        fileInfoList.push_back(fileInfo);
    }

    // group images by name
    FileInfoGroupList fileInfoGroupList;
    if (!fileInfoList.empty())
    {
        FileInfoGroup currFileInfoGroup;

        currFileInfoGroup.fileList.push_back(fileInfoList[0]);
        int numFileInfo = static_cast<int>(fileInfoList.size());

        for (int i = 1; i < numFileInfo; ++i)
        {
            const auto& prevFileInfo = fileInfoList[i - 1];
            const auto& currFileInfo = fileInfoList[i];

            if (spank::StringUtil::stringHammingDistance(currFileInfo.fileName, prevFileInfo.fileName) <= distance)
            {
                currFileInfoGroup.fileList.push_back(currFileInfo);
            }
            else
            {
                fileInfoGroupList.push_back(currFileInfoGroup);
                currFileInfoGroup.fileList.clear();
                currFileInfoGroup.fileList.push_back(currFileInfo);
            }
        }

        if (!currFileInfoGroup.fileList.empty())
        {
            fileInfoGroupList.push_back(currFileInfoGroup);
            currFileInfoGroup.fileList.clear();
        }
    }

    // print out group info
    printGroupInfo(fileInfoGroupList, outputType, commandFormat);

    return true;
}

bool ImageTool::removeAlpha(const spank::StringList& files, const spank::tstring& reg, int threshold)
{
    spank::StringSet fileSet;
    collectFiles(fileSet, files, reg);
    if (fileSet.empty()) return true;

    for (const auto& file : fileSet)
    {
        fipImage image;
        if (!image.load(file.c_str()))
        {
            std::cout << "Error, load image failed: " << file << std::endl;
            continue;
        }

        if (image.getBitsPerPixel() <= 24)
        {
            std::cout << "Skip 24 bits image: " << file << std::endl;
            continue;
        }

        if (image.getBitsPerPixel() > 32)
        {
            std::cout << "Force convert to 32 bits image: " << file << std::endl;
            image.convertTo32Bits();
        }

        if (!isAlphaEqual(image, threshold))
        {
            std::cout << "Keep alpha image: " << file << std::endl;
            continue;
        }

        if (!image.convertTo24Bits())
        {
            std::cout << "Error, can not covert image to 24 bits: " << file << std::endl;
            continue;
        }

        if (!image.save(file.c_str()))
        {
            std::cout << "Error, can not save image file: " << file << std::endl;
            continue;
        }
        else
        {
            std::cout << "alpha removed: " << file << std::endl;
        }
    }

    return true;
}

void ImageTool::collectFiles(spank::StringSet& fileSet, const spank::StringList& files, const spank::StringList& exts)
{
    spank::StringSet extSet;
    for (const auto& ext : exts)
        extSet.insert(ext);

    for (const auto& file : files)
    {
        if (spank::FileUtil::isDirectory(file))
        {
            spank::FileUtil::listFilesInDirectory(fileSet, file, extSet);
        }
        else
        {
            fileSet.insert(file);
        }
    }
}

void ImageTool::collectFiles(spank::StringSet& fileSet, const spank::StringList& files, const spank::tstring& reg)
{
    std::regex rg(reg.c_str());

    for (const auto& file : files)
    {
        if (spank::FileUtil::isDirectory(file))
        {
            spank::FileUtil::listFilesInDirectory(fileSet, file, rg);
        }
        else
        {
            if (std::regex_match(file, rg))
            {
                fileSet.insert(file);
            }
        }
    }
}

const spank::tstring& ImageTool::getFilePath(TEXTURES eTexture)
{
    static const spank::tstring s_texFilePath[NUM_TEXTURES] = {
        "diffuse.png",
        "normal.png",
        "specular.png",
    };
    if (eTexture < TEX_DIFFUSE || eTexture >= NUM_TEXTURES) return spank::EMPTY_STRING;
    return s_texFilePath[eTexture];
}

void ImageTool::printGroupInfo(const FileInfoGroupList& fileInfoGroupList, const spank::tstring& outputType, const spank::tstring& commandFormat)
{
    for (const auto& groupInfo : fileInfoGroupList)
    {
        spank::tstring outputFile = "\"" + spank::FileUtil::replaceFileExtension(groupInfo.fileList[0].fileName, outputType) + "\"";

        spank::tstring fileNameList;
        for (const auto& fileInfo : groupInfo.fileList)
        {
            fileNameList += "\"";
            fileNameList += fileInfo.fileName;
            fileNameList += "\" ";
        }

        spank::tstring finalString = commandFormat;
        spank::StringUtil::replaceString(finalString, "{OUTPUT_FILE}", outputFile);
        spank::StringUtil::replaceString(finalString, "{FILE_NAME_LIST}", fileNameList);

        std::cout << finalString << std::endl;
    }
}

bool ImageTool::convertToType(fipImage& image, const spank::tstring& destFilePath, FREE_IMAGE_FORMAT dstFif, int depth)
{
    if (depth != -1 && depth != image.getBitsPerPixel())
    {
        switch (depth)
        {
        case 8:
        {
            if (!image.convertTo8Bits())
            {
                std::cout << "Error, convert image to 8 bits failed: " << std::endl;
                return false;
            }
        }
        break;
        case 16:
        {
            if (!image.convertTo16Bits565())
            {
                std::cout << "Error, convert image to 16 bits failed: " << std::endl;
                return false;
            }
        }
        break;
        case 24:
        {
            if (!image.convertTo24Bits())
            {
                std::cout << "Error, convert image to 24 bits failed: " << std::endl;
                return false;
            }
        }
        break;
        case 32:
        {
            if (!image.convertTo24Bits())
            {
                std::cout << "Error, convert image to 32 bits failed: " << std::endl;
                return false;
            }
        }
        break;
        default:
        {
            std::cout << "Error, unsupported depth: " << depth << std::endl;
            return false;
        }
        break;
        }
    }

    fipMemoryIO memIO;
    if (!image.saveToMemory(dstFif, memIO))
    {
        std::cout << "Error, save to memory failed: " << std::endl;
        return false;
    }

    if (!memIO.seek(0L, SEEK_SET))
    {
        std::cout << "Error, seek memory failed: " << std::endl;
        return false;
    }

    if (!image.loadFromMemory(memIO))
    {
        std::cout << "Error, load from memory failed: " << std::endl;
        return false;
    }

    if (!image.save(destFilePath.c_str()))
    {
        std::cout << "Error, save image failed: " << destFilePath << std::endl;
        return false;
    }

    return true;
}

void ImageTool::DCT(const fipImage& image, const int& n, double** iMatrix)
{
    for (int i = 0; i < n; ++i)
    {
        BYTE* pPixelData = image.getScanLine(i);
        for (int j = 0; j < n; ++j)
        {
            iMatrix[i][j] = (double)pPixelData[j];
        }
    }

    double** quotient = new double*[n];
    double** quotientT = new double*[n];
    double** tmp = new double*[n];
    for (int i = 0; i < n; i++)
    {
        quotient[i] = new double[n];
        quotientT[i] = new double[n];
        tmp[i] = new double[n];
    }

    coefficient(n, quotient, quotientT);
    matrixMultiply(quotient, iMatrix, n, tmp);
    matrixMultiply(tmp, quotientT, n, iMatrix);

    for (int i = 0; i < n; i++)
    {
        delete[] tmp[i];
        delete[] quotient[i];
        delete[] quotientT[i];
    }
    delete[] tmp;
    delete[] quotient;
    delete[] quotientT;
}

void ImageTool::coefficient(const int& n, double** quotient, double** quotientT)
{
    double sqr = 1.0 / sqrt(n + 0.0);
    for (int i = 0; i < n; i++)
    {
        quotient[0][i] = sqr;
        quotientT[i][0] = sqr;
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            quotient[i][j] = sqrt(2.0 / n) * cos(i * (j + 0.5) * 3.1415926 / n);
            quotientT[j][i] = quotient[i][j];
        }
    }
}

void ImageTool::matrixMultiply(double** A, double** B, int n, double** result)
{
    double t = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            t = 0;
            for (int k = 0; k < n; k++)
            {
                t += A[i][k] * B[k][j];
            }
            result[i][j] = t;
        }
    }
}

double ImageTool::calcAverage(double** iMatrix, int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum += iMatrix[i][j];
        }
    }
    return sum / (size * size);
}

void ImageTool::fingerPrint(double** iMatrix, int size, std::bitset<HASH_LENGTH>& phash, double averagePix)
{
    for (int i = 0; i < size; i++)
    {
        int pos = i * size;
        for (int j = 0; j < size; j++)
        {
            phash[pos + j] = iMatrix[i][j] >= averagePix ? 1 : 0;
        }
    }
}

int ImageTool::hammingDistance(const std::bitset<HASH_LENGTH>& query, const std::bitset<HASH_LENGTH>& target)
{
    int distance = 0;
    for (int i = 0; i < HASH_LENGTH; i++)
    {
        distance += (query[i] == target[i] ? 0 : 1);
    }
    return distance;
}

spank::tstring ImageTool::bitTohex(const std::bitset<HASH_LENGTH>& target)
{
    spank::tstring str;
    for (int i = 0; i < HASH_LENGTH; i = i + 4)
    {
        int sum = target[i] + (target[i + 1] << 1) + (target[i + 2] << 2) + (target[i + 3] << 3);

        std::stringstream ss;
        ss << std::hex << sum;

        spank::tstring s;
        ss >> s;

        str += s;
    }

    return str;
}

bool ImageTool::getTrimedImageInfo(PackingUtil::PieceFileInfo& fileInfo, const fipImage& image)
{
    if (image.getBitsPerPixel() != 32) return false;

    fileInfo.srcWidth = image.getWidth();
    fileInfo.srcHeight = image.getHeight();

    // the image original is at bottom left
    BYTE* pixel = image.accessPixels();

    // detect top
    int top = fileInfo.srcHeight - 1;
    for (top = fileInfo.srcHeight - 1; top > 0; --top)
    {
        if (!isEmptyRow(top, fileInfo.srcWidth, fileInfo.srcHeight, pixel)) break;
    }

    // detect bottom
    int bottom = 0;
    for (bottom = 0; bottom < top; ++bottom)
    {
        if (!isEmptyRow(bottom, fileInfo.srcWidth, fileInfo.srcHeight, pixel)) break;
    }

    // detect right
    int right = fileInfo.srcWidth - 1;
    for (right = fileInfo.srcWidth - 1; right > 0; --right)
    {
        if (!isEmptyCol(right, fileInfo.srcWidth, fileInfo.srcHeight, pixel)) break;
    }

    // detect left
    int left = 0;
    for (left = 0; left < right; ++left)
    {
        if (!isEmptyCol(left, fileInfo.srcWidth, fileInfo.srcHeight, pixel)) break;
    }

    fileInfo.width = (right - left + 1);
    fileInfo.height = (top - bottom + 1);

    fileInfo.offsetX = left;
    fileInfo.offsetY = bottom;

    return true;
}

bool ImageTool::isEmptyRow(int y, int width, int height, BYTE* pixel)
{
    int rowSize = (width * 32 + 7) / 8;
    for (int x = 0; x < width; ++x)
    {
        BYTE* bits = pixel + y * rowSize + 4 * x;
        if (bits[FI_RGBA_ALPHA] != 0) return false;
    }

    return true;
}

bool ImageTool::isEmptyCol(int x, int width, int height, BYTE* pixel)
{
    int rowSize = (width * 32 + 7) / 8;
    for (int y = 0; y < height; ++y)
    {
        BYTE* bits = pixel + y * rowSize + 4 * x;
        if (bits[FI_RGBA_ALPHA] != 0) return false;
    }

    return true;
}

bool ImageTool::isAlphaEqual(fipImage& image, int threshold)
{
    int width = image.getWidth();
    int height = image.getHeight();

    int* pPixelData = (int*)image.getScanLine(0);
    int firstAlpha = *(int*)&pPixelData[0];
    firstAlpha = (firstAlpha >> 24) & 0x000000FF;

    for (int y = 0; y < height; ++y)
    {
        pPixelData = (int*)image.getScanLine(y);
        for (int x = 0; x < width; ++x)
        {
            int alpha = *(int*)&pPixelData[x];
            alpha = (alpha >> 24) & 0x000000FF;
            if (std::abs(alpha - firstAlpha) > threshold)
            {
                return false;
            }
        }
    }

    return true;
}

void ImageTool::saveAtlasInfoXml(const spank::tstring& id,
                                 const spank::tstring& imageFile,
                                 const spank::tstring& configFile,
                                 int width,
                                 int height,
                                 const PackingUtil::PieceFileInfoMap& pieceFileInfoMap)
{
    tinyxml2::XMLDocument doc;

    tinyxml2::XMLElement* pXmlImagePiece = doc.NewElement("ImagePiece");
    doc.LinkEndChild(pXmlImagePiece);

    tinyxml2::XMLElement* pXmlImageList = doc.NewElement("ImageList");
    pXmlImagePiece->LinkEndChild(pXmlImageList);

    tinyxml2::XMLElement* pXmlPieceList = doc.NewElement("PieceList");
    pXmlImagePiece->LinkEndChild(pXmlPieceList);

    tinyxml2::XMLElement* pXmlImage = doc.NewElement("Image");
    pXmlImageList->LinkEndChild(pXmlImage);

    pXmlImage->SetAttribute("id", id.c_str());
    pXmlImage->SetAttribute("file_name", imageFile.c_str());
    pXmlImage->SetAttribute("width", width);
    pXmlImage->SetAttribute("height", height);

    for (auto it : pieceFileInfoMap)
    {
        PackingUtil::PieceFileInfo& fileInfo = it.second;

        tinyxml2::XMLElement* pXmlPiece = doc.NewElement("Piece");
        pXmlPieceList->LinkEndChild(pXmlPiece);

        pXmlPiece->SetAttribute("id", fileInfo.strId.c_str());
        pXmlPiece->SetAttribute("image_id", id.c_str());
        pXmlPiece->SetAttribute("x", fileInfo.x);
        pXmlPiece->SetAttribute("y", fileInfo.y);
        pXmlPiece->SetAttribute("width", fileInfo.width);
        pXmlPiece->SetAttribute("height", fileInfo.height);

        pXmlPiece->SetAttribute("offsetX", fileInfo.offsetX);
        pXmlPiece->SetAttribute("offsetY", fileInfo.offsetY);
        pXmlPiece->SetAttribute("srcWidth", fileInfo.srcWidth);
        pXmlPiece->SetAttribute("srcHeight", fileInfo.srcHeight);
    }

    doc.SaveFile(configFile.c_str());
}

void ImageTool::saveAtlasInfoCpp(const spank::tstring& strOutDir,
                                 const spank::tstring& className,
                                 const spank::tstring& imageType,
                                 float width,
                                 float height,
                                 const PackingUtil::PieceFileInfoMap& pieceFileInfoMap)
{
    spank::tstring strPieceList;

    for (auto it : pieceFileInfoMap)
    {
        PackingUtil::PieceFileInfo& fileInfo = it.second;

        glm::vec2 lb(fileInfo.x / width, (height - fileInfo.y - fileInfo.height) / height);
        glm::vec2 rt((fileInfo.x + fileInfo.width) / width, (height - fileInfo.y) / height);

        auto strPieceListItem = fmt::format("        {{ \"{}\", {{ {}, {} }}, {{ {}, {} }}, {{ {}, {} }}, {{ {{ {:.03f}f, {:.03f}f }}, {{ {:.03f}f, {:.03f}f "
                                            "}}, {{ {:.03f}f, {:.03f}f }}, {{ {:.03f}f, {:.03f}f }} }} }},\n",
                                            fileInfo.strId,
                                            fileInfo.srcWidth,
                                            fileInfo.srcHeight,
                                            fileInfo.width,
                                            fileInfo.height,
                                            fileInfo.offsetX,
                                            fileInfo.offsetY,
                                            lb.x,
                                            lb.y,
                                            rt.x,
                                            lb.y,
                                            lb.x,
                                            rt.y,
                                            rt.x,
                                            rt.y);

        strPieceList += strPieceListItem;
    }

    spank::tstring headerFileContent = ATLAS_H_TEMPLATE;
    spank::tstring implementFileContent = ATLAS_CPP_TEMPLATE;

    spank::tstring outHFile = strOutDir + "/" + className + ".h";
    spank::tstring outCppFile = strOutDir + "/" + className + ".cpp";

    spank::StringUtil::replaceString(headerFileContent, "[CLASS_NAME]", className);

    spank::StringUtil::replaceString(implementFileContent, "[CLASS_NAME]", className);
    spank::StringUtil::replaceString(implementFileContent, "[NUM_PIECES]", fmt::format("{}", pieceFileInfoMap.size()));
    spank::StringUtil::replaceString(implementFileContent, "[IMAGE_EXT]", imageType);
    spank::StringUtil::replaceString(implementFileContent, "[PIECE_LISTS]", strPieceList);

    spank::FileUtil::writeStringIntoFile(headerFileContent, outHFile);
    spank::FileUtil::writeStringIntoFile(implementFileContent, outCppFile);
}
