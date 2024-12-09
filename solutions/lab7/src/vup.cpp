#include "vup.h"
#include "bear.h"
#include "vykhuhol.h"

Vup::Vup(int x, int y) : NPC(VupType, x, y) {}
Vup::Vup(std::istream &is) : NPC(VupType, is) {}

void Vup::print() { 
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Vup at (" << x << ", " << y << ")\n"; 
}

bool Vup::accept(Visitor &visitor) { 
    return visitor.visit(*this); 
}

void Vup::save(std::ostream &os) {
    std::lock_guard<std::mutex> lock(mtx);
    os << VupType << " " << x << " " << y << std::endl;
}

void Vup::move() {
    int dx = (std::rand() % 101) - 50; 
    int dy = (std::rand() % 101) - 50; 
    {
        std::lock_guard<std::mutex> lock(mtx);
        x += dx;
        y += dy;
    }
}

std::ostream &operator<<(std::ostream &os, Vup &vup) {
    os << "Vup: (" << vup.x << ", " << vup.y << ")";
    return os;
}
