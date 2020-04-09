/*!
 * \file FieldAttributes.cpp
 *
 * \author zjhlogo zjhlogo@gmail.com
 * \date 04/14/2016
 *
 *
 */
#include "FieldAttributes.h"

#include <limits>

NS_BEGIN

const IFieldAttribute FFA_DISABLED(IFieldAttribute::ATTR_DISABLED);
const IntegerFieldAttribute FFA_DEFAULT_INTEGER(0, 0, 1);
const IntegerFieldAttribute FFA_DEFAULT_INTEGER_UNEXPAND(0, 0, 1, true, false, 0);
const IntegerFieldAttribute FFA_DEFAULT_INTEGER_1_100_1(1, 100, 1);
const FloatFieldAttribute FFA_DEFAULT_FLOAT(0.0, 0.0, 1.0);

const IntegerFieldAttribute FFA_POSITIVE_INTEGER(0, std::numeric_limits<int>::max(), 1);
const FloatFieldAttribute FFA_FLOAT_STEP_01(0.0, 0.0, 0.1);
const FloatFieldAttribute FFA_FLOAT_STEP_001(0.0, 0.0, 0.01);
const FloatFieldAttribute FFA_DEFAULT_SCALE(0.0, 100.0, 0.01);

const IFieldAttribute FFA_PIECE_INFO_LIST_LOCAL(0, IFieldAttribute::SubType::PieceInfoListLocal);
const IFieldAttribute FFA_PIECE_INFO_LIST_GLOBAL(0, IFieldAttribute::SubType::PieceInfoListGlobal);
const IFieldAttribute FFA_COLOR_INFO_LIST_LOCAL(0, IFieldAttribute::SubType::ColorInfoListLocal);
const IFieldAttribute FFA_COLOR_INFO_LIST_GLOBAL(0, IFieldAttribute::SubType::ColorInfoListGlobal);
const IFieldAttribute FFA_STYLE_LIST_LOCAL(0, IFieldAttribute::SubType::StyleListLocal);
const IFieldAttribute FFA_STYLE_LIST_GLOBAL(0, IFieldAttribute::SubType::StyleListGlobal);

NS_END
