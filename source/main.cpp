/*!
 * \file main.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 四月 2015
 *
 *
 */
#include "ImageTool.h"

#include <tclap/CmdLine.h>

FREE_IMAGE_COLOR_CHANNEL getChannelFromString(const spank::tstring& channel)
{
    if (channel == "r")
        return FICC_RED;
    else if (channel == "g")
        return FICC_GREEN;
    else if (channel == "b")
        return FICC_BLUE;
    return FICC_ALPHA;
}

int main(int argc, char** argv)
{
    TCLAP::CmdLine cmd("image tools", ' ', "1.0.1");

    // add input file arg
    TCLAP::MultiArg<spank::tstring> inputFileArg("i", "input", "input file can be a single file or directory, depend by specific switch", true, "path");
    cmd.add(inputFileArg);

    // additional input file arg
    TCLAP::MultiArg<spank::tstring> input2FileArg("", "input2", "input file can be a single file or directory, depend by specific switch", false, "path");
    cmd.add(input2FileArg);

    // add output file arg
    TCLAP::MultiArg<spank::tstring> outputFileArg("o", "output", "output file can be a single file or directory, depend by specific switch", false, "path");
    cmd.add(outputFileArg);

    // add ext arg
    TCLAP::MultiArg<spank::tstring> extsArg("e", "ext", "extension of files", false, "string");
    cmd.add(extsArg);

    // add percent
    TCLAP::ValueArg<int> percentArg("", "percent", "resize image in percent", false, 50, "integer");
    cmd.add(percentArg);

    // add width
    TCLAP::ValueArg<int> widthArg("", "width", "image width in pixel", false, 128, "integer");
    cmd.add(widthArg);

    // add height
    TCLAP::ValueArg<int> heightArg("", "height", "image height in pixel", false, 128, "integer");
    cmd.add(heightArg);

    // add convert type
    TCLAP::ValueArg<spank::tstring> typeArg("t", "type", "convert file destination type", false, "png", "type");
    cmd.add(typeArg);

    // add channel
    std::vector<std::string> allowedChannel;
    allowedChannel.push_back("r");
    allowedChannel.push_back("g");
    allowedChannel.push_back("b");
    allowedChannel.push_back("a");
    TCLAP::ValuesConstraint<std::string> constChannelValues(allowedChannel);
    TCLAP::ValueArg<spank::tstring> channelArg("c", "channel", "image file channel, must be one of the r|g|b|a", false, "a", &constChannelValues);
    cmd.add(channelArg);

    // add addition channel
    TCLAP::ValueArg<spank::tstring> channel2Arg("", "channel2", "image file channel, must be one of the r|g|b|a", false, "a", &constChannelValues);
    cmd.add(channel2Arg);

    // add depth
    std::vector<int> allowedDepth;
    allowedDepth.push_back(-1);
    allowedDepth.push_back(8);
    allowedDepth.push_back(16);
    allowedDepth.push_back(24);
    allowedDepth.push_back(32);
    TCLAP::ValuesConstraint<int> constDepthValues(allowedDepth);
    TCLAP::ValueArg<int> depthArg("", "depth", "convert image into depth, must be one of the -1|8|16|24|32", false, -1, &constDepthValues);
    cmd.add(depthArg);

    // add rotate switch
    TCLAP::SwitchArg rotateSwitch("", "rotate", "rotate the images");
    cmd.add(rotateSwitch);

    // add trim switch
    TCLAP::SwitchArg trimSwitch("", "trim", "trim the image, remove all the border with white space");
    cmd.add(trimSwitch);

    // add delete src file
    TCLAP::SwitchArg deleteSrcSwitch("", "deletesrc", "delete source file");
    cmd.add(deleteSrcSwitch);

    // add rows
    TCLAP::ValueArg<int> rowsArg("", "row", "number of rows in the image", false, 1, "integer");
    cmd.add(rowsArg);

    // add cols
    TCLAP::ValueArg<int> colsArg("", "col", "number of cols in the image", false, 1, "integer");
    cmd.add(colsArg);

    // add format
    TCLAP::ValueArg<spank::tstring> formatArg("f", "format", "format of the output", false, "", "string");
    cmd.add(formatArg);

    // add distance
    TCLAP::ValueArg<int> distArg("d", "distance", "the hamming distance between compared image", false, 1, "integer distance");
    cmd.add(distArg);

    // add threshold
    TCLAP::ValueArg<int> thresholdArg("", "threshold", "the threshold", false, 1, "integer");
    cmd.add(thresholdArg);

    // add regex search
    TCLAP::ValueArg<spank::tstring> regexArg("", "regex", "the regex to filter file list", false, "", "string");
    cmd.add(regexArg);

    // add convention type
    // clang-format off
    TCLAP::SwitchArg resizeSwitch("", "resize", "resize image, example: --resize -i dir -e png --width 1024 --height 1024; or --resize -i dir -e png --percent 50");
    TCLAP::SwitchArg stretchSwitch("", "stretch", "stretch image, example: --stretch -i dir -e png --width 1024 --height 512");
    TCLAP::SwitchArg cropSwitch("", "crop", "crop image, example: --crop -i dir -e png --width 128 --height 128");
    TCLAP::SwitchArg convertSwitch("", "convert", "convert image, example: --convert -i dir -e bmp -t png");
    TCLAP::SwitchArg convertBlenderSwitch("", "convert-blender", "convert image, example: --convert-blender -i dir -e dat -t png");
    TCLAP::SwitchArg optiPngSwitch("", "optipng", "optimize png file size, example: --optipng -i dir");
    TCLAP::SwitchArg replaceSwitch("", "replace", "replace a file channel, the parameter -c can be [r|g|b|a] example: --replace --input src.png --input2 channel.png --channel a --channel2 a -o out.png");
    TCLAP::SwitchArg binPackSwitch("", "binpack", "binary packing images into a big image, example: --binpack -i dir -e png -t png -o out.atlas --rotate --trim");
    TCLAP::SwitchArg advBinPackSwitch("", "advbinpack", "advance binary packing images, example: --advbinpack -i images.xml -o out.xml");
    TCLAP::SwitchArg detectRedSwitch("", "detectred", "detect red pixel position in images, example: --detectred -i dir -e png -o out.xml");
    TCLAP::SwitchArg splitSwitch("", "split", "split texture to sub textures, example: --split -i dir -e png -t png --row 10 --col 10 -o dir or --split -i dir -e png -t png --width 800 --height 600 -o dir");
    TCLAP::SwitchArg subtractSwitch("", "subtract", "subtract one image color from another image by pixel and generate difference image, example(image1-image2=out): --subtract -i image1.png --input2 image2.png -o out.png");
    TCLAP::SwitchArg groupByNameSwitch("", "groupbyname", "group images by filename, example: --groupbyname -i dir -e png -f \"img2webp -mixed -d 1000 -q 90 -m 6 -mt -v -o {FILE_DIR}{FILE_COMMON_NAME}.webp -f {FILE_NAME_LIST}\" -d 1");
    TCLAP::SwitchArg groupByFingerprintSwitch("", "groupbyfp", "group images by finerprint, example: --groupbyfp -i dir -e png -f \"img2webp -mixed -d 1000 -q 90 -m 6 -mt -v -o {FILE_DIR}{FILE_COMMON_NAME}.webp -f {FILE_NAME_LIST}\" -d 1");
    TCLAP::SwitchArg removeAlphaSwitch("", "removealpha", "remove alpha channel, example: --removealpha -i dir -e png --threshold 32");
    // clang-format on

    std::vector<TCLAP::Arg*> xorList;
    xorList.push_back(&resizeSwitch);
    xorList.push_back(&stretchSwitch);
    xorList.push_back(&cropSwitch);
    xorList.push_back(&convertSwitch);
    xorList.push_back(&convertBlenderSwitch);
    xorList.push_back(&optiPngSwitch);
    xorList.push_back(&replaceSwitch);
    xorList.push_back(&binPackSwitch);
    xorList.push_back(&advBinPackSwitch);
    xorList.push_back(&detectRedSwitch);
    xorList.push_back(&splitSwitch);
    xorList.push_back(&subtractSwitch);
    xorList.push_back(&groupByNameSwitch);
    xorList.push_back(&groupByFingerprintSwitch);
    xorList.push_back(&removeAlphaSwitch);

    cmd.xorAdd(xorList);

    cmd.parse(argc, argv);

    FreeImage_Initialise();

    ImageTool tool;
    if (resizeSwitch.isSet())
    {
        if (widthArg.isSet() && heightArg.isSet())
        {
            tool.resizeImageByPixel(inputFileArg.getValue(), extsArg.getValue(), widthArg.getValue(), heightArg.getValue());
        }
        else if (percentArg.isSet())
        {
            tool.resizeImageByPercent(inputFileArg.getValue(), extsArg.getValue(), percentArg.getValue() / 100.0f);
        }
    }
    else if (stretchSwitch.isSet())
    {
        if (widthArg.isSet() && heightArg.isSet())
        {
            tool.stretchImageByPixel(inputFileArg.getValue(), extsArg.getValue(), widthArg.getValue(), heightArg.getValue());
        }
    }
    else if (cropSwitch.isSet() && widthArg.isSet() && heightArg.isSet())
    {
        tool.cropImage(inputFileArg.getValue(), extsArg.getValue(), widthArg.getValue(), heightArg.getValue());
    }
    else if (convertSwitch.isSet() && typeArg.isSet())
    {
        tool.convert(inputFileArg.getValue(), extsArg.getValue(), typeArg.getValue(), depthArg.getValue(), deleteSrcSwitch.isSet());
    }
    else if (convertBlenderSwitch.isSet() && typeArg.isSet())
    {
        tool.convertBlenderIconFile(inputFileArg.getValue(), extsArg.getValue(), typeArg.getValue(), depthArg.getValue(), deleteSrcSwitch.isSet());
    }
    else if (optiPngSwitch.isSet())
    {
        tool.optiPng(inputFileArg.getValue());
    }
    else if (replaceSwitch.isSet() && inputFileArg.isSet() && input2FileArg.isSet() && channelArg.isSet() && channel2Arg.isSet() && outputFileArg.isSet())
    {
        if (inputFileArg.getValue().size() != 1 || input2FileArg.getValue().size() != 1) return 1;
        if (outputFileArg.getValue().size() != 1) return 1;

        FREE_IMAGE_COLOR_CHANNEL eChannel1 = getChannelFromString(channelArg.getValue());
        FREE_IMAGE_COLOR_CHANNEL eChannel2 = getChannelFromString(channel2Arg.getValue());

        tool.replaceChannel(inputFileArg.getValue()[0], eChannel1, input2FileArg.getValue()[0], eChannel2, outputFileArg.getValue()[0]);
    }
    else if (binPackSwitch.isSet() && inputFileArg.isSet() && outputFileArg.isSet())
    {
        int packingFlag = PackingUtil::PACKING_FLAG_VERBOSE;
        if (rotateSwitch.isSet()) packingFlag |= PackingUtil::PACKING_FLAG_ROTATE;
        if (trimSwitch.isSet()) packingFlag |= PackingUtil::PACKING_FLAG_TRIM_WHITE_SPACE_BORDER;

        tool.binPack(inputFileArg.getValue(), extsArg.getValue(), typeArg.getValue(), outputFileArg.getValue()[0], packingFlag);
    }
    else if (advBinPackSwitch.isSet() && inputFileArg.isSet() && outputFileArg.isSet())
    {
        tool.advBinPack(inputFileArg.getValue()[0], outputFileArg.getValue()[0]);
    }
    else if (detectRedSwitch.isSet() && inputFileArg.isSet() && outputFileArg.isSet())
    {
        tool.detectRed(inputFileArg.getValue(), extsArg.getValue(), outputFileArg.getValue()[0]);
    }
    else if (splitSwitch.isSet() && inputFileArg.isSet() && typeArg.isSet() && outputFileArg.isSet())
    {
        if (rowsArg.isSet() && colsArg.isSet())
        {
            tool.splitByCount(inputFileArg.getValue(),
                              extsArg.getValue(),
                              rowsArg.getValue(),
                              colsArg.getValue(),
                              typeArg.getValue(),
                              outputFileArg.getValue()[0]);
        }
        else if (widthArg.isSet() && heightArg.isSet())
        {
            tool.splitBySize(inputFileArg.getValue(),
                             extsArg.getValue(),
                             widthArg.getValue(),
                             heightArg.getValue(),
                             typeArg.getValue(),
                             outputFileArg.getValue()[0]);
        }
    }
    else if (subtractSwitch.isSet() && inputFileArg.isSet() && input2FileArg.isSet() && outputFileArg.isSet())
    {
        tool.subtract(inputFileArg.getValue()[0], input2FileArg.getValue()[0], outputFileArg.getValue()[0]);
    }
    else if (groupByNameSwitch.isSet() && inputFileArg.isSet() && extsArg.isSet() && typeArg.isSet() && formatArg.isSet())
    {
        tool.groupByName(inputFileArg.getValue(), extsArg.getValue(), typeArg.getValue(), formatArg.getValue(), distArg.getValue());
    }
    else if (groupByFingerprintSwitch.isSet() && inputFileArg.isSet() && extsArg.isSet() && typeArg.isSet() && formatArg.isSet())
    {
        tool.groupByFingerPrint(inputFileArg.getValue(), extsArg.getValue(), typeArg.getValue(), formatArg.getValue(), distArg.getValue());
    }
    else if (removeAlphaSwitch.isSet() && inputFileArg.isSet() && regexArg.isSet())
    {
        int threshold = 0;
        if (thresholdArg.isSet()) threshold = thresholdArg.getValue();
        tool.removeAlpha(inputFileArg.getValue(), regexArg.getValue(), threshold);
    }

    FreeImage_DeInitialise();

    return 0;
}
