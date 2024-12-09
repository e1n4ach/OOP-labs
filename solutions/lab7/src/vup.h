#pragma once
#include "npc.h"

struct Vup : public NPC {
    Vup(int x, int y);
    Vup(std::istream &is);

    void print() override;

    bool accept(Visitor &visitor) override;

    void save(std::ostream &os) override;

    void move() override; 

    size_t get_kill_distance() const override { return 10; } 

    friend std::ostream &operator<<(std::ostream &os, Vup &vup);
};
