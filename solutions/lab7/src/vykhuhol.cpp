#include "vykhuhol.h"
#include "bear.h"
#include "vup.h"

Vykhuhol::Vykhuhol(int x, int y) : NPC(VykhuholType, x, y) {}
Vykhuhol::Vykhuhol(std::istream &is) : NPC(VykhuholType, is) {}

void Vykhuhol::print() { 
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Vykhuhol at (" << x << ", " << y << ")\n"; 
}

bool Vykhuhol::accept(Visitor &visitor) { 
    return visitor.visit(*this); 
}

void Vykhuhol::save(std::ostream &os) {
    std::lock_guard<std::mutex> lock(mtx);
    os << VykhuholType << " " << x << " " << y << std::endl;
}

void Vykhuhol::move() {
    int dx = (std::rand() % 11) - 5; 
    int dy = (std::rand() % 11) - 5; 
    {
        std::lock_guard<std::mutex> lock(mtx);
        x += dx;
        y += dy;
    }
}

std::ostream &operator<<(std::ostream &os, Vykhuhol &vykhuhol) {
    os << "Vykhuhol: (" << vykhuhol.x << ", " << vykhuhol.y << ")";
    return os;
}
