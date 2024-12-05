#include "src/solution.h"
#include <iostream>

int main() {
    Eleven a("1A3");  // Valid base 11 number
    Eleven b("2A");   // Valid base 11 number

    std::cout << "a = " << a << std::endl; // Should output: a = 1A3
    std::cout << "b = " << b << std::endl; // Should output: b = 2A

    Eleven c = a + b;
    std::cout << "a + b = " << c << std::endl; // Should output: a + b = 222

    c = a - b;
    std::cout << "a - b = " << c << std::endl; // Should output: a - b = 17A

    return 0;
}
