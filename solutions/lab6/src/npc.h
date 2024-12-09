#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>      
#include <memory>
#include <vector>
#include <string>
#include <set>

struct NPC;
struct Bear;
struct Vup;
struct Vykhuhol;
using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType { Unknown = 0, BearType = 1, VupType = 2, VykhuholType = 3 };

struct IFightObserver {
    virtual void on_fight(const std::shared_ptr<NPC> attacker,
                          const std::shared_ptr<NPC> defender, bool win) = 0;
};

class Visitor {
public:
    virtual bool visit(Bear &bear) = 0;
    virtual bool visit(Vup &vup) = 0;
    virtual bool visit(Vykhuhol &vykhuhol) = 0;
    virtual ~Visitor() = default;
};

struct NPC : public std::enable_shared_from_this<NPC> {
    NpcType type;
    int x{0};
    int y{0};
    std::vector<std::shared_ptr<IFightObserver>> observers;

    NPC(NpcType t, int _x, int _y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<IFightObserver> observer);
    void fight_notify(const std::shared_ptr<NPC> defender, bool win);
    virtual bool is_close(const std::shared_ptr<NPC> &other,
                          size_t distance) const;

    virtual bool accept(Visitor &visitor) = 0;

    virtual bool fight(std::shared_ptr<Bear> other) = 0;
    virtual bool fight(std::shared_ptr<Vup> other) = 0;
    virtual bool fight(std::shared_ptr<Vykhuhol> other) = 0;
    virtual void print() = 0;

    virtual void save(std::ostream &os);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};
