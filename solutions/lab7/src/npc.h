#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <mutex>
#include <shared_mutex>

struct NPC;
struct Bear;
struct Vup;
struct Vykhuhol;
using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType { Unknown = 0, BearType = 1, VupType = 2, VykhuholType = 3 };

struct IFightObserver {
    virtual void on_fight(const std::shared_ptr<NPC> attacker, int attack_roll,
                          const std::shared_ptr<NPC> defender, int defense_roll,
                          bool win) = 0;
};

class Visitor {
public:
    virtual bool visit(Bear &bear) = 0;
    virtual bool visit(Vup &vup) = 0;
    virtual bool visit(Vykhuhol &vykhuhol) = 0;
    virtual ~Visitor() = default;
};

struct NPC : public std::enable_shared_from_this<NPC> {
protected:
    mutable std::mutex mtx;  

    NpcType type;
    int x{0};
    int y{0};
    bool alive{true};

    std::vector<std::shared_ptr<IFightObserver>> observers;

public:
    NPC(NpcType t, int _x, int _y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<IFightObserver> observer);
    
    void fight_notify(const std::shared_ptr<NPC> defender, int attack_roll, int defense_roll, bool win);
    
    bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;

    virtual bool accept(Visitor &visitor) = 0;

    virtual void print() = 0;
    std::pair<int, int> position() const;
    NpcType get_type() const;

    virtual void save(std::ostream &os);

    virtual void move() = 0; 
    virtual size_t get_kill_distance() const = 0; 
    bool is_alive() const; 
    void die(); 

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};
