#include "../src/solution.h"
#include "gtest/gtest.h"

TEST(RectangleTest, AreaCalculation) {
    Rectangle<double> rect(Point<double>(0, 0), 4.0, 5.0);
    ASSERT_EQ(static_cast<double>(rect), 20.0);
}

TEST(RectangleTest, BarycenterCalculation) {
    Rectangle<double> rect(Point<double>(0, 0), 4.0, 6.0);
    Point<double> barycenter = rect.get_barycenter();
    ASSERT_NEAR(barycenter.get_x(), 2.0, 0.001);
    ASSERT_NEAR(barycenter.get_y(), 3.0, 0.001);
}

TEST(TrapezoidTest, AreaCalculation) {
    Trapezoid<double> trap({Point<double>(0, 0), Point<double>(10, 0), Point<double>(8, 6), Point<double>(2, 6)});
    ASSERT_NEAR(static_cast<double>(trap), 48.0, 0.001);
}

TEST(RhombusTest, AreaCalculation) {
    Rhombus<double> rhombus(Point<double>(0, 0), 6.0, 8.0);
    ASSERT_NEAR(static_cast<double>(rhombus), 24.0, 0.001);
}

TEST(RhombusTest, BarycenterCalculation) {
    Rhombus<double> rhombus(Point<double>(0, 0), 6.0, 8.0);
    Point<double> barycenter = rhombus.get_barycenter();
    ASSERT_NEAR(barycenter.get_x(), 0.0, 0.001);
    ASSERT_NEAR(barycenter.get_y(), 0.0, 0.001);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
