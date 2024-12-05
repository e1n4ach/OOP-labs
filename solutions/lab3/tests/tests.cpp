#include "../src/solution.h"
#include "gtest/gtest.h"

TEST(RectangleTest, AreaCalculation) {
    Rectangle rect(Point(0, 0), 4.0, 5.0);
    ASSERT_EQ(static_cast<double>(rect), 20.0);
}

TEST(RectangleTest, BarycenterCalculation) {
    Rectangle rect(Point(0, 0), 4.0, 6.0);
    Point barycenter = rect.get_barycenter();
    ASSERT_NEAR(barycenter.getX(), 2.0, 0.001);
    ASSERT_NEAR(barycenter.getY(), 3.0, 0.001);
}

TEST(TrapezoidTest, AreaCalculation) {
    Trapezoid trap({Point(0, 0), Point(10, 0), Point(8, 6), Point(2, 6)});
    ASSERT_NEAR(static_cast<double>(trap), 48.0, 0.001); // Corrected expected area
}

TEST(RhombusTest, AreaCalculation) {
    Rhombus rhombus(Point(0, 0), 6.0, 8.0);
    ASSERT_NEAR(static_cast<double>(rhombus), 24.0, 0.001);
}

TEST(RhombusTest, BarycenterCalculation) {
    Rhombus rhombus(Point(0, 0), 6.0, 8.0);
    Point barycenter = rhombus.get_barycenter();
    ASSERT_NEAR(barycenter.getX(), 0.0, 0.001);
    ASSERT_NEAR(barycenter.getY(), 0.0, 0.001);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
