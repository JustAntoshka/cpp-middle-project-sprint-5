#include <gtest/gtest.h>
#include "visualization.hpp"

using namespace geometry;
using namespace geometry::triangulation;
using namespace geometry::visualization;

TEST(DrawShapes, SimpleCheck) {
    std::vector<Shape> shapes = {
        Line{{0, 0}, {10, 0}},
        Triangle{{0, 0}, {5, 10}, {10, 0}},
        Rectangle{{0, 0}, 10, 5},
        Circle{{5, 5}, 3},
        RegularPolygon{{0, 0}, 5, 6},
        Polygon{{{0, 0}, {5, 0}, {5, 5}, {0, 5}}}
    };

    EXPECT_NO_THROW({
        Draw(shapes);
    });
}

TEST(DrawDelaunayTriangles, SimpleCheck) {
    std::vector<DelaunayTriangle> triangles = {
        {{0, 0}, {10, 0}, {0, 10}},
        {{10, 0}, {10, 10}, {0, 10}}
    };

    EXPECT_NO_THROW({
        Draw(triangles);
    });
}