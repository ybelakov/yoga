/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <yoga/Yoga.h>

static YGSize _measureTwoLines(
    YGNodeConstRef /*node*/,
    float /*width*/,
    YGMeasureMode /*widthMode*/,
    float /*height*/,
    YGMeasureMode /*heightMode*/) {
  return YGSize{292.0f, 44.0f};
}

// A measured (text) node far from the origin on a 3x screen, with its top
// below 2048 and its bottom above. Each edge is rounded to the pixel grid and
// the height is their difference; in float the two edges have different
// precision and the height came out 43.9998779 instead of 44, one step short
// of the two lines the node was measured at.
TEST(YogaTest, rounding_measured_height_far_from_origin_is_exact) {
  YGConfigRef config = YGConfigNew();
  YGConfigSetPointScaleFactor(config, 3.0f);

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root, 300);

  YGNodeRef spacer = YGNodeNewWithConfig(config);
  YGNodeStyleSetHeight(spacer, 2004.0f + 1.0f / 3.0f);
  YGNodeInsertChild(root, spacer, 0);

  YGNodeRef text = YGNodeNewWithConfig(config);
  YGNodeSetMeasureFunc(text, _measureTwoLines);
  YGNodeInsertChild(root, text, 1);

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_EQ(44.0f, YGNodeLayoutGetHeight(text));

  YGNodeFreeRecursive(root);
  YGConfigFree(config);
}
