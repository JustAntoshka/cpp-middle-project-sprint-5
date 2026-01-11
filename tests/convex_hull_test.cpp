#include <gtest/gtest.h>
#include "convex_hull.hpp"

using geometry::Point2D;
using geometry::convex_hull::GrahamScan;

TEST(GrahamScan, ReturnExpected) {
    std::vector<Point2D> points = {
        {0, 0},
        {10, 0},
        {10, 10},
        {0, 10},
        {5, 5}   // internal
    };

    auto result = GrahamScan(points);

    ASSERT_TRUE(result.has_value());

    const auto& hull = result.value();
    ASSERT_EQ(hull.size(), 4);

    EXPECT_NE(std::find(hull.begin(), hull.end(), Point2D{0, 0}), hull.end());
    EXPECT_NE(std::find(hull.begin(), hull.end(), Point2D{10, 0}), hull.end());
    EXPECT_NE(std::find(hull.begin(), hull.end(), Point2D{10, 10}), hull.end());
    EXPECT_NE(std::find(hull.begin(), hull.end(), Point2D{0, 10}), hull.end());

    EXPECT_EQ(std::find(hull.begin(), hull.end(), Point2D{5, 5}), hull.end());
}

TEST(GrahamScan, ReturnError) {
    std::vector<Point2D> points = {
        {0, 0},
        {1, 1}
    };

    auto result = GrahamScan(points);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "At least three points are required for convex hull.");
}