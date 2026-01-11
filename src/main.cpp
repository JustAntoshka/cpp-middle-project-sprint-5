#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

bool IsIntersectionSupported(const Shape& shape1, const Shape& shape2) {
    return (holds_alternative<Line>(shape1) || holds_alternative<Circle>(shape1))
        && (holds_alternative<Line>(shape2) || holds_alternative<Circle>(shape2));
}

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");
    
    auto supported = others | views::filter([&shape](const auto& other){
        return IsIntersectionSupported(shape, other);
    });

    rng::for_each(supported, [&shape](const auto& other){
        intersections::GetIntersectPoint(shape, other).transform([&shape, &other](const auto& point){
            std::println("Intersection between shapes {} and {} is found at point {}", shape, other, point);
            return point;
        }).or_else([&shape, &other](){
            std::println("Shapes {} and {} are not intersected", shape, other);
            return std::optional<Point2D>{};
        });
    });
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    rng::for_each(shapes | views::take(5), [&p](const auto& shape){
        std::println("Distance from point {} to shape {} equals to {}", p, shape, queries::DistanceToPoint(shape, p));
    });
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    rng::for_each(utils::FindAllCollisions(shapes), [](const auto& shape_pair){
        std::println("Shapes {} and {} are overlapped", shape_pair.first, shape_pair.second);
    });
    
    utils::FindHighestShape(shapes).transform([](const auto& height){
        std::println("The highest shape height equals to {}", height);
        return height;
    }).or_else([](){
        std::println("The highest shape has not been found");
        return std::optional<size_t>{};
    });

    const auto Size = shapes.size();
    
    auto distance_view = views::iota(std::size_t{0}, Size)
        | views::transform([&](size_t i){ return views::iota(i + 1, Size)
           | views::transform([&](size_t j){ return std::tuple{i, j, queries::DistanceBetweenShapes(shapes[i], shapes[j])}; });
        })
        | views::join;

    auto it = rng::find_if(distance_view, [](const auto& distance){ return std::get<2>(distance).has_value(); });
    if(it != distance_view.end()) {
        const auto& shape1 = shapes[std::get<0>(*it)];
        const auto& shape2 = shapes[std::get<1>(*it)];
        const auto& distance = std::get<2>(*it).value();
        std::println("The distance between shapes {} and {} equals to {}", shape1, shape2, distance);
    } else {
        std::println("No distance between shapes was found");
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    auto filtered = shapes | views::filter([](const auto& shape){
        return queries::GetBoundBox(shape).min_y > 50.0;
    });

    std::println("Shapes higher that 50:");
    rng::for_each(filtered | views::take(3), [](const auto& shape){
        std::println("\t{}", shape);
    });
    
    auto min_it = rng::min_element(shapes, rng::less{}, [](const auto& shape){ return queries::GetHeight(shape); });
    if(min_it != shapes.end()) {
        std::println("Shape with mininmal height: {}", *min_it);
    } else {
        std::println("Shape with mininmal height was not found");
    }

    auto max_it = rng::max_element(shapes, rng::less{}, [](const auto& shape){ return queries::GetHeight(shape); });
    if(max_it != shapes.end()) {
        std::println("Shape with maximal height: {}", *max_it);
    } else {
        std::println("Shape with maximal height was not found");
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    /* ваш код здесь */

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема 
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    /* ваш код здесь */

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //
    }
    return 0;
}