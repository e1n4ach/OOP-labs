#include "npc.h"

NPC::NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {}

NPC::NPC(NpcType t, std::istream &is) : type(t) {
    is >> x;
    is >> y;
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer) {
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win) {
    for (auto &o : observers) {
        o->on_fight(shared_from_this(), defender, win);
    }
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    return (std::pow(static_cast<double>(x - other->x), 2) +
            std::pow(static_cast<double>(y - other->y), 2)) <=
           std::pow(static_cast<double>(distance), 2);
}

void NPC::save(std::ostream &os) {
    os << x << std::endl;
    os << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc) {
    os << "{ x:" << npc.x << ", y:" << npc.y << " }";
    return os;
}
