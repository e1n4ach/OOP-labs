#include "npc.h"

NPC::NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {}

NPC::NPC(NpcType t, std::istream &is) : type(t) {
    is >> x;
    is >> y;
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer) {
    std::lock_guard<std::mutex> lock(mtx);
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, int attack_roll, int defense_roll, bool win) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto &o : observers) {
        o->on_fight(shared_from_this(), attack_roll, defender, defense_roll, win);
    }
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    std::pair<int, int> pos = other->position();
    double dist_sq = std::pow(static_cast<double>(x - pos.first), 2) +
                     std::pow(static_cast<double>(y - pos.second), 2);
    return dist_sq <= std::pow(static_cast<double>(distance), 2);
}

std::pair<int, int> NPC::position() const {
    std::lock_guard<std::mutex> lock(mtx);
    return {x, y};
}

NpcType NPC::get_type() const {
    std::lock_guard<std::mutex> lock(mtx);
    return type;
}

void NPC::save(std::ostream &os) {
    std::lock_guard<std::mutex> lock(mtx);
    os << x << " " << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc) {
    os << "{ x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}

bool NPC::is_alive() const {
    std::lock_guard<std::mutex> lock(mtx);
    return alive;
}

void NPC::die() {
    std::lock_guard<std::mutex> lock(mtx);
    alive = false;
}
