#include <iostream>
#include <string>
#include "src/solution.h"

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << (IsValidParentheses(input) ? "true" : "false") << std::endl;
    return 0;
}