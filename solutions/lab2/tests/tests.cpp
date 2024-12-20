#include "../src/solution.h"
#include "gtest/gtest.h"

TEST(ElevenBehavior, Addition) {
    Eleven e1("9A");
    Eleven e2("12");
    Eleven e3 = e1 + e2;
    ASSERT_EQ(e3, Eleven("101")); 
}

TEST(ElevenBehavior, Subtraction) {
    Eleven e1("9A");
    Eleven e2("12");
    Eleven e3 = e1 - e2;
    ASSERT_EQ(e3, Eleven("88"));
}

TEST(ElevenBehavior, InvalidInput) {
    ASSERT_THROW(Eleven e1("G1"), std::invalid_argument);
    ASSERT_THROW(Eleven e2({'1', 'G', '2'}), std::invalid_argument);
}

TEST(ElevenBehavior, Comparison) {
    Eleven e1("9A");
    Eleven e2("12");
    ASSERT_TRUE(e1 > e2);
    ASSERT_TRUE(e2 < e1);
    ASSERT_FALSE(e1 == e2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}