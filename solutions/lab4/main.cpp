#include "./src/solution.h"
#include <iostream>

int main() {
    Array<std::shared_ptr<Figure<int>>> figures_int;
    figures_int.push_back(std::make_shared<Rectangle<int>>(Point<int>(0, 0), 4, 5));
    figures_int.push_back(std::make_shared<Rhombus<int>>(Point<int>(0, 0), 6, 8));

    Array<std::shared_ptr<Rhombus<double>>> rhombuses_double;
    rhombuses_double.push_back(std::make_shared<Rhombus<double>>(Point<double>(2.0, 2.0), 6.0, 8.0));

    Array<std::shared_ptr<Figure<double>>> figures_double;

    while (true) {
        char figureType;
        std::cout << "Enter figure type (r - rectangle, t - trapezoid, h - rhombus, e - exit, p - print, d - delete): ";
        std::cin >> figureType;

        if (figureType == 'e') {
            break;
        } else if (figureType == 'p') {
            double totalArea = 0;
            for (size_t i = 0; i < figures_double.get_size(); ++i) {
                std::cout << "Figure " << i + 1 << ": " << *figures_double[i]
                          << ", Barycenter: " << figures_double[i]->get_barycenter()
                          << ", Area: " << static_cast<double>(*figures_double[i]) << '\n';
                totalArea += static_cast<double>(*figures_double[i]);
            }
            std::cout << "Total area: " << totalArea << '\n';
            continue;
        } else if (figureType == 'd') {
            size_t index;
            std::cout << "Enter index to delete: ";
            std::cin >> index;

            if (index < 1 || index > figures_double.get_size()) {
                std::cout << "Invalid index." << '\n';
            } else {
                figures_double.erase(index - 1);
                std::cout << "Figure deleted.\n";
            }
            continue;
        }

        try {
            if (figureType == 'r') {
                Point<double> start;
                double width, height;
                std::cout << "Enter start point (x y): ";
                std::cin >> start;
                std::cout << "Enter width and height: ";
                std::cin >> width >> height;
                figures_double.push_back(std::make_shared<Rectangle<double>>(start, width, height));
            } else if (figureType == 't') {
                std::cout << "Enter 4 points for trapezoid (x1 y1 x2 y2 x3 y3 x4 y4): ";
                Point<double> p1, p2, p3, p4;
                std::cin >> p1 >> p2 >> p3 >> p4;
                figures_double.push_back(std::make_shared<Trapezoid<double>>(std::initializer_list<Point<double>>{p1, p2, p3, p4}));
            } else if (figureType == 'h') {
                Point<double> center;
                double diag1, diag2;
                std::cout << "Enter center point (x y): ";
                std::cin >> center;
                std::cout << "Enter diagonals: ";
                std::cin >> diag1 >> diag2;
                figures_double.push_back(std::make_shared<Rhombus<double>>(center, diag1, diag2));
            } else {
                std::cout << "Invalid figure type." << '\n';
                continue;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}
