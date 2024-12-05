#include <string>
#include "gtest/gtest.h"
#include "../src/solution.h"

TEST(IsValidParenthesesTest, BalancedParentheses) {
    EXPECT_TRUE(IsValidParentheses("()"));
    EXPECT_TRUE(IsValidParentheses("(())((()))()"));
    EXPECT_TRUE(IsValidParentheses(""));
}

TEST(IsValidParenthesesTest, UnbalancedParentheses) {
    EXPECT_FALSE(IsValidParentheses(")("));
    EXPECT_FALSE(IsValidParentheses("(()))"));
    EXPECT_FALSE(IsValidParentheses("("));
}

TEST(IsValidParenthesesTest, EdgeCases) {
    EXPECT_TRUE(IsValidParentheses("(((((((((())))))))))"));
    EXPECT_FALSE(IsValidParentheses("(((((((((("));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}