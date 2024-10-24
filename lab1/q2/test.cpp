#include "iostream"

int main() {
    char a = '5';
    char b = '7';
    char c = static_cast<char>(a - '0' + b - '0');
    std::cout << c << std::endl;
    return 0;
}