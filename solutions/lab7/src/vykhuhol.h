#pragma once
#include "npc.h"

struct Vykhuhol : public NPC {
    Vykhuhol(int x, int y);
    Vykhuhol(std::istream &is);

    void print() override;

    bool accept(Visitor &visitor) override;

    void save(std::ostream &os) override;

    void move() override; 

    size_t get_kill_distance() const override { return 20; } 

    friend std::ostream &operator<<(std::ostream &os, Vykhuhol &vykhuhol);
};
