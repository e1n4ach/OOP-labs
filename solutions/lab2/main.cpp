#include "src/solution.h"
#include <iostream>

int main() {
    Eleven a("1A3");  
    Eleven b("2A");   

    std::cout << "a = " << a << std::endl; 
    std::cout << "b = " << b << std::endl; 

    Eleven c = a + b;
    std::cout << "a + b = " << c << std::endl; 

    c = a - b;
    std::cout << "a - b = " << c << std::endl; 

    return 0;
}