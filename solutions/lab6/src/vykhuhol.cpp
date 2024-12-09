#include "vykhuhol.h"
#include "bear.h"
#include "vup.h"

Vykhuhol::Vykhuhol(int x, int y) : NPC(VykhuholType, x, y) {}
Vykhuhol::Vykhuhol(std::istream &is) : NPC(VykhuholType, is) {}

void Vykhuhol::print() { std::cout << *this; }

bool Vykhuhol::accept(Visitor &visitor) { return visitor.visit(*this); }

bool Vykhuhol::fight(std::shared_ptr<Bear> other) {
    fight_notify(other, true);
    return true;
}

bool Vykhuhol::fight(std::shared_ptr<Vup> other) {
    fight_notify(other, false);
    return false;
}

bool Vykhuhol::fight(std::shared_ptr<Vykhuhol> other) {
    fight_notify(other, false);
    return false;
}

void Vykhuhol::save(std::ostream &os) {
    os << VykhuholType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Vykhuhol &vykhuhol) {
    os << "Vykhuhol: " << *static_cast<NPC *>(&vykhuhol) << std::endl;
    return os;
}
