/*!
 * \file PackingUtil.h
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date Ê®ÔÂ 2015
 *
 * 
 */
#pragma once
#include <FileUtil.h>
#include <FreeImagePlus/FreeImagePlus.h>
#include <map>
#include "MaxRectsBinPack.h"

class PackingUtil
{
public:
	enum CONST_DEFINE
	{
		GAP = 1,
	};

    static const int PACKING_FLAG_MAX_QUALITY = 1 << 0;
    static const int PACKING_FLAG_TRIM_WHITE_SPACE_BORDER = 1 << 1;
    static const int PACKING_FLAG_ROTATE = 1 << 2;
    static const int PACKING_FLAG_VERBOSE = 1 << 3;

	class PieceFileInfo
	{
	public:
		spank::tstring strId;
		spank::tstring path;
		bool rotated{};

		int x{};
		int y{};
		int width{};
		int height{};

        int offsetX{};
        int offsetY{};
        int srcWidth{};
        int srcHeight{};
	};

	typedef std::map<spank::tstring, PieceFileInfo> PieceFileInfoMap;

public:
	bool pack(int& widthOut, int& heightOut, PieceFileInfoMap& pieceFileInfoMap, const spank::tstring& strFileOut, int packingFlag = PACKING_FLAG_MAX_QUALITY);
    void setProgressCallback(rbp::MaxRectsBinPack::ProgressCallback cb) { m_progressCb = cb; };

private:
	bool generateRepackInfo(PieceFileInfoMap& pieceFileInfoMap, int& finalWidth, int& finalHeight, int packingFlag);
	bool repackImage(fipImage& imageOut, const PieceFileInfoMap& pieceFileInfoMap, int packingFlag);
	bool createProtectColorBoundary(fipImage& imageOut, fipImage& imageIn);

private:
    rbp::MaxRectsBinPack::ProgressCallback m_progressCb;

};
