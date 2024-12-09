#pragma once
#include "npc.h"

struct Bear : public NPC {
    Bear(int x, int y);
    Bear(std::istream &is);

    void print() override;

    bool accept(Visitor &visitor) override;

    void save(std::ostream &os) override;

    void move() override; 

    size_t get_kill_distance() const override { return 10; } 

    friend std::ostream &operator<<(std::ostream &os, Bear &bear);
};
