#include "src/solution.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<Figure *> figures;
    char command;

    while (true) {
        std::cout << "Enter command (r - rectangle, t - trapezoid, h - rhombus, p - print, d - delete, e - exit): ";
        std::cin >> command;

        if (command == 'e') {
            break;
        } else if (command == 'p') {
            double totalArea = 0;
            for (size_t i = 0; i < figures.size(); ++i) {
                std::cout << "Figure " << i + 1 << ": " << *figures[i]
                          << ", Barycenter: " << figures[i]->get_barycenter()
                          << ", Area: " << static_cast<double>(*figures[i]) << '\n';
                totalArea += static_cast<double>(*figures[i]);
            }
            std::cout << "Total area: " << totalArea << "\n\n";
            continue;
        } else if (command == 'd') {
            size_t index;
            std::cout << "Enter index to delete (1-based): ";
            std::cin >> index;
            if (index == 0 || index > figures.size()) {
                std::cout << "Invalid index.\n";
                continue;
            }
            delete figures[index - 1];
            figures.erase(figures.begin() + index - 1);
            std::cout << "Figure deleted.\n";
            continue;
        }

        try {
            if (command == 'r') {
                Point start;
                double width, height;
                std::cout << "Enter start point (x y): ";
                std::cin >> start;
                std::cout << "Enter width and height: ";
                std::cin >> width >> height;
                figures.push_back(new Rectangle(start, width, height));
            } else if (command == 't') {
                std::initializer_list<Point> trapezoidPoints;
                Point p1, p2, p3, p4;
                std::cout << "Enter 4 points for trapezoid (x1 y1 x2 y2 x3 y3 x4 y4): ";
                std::cin >> p1 >> p2 >> p3 >> p4;
                trapezoidPoints = {p1, p2, p3, p4};
                figures.push_back(new Trapezoid(trapezoidPoints));
            } else if (command == 'h') {
                Point center;
                double diag1, diag2;
                std::cout << "Enter center point (x y): ";
                std::cin >> center;
                std::cout << "Enter diagonals: ";
                std::cin >> diag1 >> diag2;
                figures.push_back(new Rhombus(center, diag1, diag2));
            } else {
                std::cout << "Invalid command.\n";
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    for (Figure *figure : figures) {
        delete figure;
    }

    return 0;
}
