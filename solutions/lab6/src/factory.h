#pragma once
#include "npc.h"
#include "bear.h"
#include "vup.h"
#include "vykhuhol.h"
#include "observers.h"
#include <memory>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

std::shared_ptr<NPC> factory(std::istream &is);

std::shared_ptr<NPC> factory(NpcType type, int x, int y);

void save(const set_t &array, const std::string &filename);

set_t load(const std::string &filename);

std::ostream &operator<<(std::ostream &os, const set_t &array);
