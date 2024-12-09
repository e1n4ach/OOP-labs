#include "../src/bear.h"
#include "../src/vup.h"
#include "../src/vykhuhol.h"
#include "../src/npc.h"
#include "../src/factory.h"
#include "../src/visitor.h"
#include "../src/observers.h"
#include "../src/fight_manager.h"

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <chrono> 

using namespace std::chrono_literals;

std::mutex print_mutex;
std::mutex cout_mutex;
std::shared_mutex npc_array_mutex;
std::atomic<bool> stop_game{false};

class MockFightObserver : public IFightObserver {
public:
    int fight_count = 0;
    std::shared_ptr<NPC> last_attacker;
    std::shared_ptr<NPC> last_defender;
    bool last_win;

    void on_fight(const std::shared_ptr<NPC> attacker, int attack_roll,
                  const std::shared_ptr<NPC> defender, int defense_roll,
                  bool win) override {
        fight_count++;
        last_attacker = attacker;
        last_defender = defender;
        last_win = win;
    }
};

class MockVisitor : public Visitor {
public:
    bool bear_visited = false;
    bool vup_visited = false;
    bool vykhuhol_visited = false;

    bool visit(Bear &bear) override {
        bear_visited = true;
        return true;
    }

    bool visit(Vup &vup) override {
        vup_visited = true;
        return true;
    }

    bool visit(Vykhuhol &vykhuhol) override {
        vykhuhol_visited = true;
        return true;
    }
};

TEST(ObserverTest, FightNotification) {
    auto mockObserver = std::make_shared<MockFightObserver>();
    auto bear = std::make_shared<Bear>(0, 0);
    auto vup = std::make_shared<Vup>(0, 0);

    bear->subscribe(mockObserver);
    bear->fight_notify(vup, 5, 3, true); 

    ASSERT_EQ(mockObserver->fight_count, 1);
    ASSERT_EQ(mockObserver->last_attacker, bear);
    ASSERT_EQ(mockObserver->last_defender, vup);
    ASSERT_TRUE(mockObserver->last_win);
}

TEST(FightTest, BearEatsVup) {
    auto bear = std::make_shared<Bear>(0, 0);
    auto vup = std::make_shared<Vup>(0, 0);

    MockVisitor mock_visitor;
    ASSERT_TRUE(bear->accept(mock_visitor));

    ASSERT_TRUE(mock_visitor.bear_visited);
}

TEST(FightTest, BearDoesNotEatBear) {
    auto bear1 = std::make_shared<Bear>(0, 0);
    auto bear2 = std::make_shared<Bear>(0, 0);

    MockVisitor mock_visitor;
    ASSERT_TRUE(bear1->accept(mock_visitor));

    ASSERT_TRUE(mock_visitor.bear_visited);

    MockVisitor mock_visitor2;
    ASSERT_TRUE(bear2->accept(mock_visitor2));
    ASSERT_TRUE(mock_visitor2.bear_visited);
}

TEST(FightTest, VupDoesNotHarmAnyone) {
    auto vup = std::make_shared<Vup>(0, 0);
    auto bear = std::make_shared<Bear>(0, 0);
    auto vykhuhol = std::make_shared<Vykhuhol>(0, 0);

    MockVisitor mock_visitor;
    ASSERT_TRUE(vup->accept(mock_visitor));

    ASSERT_TRUE(mock_visitor.vup_visited);
}

TEST(FightTest, VykhuholKillsBear) {
    auto vykhuhol = std::make_shared<Vykhuhol>(0, 0);
    auto bear = std::make_shared<Bear>(0, 0);

    MockVisitor mock_visitor;
    ASSERT_TRUE(vykhuhol->accept(mock_visitor));

    ASSERT_TRUE(mock_visitor.vykhuhol_visited);
}

TEST(FightTest, DistanceCheck) {
    auto bear1 = std::make_shared<Bear>(0, 0);
    auto bear2 = std::make_shared<Bear>(3, 4);

    ASSERT_TRUE(bear1->is_close(bear2, 5));
    ASSERT_FALSE(bear1->is_close(bear2, 4));
}

TEST(SaveLoadTest, SaveAndLoad) {
    set_t array;
    array.insert(std::make_shared<Bear>(100, 200));
    array.insert(std::make_shared<Vup>(300, 400));
    array.insert(std::make_shared<Vykhuhol>(500, 600));

    save(array, "test_save.txt");

    set_t loaded_array = load("test_save.txt");

    ASSERT_EQ(array.size(), loaded_array.size());

    std::remove("test_save.txt");
}

TEST(FightManagerTest, AddAndProcessFightEvent) {
    FightManager fm;
    std::atomic<bool> processed{false};

    auto attacker = std::make_shared<Bear>(0, 0);
    auto defender = std::make_shared<Vykhuhol>(1, 1);

    fm.add_event({attacker, defender});
    stop_game = false;
    std::thread fight_thread([&fm, &processed]() {
        std::optional<FightEvent> event = fm.get_event();
        if (event) {
            processed = true;
            ASSERT_TRUE(event->attacker->is_alive());
            ASSERT_TRUE(event->defender->is_alive());
        }
    });
    std::this_thread::sleep_for(200ms);
    stop_game = true;
    fm.stop_processing();
    fight_thread.join();

    ASSERT_TRUE(processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
