#pragma once
#include "npc.h"
#include "vup.h"       
#include "vykhuhol.h"  

struct Bear : public NPC {
    Bear(int x, int y);
    Bear(std::istream &is);

    void print() override;

    bool accept(Visitor &visitor) override;

    bool fight(std::shared_ptr<Bear> other) override;
    bool fight(std::shared_ptr<Vup> other) override;
    bool fight(std::shared_ptr<Vykhuhol> other) override;

    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Bear &bear);
};
