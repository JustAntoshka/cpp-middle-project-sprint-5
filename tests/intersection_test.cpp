#include <gtest/gtest.h>
#include "intersections.hpp"

using namespace geometry;
using namespace geometry::intersections;

bool compareDouble(double d1, double d2) {
    return d1 - d2 <= geometry::intersections::EPS;
}

bool comparePoints(const Point2D& p1, const Point2D& p2) {
    return compareDouble(p1.x, p2.x) && compareDouble(p1.y, p2.y);
}

void EXPECT_NO_POINT(const std::optional<Point2D>& act_p) {
    EXPECT_TRUE(!act_p.has_value());
}

void EXPECT_POINT(const std::optional<Point2D>& act_p, const Point2D& exp_p) {
    EXPECT_TRUE(act_p.has_value());
    EXPECT_TRUE(comparePoints(act_p.value(), exp_p));
}

void EXPECT_ONE_OF_POINTS(const std::optional<Point2D>& act_p, const Point2D& exp_p1, const Point2D& exp_p2) {
    EXPECT_TRUE(act_p.has_value());
    EXPECT_TRUE(comparePoints(act_p.value(), exp_p1) || comparePoints(act_p.value(), exp_p2));
}

TEST(IntersectionTest, LineLineIntersection) {
    Line l1{{0, 0}, {2, 2}};
    Line l2{{0, 2}, {2, 0}};

    auto pt = GetIntersectPoint(l1, l2);
    EXPECT_TRUE(pt.has_value());
    EXPECT_NEAR(pt->x, 1, 1e-10);
    EXPECT_NEAR(pt->y, 1, 1e-10);
}

TEST(IntersectionTest, LineCircleIntersection_Yeq0_Center) {
    Line l1{{-5, 0}, {5, 0}};
    Line l2{{0, 0}, {5, 0}};
    Line l3{{-5, 0}, {0, 0}};
    Line l4{{-1, 0}, {1, 0}};
    Line l5{{-5, 3}, {5, 3}};
    Circle c{{0, 0}, 3};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);
    auto p5 = GetIntersectPoint(l5, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{-3, 0}, Point2D{3, 0});
    EXPECT_POINT(p2, Point2D{3, 0});
    EXPECT_POINT(p3, Point2D{-3, 0});
    EXPECT_NO_POINT(p4);
    EXPECT_POINT(p5, Point2D{0, 3});
}

TEST(IntersectionTest, LineCircleIntersection_Yeq0_Shift) {
    Line l1{{-5, 1}, {5, 1}};
    Line l2{{0, 1}, {5, 1}};
    Line l3{{-5, 1}, {0, 1}};
    Line l4{{-1, 1}, {1, 1}};
    Line l5{{-5, 4}, {5, 4}};
    Circle c{{1, 1}, 3};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);
    auto p5 = GetIntersectPoint(l5, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{-2, 1}, Point2D{4, 1});
    EXPECT_POINT(p2, Point2D{4, 1});
    EXPECT_POINT(p3, Point2D{-2, 1});
    EXPECT_NO_POINT(p4);
    EXPECT_POINT(p5, Point2D{1, 4});
}

TEST(IntersectionTest, LineCircleIntersection_YeqC_Center) {
    Line l1{{-10, 3}, {10, 3}};
    Line l2{{0, 3}, {10, 3}};
    Line l3{{-10, 3}, {0, 3}};
    Line l4{{-1, 3}, {1, 3}};
    Circle c{{0, 0}, 5};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{-4, 3}, Point2D{4, 3});
    EXPECT_POINT(p2, Point2D{4, 3});
    EXPECT_POINT(p3, Point2D{-4, 3});
    EXPECT_NO_POINT(p4);
}

TEST(IntersectionTest, LineCircleIntersection_YeqC_Shift) {
    Line l1{{-10, 4}, {10, 4}};
    Line l2{{0, 4}, {10, 4}};
    Line l3{{-10, 4}, {0, 4}};
    Line l4{{-1, 4}, {1, 4}};
    Circle c{{1, 1}, 5};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{-3, 4}, Point2D{5, 4});
    EXPECT_POINT(p2, Point2D{5, 4});
    EXPECT_POINT(p3, Point2D{-3, 4});
    EXPECT_NO_POINT(p4);
}

TEST(IntersectionTest, LineCircleIntersection_Xeq0_Center) {
    Line l1{{0, -5}, {0, 5}};
    Line l2{{0, 0}, {0, 5}};
    Line l3{{0, 0}, {0, -5}};
    Line l4{{0, -1}, {0, 1}};
    Line l5{{-3, -3}, {-3, 3}};
    Circle c{{0, 0}, 3};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);
    auto p5 = GetIntersectPoint(l5, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{0, -3}, Point2D{0, 3});
    EXPECT_POINT(p2, Point2D{0, 3});
    EXPECT_POINT(p3, Point2D{0, -3});
    EXPECT_NO_POINT(p4);
    EXPECT_POINT(p5, Point2D{-3, 0});
}

TEST(IntersectionTest, LineCircleIntersection_Xeq0_Shift) {
    Line l1{{1, -5}, {1, 5}};
    Line l2{{1, 1}, {1, 5}};
    Line l3{{1, 1}, {1, -5}};
    Line l4{{1, 0}, {1, 2}};
    Line l5{{-2, -3}, {-2, 3}};
    Circle c{{1, 1}, 3};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);
    auto p5 = GetIntersectPoint(l5, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{1, -2}, Point2D{1, 4});
    EXPECT_POINT(p2, Point2D{1, 4});
    EXPECT_POINT(p3, Point2D{1, -2});
    EXPECT_NO_POINT(p4);
    EXPECT_POINT(p5, Point2D{-2, 1});
}

TEST(IntersectionTest, LineCircleIntersection_XeqC_Center) {
    Line l1{{3, -10}, {3, 10}};
    Line l2{{3, 0}, {3, 10}};
    Line l3{{3, -10}, {3, 0}};
    Line l4{{3, -1}, {3, 1}};
    Circle c{{0, 0}, 5};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{3, -4}, Point2D{3, 4});
    EXPECT_POINT(p2, Point2D{3, 4});
    EXPECT_POINT(p3, Point2D{3, -4});
    EXPECT_NO_POINT(p4);
}

TEST(IntersectionTest, LineCircleIntersection_XeqC_Shift) {
    Line l1{{4, -10}, {4, 10}};
    Line l2{{4, 0}, {4, 10}};
    Line l3{{4, -10}, {4, 0}};
    Line l4{{4, -1}, {4, 1}};
    Circle c{{1, 1}, 5};

    auto p1 = GetIntersectPoint(l1, c);
    auto p2 = GetIntersectPoint(l2, c);
    auto p3 = GetIntersectPoint(l3, c);
    auto p4 = GetIntersectPoint(l4, c);

    EXPECT_ONE_OF_POINTS(p1, Point2D{4, -3}, Point2D{4, 5});
    EXPECT_POINT(p2, Point2D{4, 5});
    EXPECT_POINT(p3, Point2D{4, -3});
    EXPECT_NO_POINT(p4);
}

TEST(IntersectionTest, LineCircleIntersection_YeqKX) {
    Line l1{{3, 4}, {-3, -4}};
    Line l2{{6, 8}, {-6, -8}};
    Circle c{{0, 0}, 5};

    auto opt_p1 = GetIntersectPoint(l1, c);
    auto opt_p2 = GetIntersectPoint(l2, c);
    EXPECT_TRUE(opt_p1.has_value());
    EXPECT_TRUE(opt_p2.has_value());

    const auto& p1 = opt_p1.value();
    const auto& p2 = opt_p2.value();
    EXPECT_TRUE(comparePoints(p1, Point2D{3, 4}) || comparePoints(p1, Point2D{-3, -4}));
    EXPECT_TRUE(comparePoints(p2, Point2D{3, 4}) || comparePoints(p2, Point2D{-3, -4}));
}

TEST(IntersectionTest, CircleLineIntersection_YeqKX) {
    Line l1{{3, 4}, {-3, -4}};
    Line l2{{6, 8}, {-6, -8}};
    Circle c{{0, 0}, 5};

    auto opt_p1 = GetIntersectPoint(c, l1);
    auto opt_p2 = GetIntersectPoint(c, l2);
    EXPECT_TRUE(opt_p1.has_value());
    EXPECT_TRUE(opt_p2.has_value());

    const auto& p1 = opt_p1.value();
    const auto& p2 = opt_p2.value();
    EXPECT_TRUE(comparePoints(p1, Point2D{3, 4}) || comparePoints(p1, Point2D{-3, -4}));
    EXPECT_TRUE(comparePoints(p2, Point2D{3, 4}) || comparePoints(p2, Point2D{-3, -4}));
}

TEST(IntersectionTest, CircleCircleIntersection_TwoPoints) {
    Circle c1{{0, 0}, 2};
    Circle c2{{3, 0}, 2};

    auto pt = GetIntersectPoint(c1, c2);
    ASSERT_TRUE(pt.has_value());
    EXPECT_NEAR(pt->x, 1.5, geometry::intersections::EPS);
    EXPECT_NEAR(pt->y, std::sqrt(2 * 2 - 1.5 * 1.5), geometry::intersections::EPS);
}

TEST(IntersectionTest, CircleCircleIntersection_OnePoint) {
    Circle c1{{0, 0}, 2};
    Circle c2{{4, 0}, 2};

    auto pt = GetIntersectPoint(c1, c2);
    ASSERT_TRUE(pt.has_value());
    EXPECT_NEAR(pt->x, 2, geometry::intersections::EPS);
    EXPECT_NEAR(pt->y, 0, geometry::intersections::EPS);
}

TEST(IntersectionTest, CircleCircleIntersection_NoPoints) {
    Circle c1{{0, 0}, 2};
    Circle c2{{5, 0}, 2};

    auto pt = GetIntersectPoint(c1, c2);
    ASSERT_FALSE(pt.has_value());
}

TEST(IntersectionTest, UnsupportedShapeCombinationThrows) {
    Triangle tri{{0,0}, {1,0}, {0,1}};
    Line line{{0,0}, {1,1}};
    RegularPolygon regpoly{{0,0}, 1, 10};
    Circle circle{{0,0}, 1};
    Polygon poly{{{0,0}, {0,1}, {1,2}, {2,1}, {2,0}}};

    EXPECT_THROW(GetIntersectPoint(tri, tri), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(tri, line), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(tri, regpoly), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(tri, circle), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(tri, poly), std::logic_error);

    EXPECT_THROW(GetIntersectPoint(line, tri), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(line, regpoly), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(line, poly), std::logic_error);

    EXPECT_THROW(GetIntersectPoint(regpoly, tri), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(regpoly, line), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(regpoly, regpoly), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(regpoly, circle), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(regpoly, poly), std::logic_error);

    EXPECT_THROW(GetIntersectPoint(circle, tri), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(circle, regpoly), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(circle, poly), std::logic_error);

    EXPECT_THROW(GetIntersectPoint(poly, tri), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(poly, line), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(poly, regpoly), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(poly, circle), std::logic_error);
    EXPECT_THROW(GetIntersectPoint(poly, poly), std::logic_error);
}