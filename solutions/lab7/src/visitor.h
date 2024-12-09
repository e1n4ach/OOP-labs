#pragma once
#include "npc.h"

class AttackerVisitor : public Visitor {
public:
    AttackerVisitor(const std::shared_ptr<NPC> &attacker) : attacker_(attacker) {}

    bool visit(Bear &bear) override;
    bool visit(Vup &vup) override;
    bool visit(Vykhuhol &vykhuhol) override;

private:
    std::shared_ptr<NPC> attacker_;
};
