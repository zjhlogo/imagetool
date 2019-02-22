/*!
 * \file PackingUtil.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date Ê®ÔÂ 2015
 *
 * 
 */
#include "PackingUtil.h"
#include <format.h>

bool PackingUtil::pack(int& widthOut, int& heightOut, PieceFileInfoMap& pieceFileInfoMap, const spank::tstring& strFileOut, int packingFlag/* = PACKING_FLAG_MAX_QUALITY */)
{
	// generate repack info
	int finalWidth = 0;
	int finalHeight = 0;
	if (!generateRepackInfo(pieceFileInfoMap, finalWidth, finalHeight, packingFlag)) return false;

	// create new image
    if (packingFlag & PACKING_FLAG_VERBOSE)
    {
        if (m_progressCb) m_progressCb("start pasting subimages...");
    }
	fipImage image(FIT_BITMAP, finalWidth, finalHeight, 32);
	if (!repackImage(image, pieceFileInfoMap, packingFlag)) return false;

	fipImage finalImage;
	image.copySubImage(finalImage, GAP, GAP, finalWidth - GAP, finalHeight - GAP);

	for (auto& it : pieceFileInfoMap)
	{
		PieceFileInfo& pieceFileInfo = it.second;
		pieceFileInfo.x -= GAP;
		pieceFileInfo.y -= GAP;
	}

	finalImage.save(strFileOut.c_str());

	widthOut = finalWidth - GAP * 2;
	heightOut = finalHeight - GAP * 2;
	return true;
}

bool PackingUtil::generateRepackInfo(PieceFileInfoMap& pieceFileInfoMap, int& finalWidth, int& finalHeight, int packingFlag)
{
	// collect rect size list
	float totalArea = 0;
	std::vector<rbp::RectSize> rectList;
	for (const auto& it : pieceFileInfoMap)
	{
		const PieceFileInfo& fileInfo = it.second;
		rbp::RectSize rectSize;
		rectSize.width = (fileInfo.width + GAP * 2);
		rectSize.height = (fileInfo.height + GAP * 2);
		totalArea += (rectSize.width * rectSize.height);
		rectList.push_back(rectSize);
	}

	// calculate the best width and height for packing
	spank::uint bestWidth = 1;
	spank::uint bestHeight = 1;
	while ((bestWidth + GAP * 2) * (bestHeight + GAP * 2) < totalArea)
	{
		if (bestHeight < bestWidth)
		{
			bestHeight = bestHeight << 1;
		}
		else
		{
			bestWidth = bestWidth << 1;
		}

		if (bestWidth <= 0 || bestHeight <= 0) return false;
	}

	// start packing
	bool bSuccess = false;
	std::map<int, rbp::Rect> rectFinal;
	float occupancyFinal = 1.0f;

	while (!bSuccess)
	{
        int rangeBegin = rbp::MaxRectsBinPack::RectBestShortSideFit;
        int rangeEnd = (packingFlag & PACKING_FLAG_MAX_QUALITY) ? rbp::MaxRectsBinPack::RectContactPointRule : rbp::MaxRectsBinPack::RectBestShortSideFit;

		for (int i = rangeBegin; i <= rangeEnd; ++i)
		{
			rbp::MaxRectsBinPack::FreeRectChoiceHeuristic method = static_cast<rbp::MaxRectsBinPack::FreeRectChoiceHeuristic>(i);
			std::map<int, rbp::Rect> rectResult;
			rbp::MaxRectsBinPack packer(bestWidth + GAP * 2, bestHeight + GAP * 2);

            packer.enableRotate((packingFlag & PACKING_FLAG_ROTATE) == PACKING_FLAG_ROTATE);
            if (packingFlag & PACKING_FLAG_VERBOSE)
            {
                packer.setProgressCallback(m_progressCb);
            }

			if (packer.Insert(rectList, rectResult, method))
			{
				float occupancy = packer.Occupancy();
				if (occupancyFinal > occupancy)
				{
					rectFinal = rectResult;
					occupancyFinal = occupancy;
				}

				bSuccess = true;
			}
		}

		if (!bSuccess)
		{
			if (bestHeight < bestWidth)
			{
				bestHeight = bestHeight << 1;
			}
			else
			{
				bestWidth = bestWidth << 1;
			}

			if (bestWidth <= 0 || bestHeight <= 0) return false;
		}
	}

	if (!bSuccess) return false;

	// save final result
	int index = 0;
	for (auto& it : pieceFileInfoMap)
	{
		PieceFileInfo& fileInfo = it.second;
		const rbp::Rect& packedRect = rectFinal[index];

		fileInfo.rotated = (packedRect.width - GAP * 2 != fileInfo.width);
		fileInfo.x = packedRect.x + GAP;
		fileInfo.y = packedRect.y + GAP;
		fileInfo.width = packedRect.width - GAP * 2;
		fileInfo.height = packedRect.height - GAP * 2;

		++index;
	}

	finalWidth = bestWidth + GAP * 2;
	finalHeight = bestHeight + GAP * 2;

	return bSuccess;
}

bool PackingUtil::repackImage(fipImage& imageOut, const PieceFileInfoMap& pieceFileInfoMap, int packingFlag)
{
	for (const auto& it : pieceFileInfoMap)
	{
		const PieceFileInfo& fileInfo = it.second;

		fipImage subImage;

        if (packingFlag & PACKING_FLAG_TRIM_WHITE_SPACE_BORDER)
        {
            fipImage srcImage;
            srcImage.load(fileInfo.path.c_str());

            int width = srcImage.getWidth();
            int height = srcImage.getHeight();

            srcImage.copySubImage(subImage, fileInfo.offsetX, height - (fileInfo.offsetY + fileInfo.height), fileInfo.offsetX + fileInfo.width, height - fileInfo.offsetY);
        }
        else
        {
            subImage.load(fileInfo.path.c_str());
        }

		if (fileInfo.rotated)
		{
			subImage.rotate(90.0);
		}

		fipImage subImageWithBoundary;
		createProtectColorBoundary(subImageWithBoundary, subImage);

		imageOut.pasteSubImage(subImageWithBoundary, fileInfo.x - GAP, fileInfo.y - GAP);

        if (packingFlag & PACKING_FLAG_VERBOSE)
        {
            if (m_progressCb) m_progressCb(fmt::format("image pasted: {0}", fileInfo.path));
        }
	}

	return true;
}

bool PackingUtil::createProtectColorBoundary(fipImage& imageOut, fipImage& imageIn)
{
	int width = imageIn.getWidth();
	int height = imageIn.getHeight();

	imageOut.setSize(FIT_BITMAP, width + GAP * 2, height + GAP * 2, 32);
	imageOut.pasteSubImage(imageIn, GAP, GAP);

	for (int x = 0; x < width; ++x)
	{
		RGBQUAD colorFinal;
		imageIn.getPixelColor(x, 0, &colorFinal);
		imageOut.setPixelColor(x + GAP, 0, &colorFinal);

		imageIn.getPixelColor(x, height - 1, &colorFinal);
		imageOut.setPixelColor(x + GAP, height + 1, &colorFinal);
	}

	for (int y = 0; y < height; ++y)
	{
		RGBQUAD colorFinal;
		imageIn.getPixelColor(0, y, &colorFinal);
		imageOut.setPixelColor(0, y + GAP, &colorFinal);

		imageIn.getPixelColor(width - 1, y, &colorFinal);
		imageOut.setPixelColor(width + 1, y + GAP, &colorFinal);
	}

	RGBQUAD colorFinal;
	imageIn.getPixelColor(0, 0, &colorFinal);
	imageOut.setPixelColor(0, 0, &colorFinal);

	imageIn.getPixelColor(0, height - 1, &colorFinal);
	imageOut.setPixelColor(0, height + 1, &colorFinal);

	imageIn.getPixelColor(width - 1, 0, &colorFinal);
	imageOut.setPixelColor(width + 1, 0, &colorFinal);

	imageIn.getPixelColor(width - 1, height - 1, &colorFinal);
	imageOut.setPixelColor(width + 1, height + 1, &colorFinal);

	return true;
}
