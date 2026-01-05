#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>

template <class... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};

namespace geometry::intersections {

constexpr double EPS = 10e-10;

/*
* Класс для поиска пересечений между двумя фигурами
*
* Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
*    - Line   & Line
*    - Line   & Circle
*    - Circle & Circle
*
* Для всех остальных требуется выбросить исключение std::logic_error
*/
class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line&, const Line&);
    std::optional<Point2D> operator()(const Line&, const Circle&);
    std::optional<Point2D> operator()(const Circle&, const Line&);
    std::optional<Point2D> operator()(const Circle&, const Circle&);

    std::optional<Point2D> operator()(const Shape&, const Shape&);
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(Multilambda{IntersectionVisitor{}}, shape1, shape2);
}



bool isPointInSegment(const Point2D& point, const Line& line) {
    auto bbox = line.BoundBox();
    return bbox.min_x <= point.x && point.x <= bbox.max_x
        && bbox.min_y <= point.y && point.y <= bbox.max_y;
}

std::vector<Point2D> getLineAndCircleIntersect(double A, double B, double C, double r) {
    // Nearest point of line to circle center (0, 0)
    auto x = - (A * C) / (A * A + B * B);
    auto y = - (B * C) / (A * A + B * B);
    auto dist_to_line = std::abs(C) / std::sqrt(A * A + B * B);

    if(dist_to_line > r) {
        return {};
    }

    // Distance from nearest point to intersection with circle
    auto d = std::sqrt((r * r - dist_to_line * dist_to_line));

    if(d == 0) {
        return {{x, y}};
    }

    Point2D intersection1 = {x + B * d, y - A * d};
    Point2D intersection2 = {x - B * d, y + A * d};

    return {intersection1, intersection2};    
}

std::optional<Point2D> IntersectionVisitor::operator()(const Line& line1, const Line& line2) {
    auto bbox1 = line1.BoundBox();
    auto bbox2 = line2.BoundBox();

    if(!bbox1.Overlaps(bbox2)) {
        return std::nullopt;
    }

    auto area = [](const Point2D& a, const Point2D& b, const Point2D& c) {
        return (b - a).Cross(c - a);
    };
    auto sign = [](double value) {
        if(value > 0) return 1;
        if(value < 0) return -1;
        return 0;
    };
    auto area_sign = [&sign, &area](const Point2D& a, const Point2D& b, const Point2D& c) {
        return sign(area(a, b, c));
    };

    auto a = line1.start;
    auto b = line1.end;
    auto c = line2.start;
    auto d = line2.end;

    auto sign_abc = area_sign(a, b, c);
    auto sign_abd = area_sign(a, b, d);
    auto sign_cda = area_sign(c, d, a);
    auto sign_cdb = area_sign(c, d, b);

    if(sign_abc * sign_abd > 0 || sign_cda * sign_cdb > 0) {
        return std::nullopt;
    }

    auto den = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);
    if(den < EPS) {
        auto x = ((a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x)) / den;
        auto y = ((a.x * b.y - a.y * b.x) * (c.y - d.y) - (a.y - b.y) * (c.x * d.y - c.y * d.x)) / den;
        return {{x, y}};
    }
    if(a == c || a == d) return {a};
    if(b == c || b == d) return {b};
    return std::nullopt;
}

std::optional<Point2D> IntersectionVisitor::operator()(const Line& line, const Circle& circle) {
    auto x0 = circle.center_p.x, y0 = circle.center_p.y;
    auto x1 = line.start.x, y1 = line.start.y;
    auto x2 = line.end.x, y2 = line.end.y;
    auto r = circle.radius;

    // Ax + By + C = 0
    auto A = y1 - y2;
    auto B = x2 - x1;
    auto C = x1 * y2 - x2 * y1;
    
    // Ax' + By' + C' = 0
    auto C_prime = A * x0 + B * y0 + C;

    auto points = getLineAndCircleIntersect(A, B, C_prime, r);

    std::for_each(points.begin(), points.end(), [&circle](auto& p){ p = p + circle.center_p; });

    for(const auto& p : points) {
        if(isPointInSegment(p, line)) {
            return {p};
        }
    }
    return std::nullopt;
}

std::optional<Point2D> IntersectionVisitor::operator()(const Circle& circle, const Line& line) {
    return operator()(line, circle);
}

std::optional<Point2D> IntersectionVisitor::operator()(const Circle& circle1, const Circle& circle2) {
    const auto& c1 = circle1.center_p, c2 = circle2.center_p;
    auto r1 = circle1.radius, r2 = circle2.radius;
    if(c1 == c2) {
        return std::nullopt;
    }
    
    Point2D c1_prime{};
    auto c2_prime = c2 - c1;
    auto x2_prime = c2_prime.x, y2_prime = c2_prime.y;

    // Ax + By + C = 0
    auto A = -2 * x2_prime;
    auto B = -2 * y2_prime;
    auto C = x2_prime * x2_prime + y2_prime * y2_prime + r1 * r1 - r2 * r2;

    auto points = getLineAndCircleIntersect(A, B, C, r1);
    std::for_each(points.begin(), points.end(), [&c1](auto& p){ p = p + c1; });

    return points.empty() ? std::nullopt : std::optional{points[0]};
}

std::optional<Point2D> IntersectionVisitor::operator()(const Shape&, const Shape&) {
    throw std::logic_error("Intersection for given shapes is not supported");
}


}  // namespace geometry::intersections