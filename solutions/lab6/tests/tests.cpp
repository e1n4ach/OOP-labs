#include "../src/bear.h"
#include "../src/vup.h"
#include "../src/vykhuhol.h"
#include "../src/npc.h"
#include "../src/factory.h"
#include "../src/visitor.h"
#include "../src/observers.h"

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

class MockFightObserver : public IFightObserver {
public:
    int fight_count = 0;
    std::shared_ptr<NPC> last_attacker;
    std::shared_ptr<NPC> last_defender;
    bool last_win;

    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender, bool win) override {
        fight_count++;
        last_attacker = attacker;
        last_defender = defender;
        last_win = win;
    }
};

TEST(FactoryTest, CreateBear) {
    auto bear = factory(BearType, 100, 200);
    ASSERT_NE(bear, nullptr);
    ASSERT_EQ(bear->type, BearType);
    ASSERT_EQ(bear->x, 100);
    ASSERT_EQ(bear->y, 200);
}

TEST(FactoryTest, CreateVup) {
    auto vup = factory(VupType, 300, 400);
    ASSERT_NE(vup, nullptr);
    ASSERT_EQ(vup->type, VupType);
    ASSERT_EQ(vup->x, 300);
    ASSERT_EQ(vup->y, 400);
}

TEST(FactoryTest, CreateVykhuhol) {
    auto vykhuhol = factory(VykhuholType, 500, 600);
    ASSERT_NE(vykhuhol, nullptr);
    ASSERT_EQ(vykhuhol->type, VykhuholType);
    ASSERT_EQ(vykhuhol->x, 500);
    ASSERT_EQ(vykhuhol->y, 600);
}

TEST(FactoryTest, LoadBear) {
    std::stringstream ss("1\n150\n250\n");
    auto bear = factory(ss);
    ASSERT_NE(bear, nullptr);
    ASSERT_EQ(bear->type, BearType);
    ASSERT_EQ(bear->x, 150);
    ASSERT_EQ(bear->y, 250);
}

TEST(FightTest, BearEatsVup) {
    auto bear = std::make_shared<Bear>(0, 0);
    auto vup = std::make_shared<Vup>(0, 0);

    ASSERT_TRUE(bear->fight(vup));
}

TEST(FightTest, BearDoesNotEatBear) {
    auto bear1 = std::make_shared<Bear>(0, 0);
    auto bear2 = std::make_shared<Bear>(0, 0);

    ASSERT_FALSE(bear1->fight(bear2));
}

TEST(FightTest, VupDoesNotHarmAnyone) {
    auto vup = std::make_shared<Vup>(0, 0);
    auto bear = std::make_shared<Bear>(0, 0);
    auto vykhuhol = std::make_shared<Vykhuhol>(0, 0);

    ASSERT_FALSE(vup->fight(bear));
    ASSERT_FALSE(vup->fight(vup));
    ASSERT_FALSE(vup->fight(vykhuhol));
}

TEST(FightTest, VykhuholKillsBear) {
    auto vykhuhol = std::make_shared<Vykhuhol>(0, 0);
    auto bear = std::make_shared<Bear>(0, 0);

    ASSERT_TRUE(vykhuhol->fight(bear));
}

TEST(FightTest, DistanceCheck) {
    auto bear1 = std::make_shared<Bear>(0, 0);
    auto bear2 = std::make_shared<Bear>(300, 400);

    ASSERT_TRUE(bear1->is_close(bear2, 500));
    ASSERT_TRUE(bear1->is_close(bear2, 500)); 
    ASSERT_FALSE(bear1->is_close(bear2, 499));
}

TEST(ObserverTest, FightNotification) {
    auto mockObserver = std::make_shared<MockFightObserver>();
    auto bear = std::make_shared<Bear>(0, 0);
    auto vup = std::make_shared<Vup>(0, 0);

    bear->subscribe(mockObserver);
    bear->fight(vup);

    ASSERT_EQ(mockObserver->fight_count, 1);
    ASSERT_EQ(mockObserver->last_attacker, bear);
    ASSERT_EQ(mockObserver->last_defender, vup);
    ASSERT_TRUE(mockObserver->last_win);
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
