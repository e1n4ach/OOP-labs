#include "bear.h"
#include "vup.h"
#include "vykhuhol.h"

Bear::Bear(int x, int y) : NPC(BearType, x, y) {}
Bear::Bear(std::istream &is) : NPC(BearType, is) {}

void Bear::print() { std::cout << *this; }

bool Bear::accept(Visitor &visitor) { return visitor.visit(*this); }

bool Bear::fight(std::shared_ptr<Bear> other) {
    fight_notify(other, false);
    return false;
}

bool Bear::fight(std::shared_ptr<Vup> other) {
    fight_notify(other, true);
    return true;
}

bool Bear::fight(std::shared_ptr<Vykhuhol> other) {
    fight_notify(other, true);
    return true;
}

void Bear::save(std::ostream &os) {
    os << BearType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Bear &bear) {
    os << "Bear: " << *static_cast<NPC *>(&bear) << std::endl;
    return os;
}
