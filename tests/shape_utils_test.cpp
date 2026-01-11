#include <gtest/gtest.h>
#include "shape_utils.hpp"

using namespace geometry;
using namespace geometry::utils;

TEST(ParseShapes, ParsesMultipleShapesAndIgnoresEmptyTokens) {
    std::string input =
        "  circle 0 0 10  ; ;  rectangle 0 0 10 20 ;   triangle 0 0 1 0 0 1; ";

    auto shapes = ParseShapes(input);

    ASSERT_EQ(shapes.size(), 3);

    const auto& circle = std::get<Circle>(shapes[0]);
    const auto& rectangle = std::get<Rectangle>(shapes[1]);
    const auto& triangle = std::get<Triangle>(shapes[2]);

    EXPECT_EQ(circle.center_p, Point2D(0,0));
    EXPECT_EQ(circle.radius, 10);
    EXPECT_EQ(rectangle.bottom_left, Point2D(0,0));
    EXPECT_EQ(rectangle.width, 10);
    EXPECT_EQ(rectangle.height, 20);
    EXPECT_EQ(triangle.a, Point2D(0,0));
    EXPECT_EQ(triangle.b, Point2D(1,0));
    EXPECT_EQ(triangle.c, Point2D(0,1));
}

TEST(FindAllCollisions, DetectsOverlappingShapes) {
    const auto a = ParseShapes("rectangle 0 0 10 10")[0];
    const auto b = ParseShapes("rectangle 5 5 10 10")[0]; // пересекается с a
    const auto c = ParseShapes("rectangle 30 30 5 5")[0]; // не пересекается

    std::vector<Shape> shapes = {a, b, c};

    auto collisions = FindAllCollisions(shapes);

    ASSERT_EQ(collisions.size(), 1);

    const auto& rectangle_a = std::get<Rectangle>(collisions[0].first);
    const auto& rectangle_b = std::get<Rectangle>(collisions[0].second);

    EXPECT_EQ(rectangle_a.bottom_left, Point2D(0,0));
    EXPECT_EQ(rectangle_a.width, 10);
    EXPECT_EQ(rectangle_a.height, 10);
    EXPECT_EQ(rectangle_b.bottom_left, Point2D(5,5));
    EXPECT_EQ(rectangle_b.width, 10);
    EXPECT_EQ(rectangle_b.height, 10);
}

TEST(FindHighestShape, ReturnsMaximumHeight) {
    std::vector<Shape> shapes = {
        ParseShapes("rectangle 0 0 10 5")[0],   // height = 5
        ParseShapes("rectangle 0 0 10 20")[0],  // height = 20
        ParseShapes("rectangle 0 0 10 15")[0]   // height = 15
    };

    auto highest = FindHighestShape(shapes);

    ASSERT_TRUE(highest.has_value());
    EXPECT_EQ(*highest, 20u);
}