#include <gtest/gtest.h>
#include "geometry.hpp"

#include <print>

using namespace geometry;

/**************************************************************************************************
* Point2D
**************************************************************************************************/

TEST(GeometryTest_Point, CtorAndOpEq) {
    EXPECT_EQ(Point2D{}, Point2D{});
    EXPECT_EQ(Point2D(0, 0), Point2D(0, 0));
    EXPECT_EQ(Point2D{}, Point2D(0, 0));
    EXPECT_EQ(Point2D(42, 42), Point2D(42, 42) );
    EXPECT_EQ(Point2D(42, -42), Point2D(42, -42) );
    EXPECT_EQ(Point2D(-42, 42), Point2D(-42, 42) );
    EXPECT_EQ(Point2D(-42, -42), Point2D(-42, -42) );

    EXPECT_NE(Point2D{}, Point2D(42, 42));
    EXPECT_NE(Point2D(42, 42), Point2D{});
    EXPECT_NE(Point2D(42, 42), Point2D(42, 42.00000000001));
}

TEST(GeometryTest_Point, CtorAndOpLess) {
    EXPECT_TRUE(Point2D{} < Point2D(42.42, 42.42));
    EXPECT_TRUE(Point2D(-42.42, -42.42) < Point2D(42.42, 42.42));
    
    EXPECT_FALSE(Point2D{} < Point2D(-42.42, -42.42));
    EXPECT_FALSE(Point2D(-42.42, 42.42) < Point2D(42.42, 42.42));
    EXPECT_FALSE(Point2D(42.42, -42.42) < Point2D(42.42, 42.42));
}

TEST(GeometryTest_Point, AdditionAndSubtraction) {
    Point2D a{1.0, 2.0};
    Point2D b{3.5, -4.0};

    EXPECT_EQ(a + b, Point2D(4.5, -2.0));
    EXPECT_EQ(b + a, Point2D(4.5, -2.0));

    EXPECT_EQ(a - b, Point2D(-2.5, 6.0));
    EXPECT_EQ(b - a, Point2D(2.5, -6.0));

    EXPECT_EQ(a - a, Point2D(0.0, 0.0));
}

TEST(GeometryTest_Point, ScalarMultiplicationAndDivision) {
    Point2D p{2.0, -4.0};

    EXPECT_EQ(p * 2.0, Point2D(4.0, -8.0));
    EXPECT_EQ(p * 0.5, Point2D(1.0, -2.0));

    EXPECT_EQ(p / 2.0, Point2D(1.0, -2.0));
    EXPECT_EQ(p / -2.0, Point2D(-1.0, 2.0));
}

TEST(GeometryTest_Point, DotProduct) {
    Point2D a{1.0, 2.0};
    Point2D b{3.0, 4.0};

    EXPECT_DOUBLE_EQ(a.Dot(b), 11.0);
    EXPECT_DOUBLE_EQ(b.Dot(a), 11.0);

    EXPECT_DOUBLE_EQ(a.Dot(a), 5.0);
    EXPECT_DOUBLE_EQ(Point2D{}.Dot(a), 0.0);

    // Orthogonality
    Point2D a_ortho{-2.0, 1.0};
    Point2D c1{42.0, 0.0};
    Point2D c2{0.0, 0.42};

    EXPECT_DOUBLE_EQ(a.Dot(a_ortho), 0);
    EXPECT_DOUBLE_EQ(c1.Dot(c2), 0);
    EXPECT_DOUBLE_EQ(c2.Dot(c1), 0);
}

TEST(GeometryTest_Point, CrossProduct) {
    Point2D a{1.0, 0.0};
    Point2D b{0.0, 1.0};

    EXPECT_DOUBLE_EQ(a.Cross(b), 1.0);
    EXPECT_DOUBLE_EQ(b.Cross(a), -1.0);

    EXPECT_DOUBLE_EQ(a.Cross(a), 0.0);
    EXPECT_DOUBLE_EQ(Point2D{}.Cross(b), 0.0);
}

TEST(GeometryTest_Point, Length) {
    EXPECT_DOUBLE_EQ(Point2D{}.Length(), 0.0);

    Point2D a{3.0, 4.0};
    EXPECT_DOUBLE_EQ(a.Length(), 5.0);

    Point2D b{-3.0, -4.0};
    EXPECT_DOUBLE_EQ(b.Length(), 5.0);
}

TEST(GeometryTest_Point, Normalize) {
    Point2D zero{};
    EXPECT_EQ(zero.Normalize(), Point2D(0.0, 0.0));

    Point2D p{3.0, 4.0};
    auto n = p.Normalize();

    EXPECT_DOUBLE_EQ(n.x, 0.6);
    EXPECT_DOUBLE_EQ(n.y, 0.8);
}

TEST(GeometryTest_Point, DistanceTo) {
    Point2D a{0.0, 0.0};
    Point2D b{3.0, 4.0};

    EXPECT_DOUBLE_EQ(a.DistanceTo(b), 5.0);
    EXPECT_DOUBLE_EQ(b.DistanceTo(a), 5.0);

    EXPECT_DOUBLE_EQ(a.DistanceTo(a), 0.0);
    EXPECT_DOUBLE_EQ(b.DistanceTo(b), 0.0);
}



/**************************************************************************************************
* Bounding Box
**************************************************************************************************/

TEST(GeometryTest_BoundingBox, DefaultCtor) {
    BoundingBox b;

    EXPECT_DOUBLE_EQ(b.min_x, 0.0);
    EXPECT_DOUBLE_EQ(b.min_y, 0.0);
    EXPECT_DOUBLE_EQ(b.max_x, 0.0);
    EXPECT_DOUBLE_EQ(b.max_y, 0.0);

    EXPECT_DOUBLE_EQ(b.Width(), 0.0);
    EXPECT_DOUBLE_EQ(b.Height(), 0.0);
    EXPECT_EQ(b.Center(), Point2D(0.0, 0.0));
}

TEST(GeometryTest_BoundingBox, ParametrizedCtor) {
    BoundingBox b{-1.0, -2.0, 3.0, 4.0};

    EXPECT_DOUBLE_EQ(b.min_x, -1.0);
    EXPECT_DOUBLE_EQ(b.min_y, -2.0);
    EXPECT_DOUBLE_EQ(b.max_x, 3.0);
    EXPECT_DOUBLE_EQ(b.max_y, 4.0);

    EXPECT_DOUBLE_EQ(b.Width(), 4.0);
    EXPECT_DOUBLE_EQ(b.Height(), 6.0);
    EXPECT_EQ(b.Center(), Point2D(1.0, 1.0));
}

TEST(GeometryTest_BoundingBox, OverlapsBasic) {
    BoundingBox a{0.0, 0.0, 2.0, 2.0};
    BoundingBox b{1.0, 1.0, 3.0, 3.0};

    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_TRUE(b.Overlaps(a));
}

TEST(GeometryTest_BoundingBox, OverlapsNone) {
    BoundingBox a{0.0, 0.0, 1.0, 1.0};
    BoundingBox b{2.0, 2.0, 3.0, 3.0};

    EXPECT_FALSE(a.Overlaps(b));
    EXPECT_FALSE(b.Overlaps(a));
}

TEST(GeometryTest_BoundingBox, OverlapsTouchingEdges) {
    BoundingBox a{0.0, 0.0, 1.0, 1.0};
    BoundingBox b{1.0, 1.0, 2.0, 2.0};

    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_TRUE(b.Overlaps(a));
}

TEST(GeometryTest_BoundingBox, OverlapsContained) {
    BoundingBox outer{-10.0, -10.0, 10.0, 10.0};
    BoundingBox inner{-1.0, -1.0, 1.0, 1.0};

    EXPECT_TRUE(outer.Overlaps(inner));
    EXPECT_TRUE(inner.Overlaps(outer));
}

TEST(GeometryTest_BoundingBox, OnePointBox) {
    BoundingBox point{1.0, 1.0, 1.0, 1.0};
    BoundingBox area{0.0, 0.0, 2.0, 2.0};

    EXPECT_DOUBLE_EQ(point.Width(), 0.0);
    EXPECT_DOUBLE_EQ(point.Height(), 0.0);

    EXPECT_TRUE(point.Overlaps(area));
    EXPECT_TRUE(area.Overlaps(point));
}



/**************************************************************************************************
* Line
**************************************************************************************************/

TEST(GeometryTest_Line, CtorStoresPoints) {
    Point2D a{1.0, 2.0};
    Point2D b{3.0, 4.0};

    Line l{a, b};

    EXPECT_EQ(l.start, a);
    EXPECT_EQ(l.end, b);
}

TEST(GeometryTest_Line, Length) {
    Line l{{0.0, 0.0}, {3.0, 4.0}};
    EXPECT_DOUBLE_EQ(l.Length(), 5.0);
}

TEST(GeometryTest_Line, ZeroLength) {
    Line l{{1.0, 1.0}, {1.0, 1.0}};
    EXPECT_DOUBLE_EQ(l.Length(), 0.0);
}

TEST(GeometryTest_Line, Direction) {
    Line l{{0.0, 0.0}, {3.0, 4.0}};
    auto d = l.Direction();

    EXPECT_DOUBLE_EQ(d.x, 0.6);
    EXPECT_DOUBLE_EQ(d.y, 0.8);
}

TEST(GeometryTest_Line, DirectionOfZeroLength) {
    Line l{{1.0, 1.0}, {1.0, 1.0}};
    EXPECT_EQ(l.Direction(), Point2D(0.0, 0.0));
}

TEST(GeometryTest_Line, BoundBox) {
    Line l{{3.0, 1.0}, {1.0, 5.0}};
    auto b = l.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, 1.0);
    EXPECT_DOUBLE_EQ(b.max_x, 3.0);
    EXPECT_DOUBLE_EQ(b.min_y, 1.0);
    EXPECT_DOUBLE_EQ(b.max_y, 5.0);
}

TEST(GeometryTest_Line, Height) {
    Line l{{0.0, 2.0}, {5.0, -3.0}};
    EXPECT_DOUBLE_EQ(l.Height(), 2.0);
}

TEST(GeometryTest_Line, Center) {
    Line l{{0.0, 0.0}, {2.0, 2.0}};
    EXPECT_EQ(l.Center(), Point2D(1.0, 1.0));
}

TEST(GeometryTest_Line, Vertices) {
    Line l{{1.0, 2.0}, {3.0, 4.0}};
    auto v = l.Vertices();

    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], Point2D(1.0, 2.0));
    EXPECT_EQ(v[1], Point2D(3.0, 4.0));
}

TEST(GeometryTest_Line, Lines) {
    Line l{{1.0, 2.0}, {3.0, 4.0}};
    auto lines = l.Lines();

    ASSERT_EQ(lines.x.size(), 2u);
    ASSERT_EQ(lines.y.size(), 2u);

    EXPECT_DOUBLE_EQ(lines.x[0], 1.0);
    EXPECT_DOUBLE_EQ(lines.y[0], 2.0);

    EXPECT_DOUBLE_EQ(lines.x[1], 3.0);
    EXPECT_DOUBLE_EQ(lines.y[1], 4.0);
}



/**************************************************************************************************
* Triangle
**************************************************************************************************/

TEST(GeometryTest_Triangle, CtorStoresVertices) {
    Point2D a{0.0, 0.0};
    Point2D b{1.0, 0.0};
    Point2D c{0.0, 1.0};

    Triangle t{a, b, c};

    EXPECT_EQ(t.a, a);
    EXPECT_EQ(t.b, b);
    EXPECT_EQ(t.c, c);
}

TEST(GeometryTest_Triangle, Area) {
    Triangle t1{{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}};
    Triangle t2{{0.0, 0.0}, {-4.0, 0.0}, {0.0, -3.0}};

    EXPECT_DOUBLE_EQ(t1.Area(), 6.0);
    EXPECT_DOUBLE_EQ(t2.Area(), 6.0);
}

TEST(GeometryTest_Triangle, AreaDegenerate) {
    Triangle t{{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}};
    EXPECT_DOUBLE_EQ(t.Area(), 0.0);
}

TEST(GeometryTest_Triangle, BoundBox) {
    Triangle t{{-1.0, 2.0}, {3.0, -4.0}, {0.0, 1.0}};
    auto b = t.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, -1.0);
    EXPECT_DOUBLE_EQ(b.max_x, 3.0);
    EXPECT_DOUBLE_EQ(b.min_y, -4.0);
    EXPECT_DOUBLE_EQ(b.max_y, 2.0);
}

TEST(GeometryTest_Triangle, Vertices) {
    Triangle t{{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    auto v = t.Vertices();

    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], Point2D(1.0, 2.0));
    EXPECT_EQ(v[1], Point2D(3.0, 4.0));
    EXPECT_EQ(v[2], Point2D(5.0, 6.0));
}

TEST(GeometryTest_Triangle, Height) {
    Triangle t{{0.0, 1.0}, {2.0, 5.0}, {-1.0, 3.0}};
    EXPECT_DOUBLE_EQ(t.Height(), 5.0);
}

TEST(GeometryTest_Triangle, Center) {
    Triangle t{{0.0, 0.0}, {3.0, 0.0}, {0.0, 3.0}};
    EXPECT_EQ(t.Center(), Point2D(1.0, 1.0));
}

TEST(GeometryTest_Triangle, Lines) {
    Triangle t{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    auto lines = t.Lines();

    ASSERT_EQ(lines.x.size(), 4u);
    ASSERT_EQ(lines.y.size(), 4u);

    EXPECT_DOUBLE_EQ(lines.x[0], 0.0);
    EXPECT_DOUBLE_EQ(lines.y[0], 0.0);

    EXPECT_DOUBLE_EQ(lines.x[1], 1.0);
    EXPECT_DOUBLE_EQ(lines.y[1], 0.0);

    EXPECT_DOUBLE_EQ(lines.x[2], 0.0);
    EXPECT_DOUBLE_EQ(lines.y[2], 1.0);

    EXPECT_DOUBLE_EQ(lines.x[3], 0.0);
    EXPECT_DOUBLE_EQ(lines.y[3], 0.0);
}



/**************************************************************************************************
* Rectangle
**************************************************************************************************/

TEST(GeometryTest_Rectangle, CtorStoresData) {
    Point2D bl{1.0, 2.0};
    Rectangle r{bl, 3.0, 4.0};

    EXPECT_EQ(r.bottom_left, bl);
    EXPECT_DOUBLE_EQ(r.width, 3.0);
    EXPECT_DOUBLE_EQ(r.height, 4.0);
}

TEST(GeometryTest_Rectangle, TopRight) {
    Rectangle r{{1.0, 2.0}, 3.0, 4.0};
    EXPECT_EQ(r.TopRight(), Point2D(4.0, 6.0));
}

TEST(GeometryTest_Rectangle, BoundBox) {
    Rectangle r{{-1.0, -2.0}, 5.0, 6.0};
    auto b = r.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, -1.0);
    EXPECT_DOUBLE_EQ(b.min_y, -2.0);
    EXPECT_DOUBLE_EQ(b.max_x, 4.0);
    EXPECT_DOUBLE_EQ(b.max_y, 4.0);
}

TEST(GeometryTest_Rectangle, Vertices) {
    Rectangle r{{0.0, 0.0}, 2.0, 3.0};
    auto v = r.Vertices();

    ASSERT_EQ(v.size(), 4u);

    EXPECT_EQ(v[0], Point2D(0.0, 0.0));
    EXPECT_EQ(v[1], Point2D(2.0, 0.0));
    EXPECT_EQ(v[2], Point2D(2.0, 3.0));
    EXPECT_EQ(v[3], Point2D(0.0, 3.0));
}

TEST(GeometryTest_Rectangle, Height) {
    Rectangle r{{0.0, 5.0}, 10.0, 7.0};
    EXPECT_DOUBLE_EQ(r.Height(), 12.0);
}

TEST(GeometryTest_Rectangle, Center) {
    Rectangle r{{0.0, 0.0}, 4.0, 6.0};
    EXPECT_EQ(r.Center(), Point2D(2.0, 3.0));
}

TEST(GeometryTest_Rectangle, Lines) {
    Rectangle r{{1.0, 2.0}, 3.0, 4.0};
    auto lines = r.Lines();

    ASSERT_EQ(lines.x.size(), 5u);
    ASSERT_EQ(lines.y.size(), 5u);

    EXPECT_DOUBLE_EQ(lines.x[0], 1.0);
    EXPECT_DOUBLE_EQ(lines.y[0], 2.0);
    EXPECT_DOUBLE_EQ(lines.x[1], 1.0);
    EXPECT_DOUBLE_EQ(lines.y[1], 6.0);
    EXPECT_DOUBLE_EQ(lines.x[2], 4.0);
    EXPECT_DOUBLE_EQ(lines.y[2], 6.0);
    EXPECT_DOUBLE_EQ(lines.x[3], 4.0);
    EXPECT_DOUBLE_EQ(lines.y[3], 2.0);
    EXPECT_DOUBLE_EQ(lines.x[4], 1.0);
    EXPECT_DOUBLE_EQ(lines.y[4], 2.0);
}

TEST(GeometryTest_Rectangle, DegenerateRectangle) {
    Rectangle r1{{0.0, 0.0}, 0.0, 5.0};
    Rectangle r2{{0.0, 0.0}, 5.0, 0.0};
    Rectangle r3{{0.0, 0.0}, 0.0, 0.0};

    EXPECT_DOUBLE_EQ(r1.BoundBox().Width(), 0.0);
    EXPECT_DOUBLE_EQ(r2.BoundBox().Height(), 0.0);
    EXPECT_DOUBLE_EQ(r3.BoundBox().Width(), 0.0);
    EXPECT_DOUBLE_EQ(r3.BoundBox().Height(), 0.0);
}



/**************************************************************************************************
* RegularPolygon
**************************************************************************************************/

TEST(GeometryTest_RegularPolygon, CtorStoresData) {
    Point2D center{1.0, 2.0};
    RegularPolygon p{center, 5.0, 6};

    EXPECT_EQ(p.center_p, center);
    EXPECT_DOUBLE_EQ(p.radius, 5.0);
    EXPECT_EQ(p.sides, 6);
}

TEST(GeometryTest_RegularPolygon, VerticesCountAndRadius) {
    RegularPolygon p{{0.0, 0.0}, 10.0, 8};
    auto verts = p.Vertices();

    ASSERT_EQ(verts.size(), 8u);

    for (const auto& v : verts) {
        EXPECT_DOUBLE_EQ(v.DistanceTo(Point2D(0.0, 0.0)), 10.0);
    }
}

TEST(GeometryTest_RegularPolygon, BoundBox) {
    RegularPolygon p{{2.0, 3.0}, 4.0, 16};
    auto b = p.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, -2.0);
    EXPECT_DOUBLE_EQ(b.min_y, -1.0);
    EXPECT_DOUBLE_EQ(b.max_x, 6.0);
    EXPECT_DOUBLE_EQ(b.max_y, 7.0);
}

TEST(GeometryTest_RegularPolygon, Height) {
    RegularPolygon p{{0.0, -5.0}, 3.0, 12};
    EXPECT_DOUBLE_EQ(p.Height(), -2.0);
}

TEST(GeometryTest_RegularPolygon, Center) {
    Point2D center{7.0, -3.0};
    RegularPolygon p{center, 2.0, 5};

    EXPECT_EQ(p.Center(), center);
}

TEST(GeometryTest_RegularPolygon, LinesMatchVertices) {
    RegularPolygon p{{0.0, 0.0}, 3.0, 5};
    auto verts = p.Vertices();
    auto lines = p.Lines();

    for (size_t i = 0; i < verts.size(); ++i) {
        EXPECT_DOUBLE_EQ(lines.x[i], verts[i].x);
        EXPECT_DOUBLE_EQ(lines.y[i], verts[i].y);
    }
}



/**************************************************************************************************
* Circle
**************************************************************************************************/

TEST(GeometryTest_Circle, CtorStoresData) {
    Point2D center{1.0, -2.0};
    Circle c{center, 5.0};

    EXPECT_EQ(c.center_p, center);
    EXPECT_DOUBLE_EQ(c.radius, 5.0);
}

TEST(GeometryTest_Circle, BoundBox) {
    Circle c{{2.0, 3.0}, 4.0};
    auto b = c.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, -2.0);
    EXPECT_DOUBLE_EQ(b.min_y, -1.0);
    EXPECT_DOUBLE_EQ(b.max_x, 6.0);
    EXPECT_DOUBLE_EQ(b.max_y, 7.0);
}

TEST(GeometryTest_Circle, Height) {
    Circle c{{0.0, -5.0}, 3.0};
    EXPECT_DOUBLE_EQ(c.Height(), -2.0);
}

TEST(GeometryTest_Circle, Center) {
    Point2D center{10.0, 20.0};
    Circle c{center, 7.0};

    EXPECT_EQ(c.Center(), center);
}

TEST(GeometryTest_Circle, Vertices) {
    Circle c{{0.0, 0.0}, 5.0};
    auto verts = c.Vertices(32);

    EXPECT_EQ(verts.size(), 32u);

    for (const auto& v : verts) {
        EXPECT_DOUBLE_EQ(v.DistanceTo(Point2D(0.0, 0.0)), 5.0);
    }
}

TEST(GeometryTest_Circle, LinesPointsOnCircle) {
    Circle c{{1.0, 1.0}, 3.0};
    auto lines = c.Lines(16);

    for (size_t i = 0; i + 1 < lines.x.size(); ++i) {
        Point2D p{lines.x[i], lines.y[i]};
        EXPECT_DOUBLE_EQ(p.DistanceTo(c.center_p), 3.0);
    }
}

TEST(GeometryTest_Circle, ZeroRadius) {
    Circle c{{2.0, 3.0}, 0.0};

    auto b = c.BoundBox();
    EXPECT_DOUBLE_EQ(b.min_x, 2.0);
    EXPECT_DOUBLE_EQ(b.max_x, 2.0);
    EXPECT_DOUBLE_EQ(b.min_y, 3.0);
    EXPECT_DOUBLE_EQ(b.max_y, 3.0);

    auto verts = c.Vertices(8);
    for (const auto& v : verts) {
        EXPECT_EQ(v, Point2D(2.0, 3.0));
    }
}



/**************************************************************************************************
* Polygon
**************************************************************************************************/

TEST(GeometryTest_Polygon, CtorStoresVertices) {
    std::vector<Point2D> pts = {
        {0.0, 0.0},
        {2.0, 0.0},
        {2.0, 1.0},
        {0.0, 1.0}
    };

    Polygon p{pts};
    auto v = p.Vertices();

    ASSERT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], Point2D(0.0, 0.0));
    EXPECT_EQ(v[1], Point2D(2.0, 0.0));
    EXPECT_EQ(v[2], Point2D(2.0, 1.0));
    EXPECT_EQ(v[3], Point2D(0.0, 1.0));
}

TEST(GeometryTest_Polygon, BoundBox) {
    Polygon p({
        {-1.0, 2.0},
        {3.0, -4.0},
        {0.0, 5.0}
    });

    auto b = p.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, -1.0);
    EXPECT_DOUBLE_EQ(b.max_x, 3.0);
    EXPECT_DOUBLE_EQ(b.min_y, -4.0);
    EXPECT_DOUBLE_EQ(b.max_y, 5.0);
}

TEST(GeometryTest_Polygon, Height) {
    Polygon p({
        {0.0, 1.0},
        {2.0, 5.0},
        {-1.0, 3.0}
    });

    EXPECT_DOUBLE_EQ(p.Height(), 4.0);
}

TEST(GeometryTest_Polygon, Center) {
    Polygon p({
        {0.0, 0.0},
        {4.0, 0.0},
        {4.0, 2.0},
        {0.0, 2.0}
    });

    EXPECT_EQ(p.Center(), Point2D(2.0, 1.0));
}

TEST(GeometryTest_Polygon, Vertices) {
    std::vector<Point2D> pts = {
        {1.0, 1.0},
        {2.0, 3.0},
        {4.0, 5.0}
    };

    Polygon p{pts};
    auto v = p.Vertices();

    ASSERT_EQ(v.size(), pts.size());
    for (size_t i = 0; i < pts.size(); ++i) {
        EXPECT_EQ(v[i], pts[i]);
    }
}

TEST(GeometryTest_Polygon, Lines) {
    std::vector<Point2D> pts = {
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0}
    };

    Polygon p{pts};
    auto lines = p.Lines();

    for (size_t i = 0; i < pts.size(); ++i) {
        EXPECT_DOUBLE_EQ(lines.x[i], pts[i].x);
        EXPECT_DOUBLE_EQ(lines.y[i], pts[i].y);
    }
}

TEST(GeometryTest_Polygon, DegenerateSinglePoint) {
    Polygon p({
        {2.0, 3.0}
    });

    auto b = p.BoundBox();

    EXPECT_DOUBLE_EQ(b.min_x, 2.0);
    EXPECT_DOUBLE_EQ(b.max_x, 2.0);
    EXPECT_DOUBLE_EQ(b.min_y, 3.0);
    EXPECT_DOUBLE_EQ(b.max_y, 3.0);

    EXPECT_DOUBLE_EQ(p.Height(), 0.0);
    EXPECT_EQ(p.Center(), Point2D(2.0, 3.0));
}