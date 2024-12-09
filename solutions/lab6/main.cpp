#include "./src/npc.h"
#include "./src/bear.h"
#include "./src/vup.h"
#include "./src/vykhuhol.h"
#include "./src/factory.h"
#include "./src/visitor.h"
#include "./src/observers.h"

#include <memory>
#include <set>
#include <cstdlib>
#include <ctime>
#include <iostream>

set_t fight(const set_t &array, size_t distance) {
    set_t dead_list;

    for (const auto &attacker : array) {
        for (const auto &defender : array) {
            if (attacker != defender && attacker->is_close(defender, distance)) {
                AttackerVisitor visitor(attacker);
                if (defender->accept(visitor)) {
                    dead_list.insert(defender);
                }
            }
        }
    }
    return dead_list;
}

int main() {
    set_t array;

    std::cout << "Generating ..." << std::endl;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < 10; ++i)
        array.insert(factory(static_cast<NpcType>(std::rand() % 3 + 1),
                            std::rand() % 501, std::rand() % 501)); 
    std::cout << "Saving ..." << std::endl;

    save(array, "npc.txt");

    std::cout << "Loading ..." << std::endl;
    array = load("npc.txt");

    std::cout << "Fighting ..." << std::endl << array;

    for (size_t distance = 20; (distance <= 100) && !array.empty();
         distance += 10) {
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list)
            array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "distance: " << distance << std::endl
                  << "killed: " << dead_list.size() << std::endl
                  << std::endl
                  << std::endl;
    }

    std::cout << "Survivors:" << array;

    return 0;
}
