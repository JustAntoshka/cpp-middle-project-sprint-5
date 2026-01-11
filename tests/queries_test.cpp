#include <gtest/gtest.h>
#include "queries.hpp"
#include <limits>

using namespace geometry;
using namespace geometry::queries;

constexpr double EPS = 1e-10;

TEST(GeometryTest_PointToShapeDistanceVisitor, PointToLine) {
    Point2D p{1.0, 1.0};
    Line line{{0.0, 0.0}, {2.0, 0.0}};

    PointToShapeDistanceVisitor visitor{p};
    double dist = visitor(line);

    EXPECT_NEAR(dist, 1.0, EPS);
}

TEST(GeometryTest_PointToShapeDistanceVisitor, PointToTriangle) {
    Triangle tri{{0, 0}, {2, 0}, {0, 2}};
    Point2D p{1.0, -1.0};

    PointToShapeDistanceVisitor visitor{p};
    double dist = visitor(tri);

    EXPECT_NEAR(dist, 1.0, EPS);
}

TEST(GeometryTest_PointToShapeDistanceVisitor, PointToRectangle) {
    Rectangle rect{{0, 0}, 2.0, 2.0};
    Point2D p{3.0, 1.0};

    PointToShapeDistanceVisitor visitor{p};
    double dist = visitor(rect);

    EXPECT_NEAR(dist, 1.0, EPS);
}

TEST(GeometryTest_PointToShapeDistanceVisitor, PointToCircle) {
    Circle circle{{0, 0}, 2.0};
    Point2D p{5.0, 0.0};

    PointToShapeDistanceVisitor visitor{p};
    double dist = visitor(circle);

    EXPECT_NEAR(dist, 3.0, EPS);
}

TEST(GeometryTest_PointToShapeDistanceVisitor, PointToPolygon) {
    Polygon poly({
        {0, 0},
        {2, 0},
        {2, 2},
        {0, 2}
    });

    Point2D p{3.0, 0.0};
    PointToShapeDistanceVisitor visitor{p};

    double dist = visitor(poly);
    EXPECT_NEAR(dist, 1.0, EPS);
}



TEST(GeometryTest_ShapeToShapeDistanceVisitor, CircleToCircle_NoIntersection) {
    Circle c1{{0, 0}, 1.0};
    Circle c2{{5, 0}, 1.0};

    ShapeToShapeDistanceVisitor visitor;
    auto dist = visitor(c1, c2);

    ASSERT_TRUE(dist.has_value());
    EXPECT_NEAR(*dist, 3.0, EPS);
}

TEST(GeometryTest_ShapeToShapeDistanceVisitor, LineToLine_Parallel) {
    Line l1{{0, 0}, {2, 0}};
    Line l2{{0, 2}, {2, 2}};

    ShapeToShapeDistanceVisitor visitor;
    auto dist = visitor(l1, l2);

    ASSERT_TRUE(dist.has_value());
    EXPECT_NEAR(*dist, 2.0, EPS);
}

TEST(GeometryTest_ShapeToShapeDistanceVisitor, UnsupportedShapeCombinationThrows) {
    Triangle tri{{0,0}, {1,0}, {0,1}};
    Line line{{0,0}, {1,1}};
    RegularPolygon regpoly{{0,0}, 1, 10};
    Circle circle{{0,0}, 1};
    Polygon poly{{{0,0}, {0,1}, {1,2}, {2,1}, {2,0}}};

    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(tri, tri).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(tri, line).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(tri, regpoly).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(tri, circle).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(tri, poly).has_value());

    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(line, tri).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(line, regpoly).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(line, circle).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(line, poly).has_value());

    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(regpoly, tri).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(regpoly, line).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(regpoly, regpoly).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(regpoly, circle).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(regpoly, poly).has_value());

    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(circle, tri).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(circle, line).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(circle, regpoly).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(circle, poly).has_value());

    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(poly, tri).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(poly, line).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(poly, regpoly).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(poly, circle).has_value());
    EXPECT_FALSE(ShapeToShapeDistanceVisitor{}(poly, poly).has_value());
}