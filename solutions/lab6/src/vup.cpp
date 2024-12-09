#include "vup.h"
#include "bear.h"
#include "vykhuhol.h"

Vup::Vup(int x, int y) : NPC(VupType, x, y) {}
Vup::Vup(std::istream &is) : NPC(VupType, is) {}

void Vup::print() { std::cout << *this; }

bool Vup::accept(Visitor &visitor) { return visitor.visit(*this); }

bool Vup::fight(std::shared_ptr<Bear> other) {
    fight_notify(other, false);
    return false;
}

bool Vup::fight(std::shared_ptr<Vup> other) {
    fight_notify(other, false);
    return false;
}

bool Vup::fight(std::shared_ptr<Vykhuhol> other) {
    fight_notify(other, false);
    return false;
}

void Vup::save(std::ostream &os) {
    os << VupType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Vup &vup) {
    os << "Vup: " << *static_cast<NPC *>(&vup) << std::endl;
    return os;
}
