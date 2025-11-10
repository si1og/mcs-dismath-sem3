#include "cliui.h"
#include "multiset.h"
#include "universe.h"

int main() {
    try {
        Universe uni = Universe(3, 5);
        uni.printTable();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
}
