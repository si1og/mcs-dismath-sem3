#include "bdd.h"

int main() {
    std::vector<int> functionVector = {
        0, 0, 1, 0,
        1, 0, 1, 1,
        0, 0, 0, 0,
        0, 0, 1, 1
    };

    BDD func(4, functionVector);

    while (true) {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║                    МЕНЮ                        ║\n";
        std::cout << "╠════════════════════════════════════════════════╣\n";
        std::cout << "║  1. Таблица истинности                         ║\n";
        std::cout << "║  2. СДНФ                                       ║\n";
        std::cout << "║  3. СКНФ                                       ║\n";
        std::cout << "║  4. Полином Жегалкина                          ║\n";
        std::cout << "║  5. БДР                                        ║\n";
        std::cout << "║  6. Вычислить по полиному Жегалкина            ║\n";
        std::cout << "║  7. Вычислить по БДР                           ║\n";
        std::cout << "║  0. Выход                                      ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n";
        std::cout << "Выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                func.displayTable();
                break;
            case 2:
                func.displaySDNF();
                break;
            case 3:
                func.displaySKNF();
                break;
            case 4:
                func.displayPolynomial();
                break;
            case 5:
                func.displayBDD();
                break;
            case 6:
                func.interactiveEvaluate();
                break;
            case 7:
                func.interactiveEvaluateBDD();
                break;
            case 0:
                std::cout << "До свидания!\n";
                return 0;
            default:
                std::cout << "Неверный выбор!\n";
        }
    }

    return 0;
}
