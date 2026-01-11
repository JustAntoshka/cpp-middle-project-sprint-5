#include <gtest/gtest.h>
#include "triangulation.hpp"

using geometry::Point2D;
using geometry::triangulation::DelaunayTriangulation;
using geometry::triangulation::DelaunayTriangle;

TEST(DelaunayTriangulation, ReturnExpected) {
    std::vector<Point2D> points = {
        {0, 0},
        {10, 0},
        {10, 10},
        {0, 10}
    };

    auto result = DelaunayTriangulation(points);

    ASSERT_TRUE(result.has_value());

    const auto& triangles = result.value();

    ASSERT_EQ(triangles.size(), 2);

    for (const auto& t : triangles) {
        for (const auto& v : t.vertices()) {
            EXPECT_NE(std::find(points.begin(), points.end(), v), points.end());
        }
    }
}

TEST(DelaunayTriangulation, ReturnUnexpected) {
    std::vector<Point2D> points = {
        {0, 0},
        {1, 1}
    };

    auto result = DelaunayTriangulation(points);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "At least three points are required for triangulation.");
}