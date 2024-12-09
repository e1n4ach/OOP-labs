#include "bear.h"
#include "vup.h"
#include "vykhuhol.h"

Bear::Bear(int x, int y) : NPC(BearType, x, y) {}
Bear::Bear(std::istream &is) : NPC(BearType, is) {}

void Bear::print() { 
    std::lock_guard<std::mutex> lock(mtx);  
    std::cout << "Bear at (" << x << ", " << y << ")\n"; 
}

bool Bear::accept(Visitor &visitor) { return visitor.visit(*this); }

void Bear::save(std::ostream &os) {
    std::lock_guard<std::mutex> lock(mtx);
    os << BearType << " " << x << " " << y << std::endl;
}

void Bear::move() {
    int dx = (std::rand() % 11) - 5; 
    int dy = (std::rand() % 11) - 5; 
    {
        std::lock_guard<std::mutex> lock(mtx);
        x += dx;
        y += dy;
    }
}

std::ostream &operator<<(std::ostream &os, Bear &bear) {
    os << "Bear: (" << bear.x << ", " << bear.y << ")";
    return os;
}
