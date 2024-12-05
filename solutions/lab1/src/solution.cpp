#include "solution.h"

bool IsValidParentheses(const std::string& s) {
    int balance = 0;
    for (char c : s) {
        if (c == '(') {
            balance++;
        } else if (c == ')') {
            balance--;
        }
        if (balance < 0) { // Если закрывающих скобок больше, чем открывающих
            return false;
        }
    }
    return balance == 0; // Проверяем, сбалансированы ли скобки
}