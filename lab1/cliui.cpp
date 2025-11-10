#include "cliui.h"


CLIUI::CLIUI() {}

CLIUI::~CLIUI() {}

void CLIUI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void CLIUI::pause() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void CLIUI::printHeader(const std::string& title) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << std::left << std::setw(54) << title << "\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
}

void CLIUI::printSeparator() {
    std::cout << "─────────────────────────────────────────────────────────\n";
}

std::string CLIUI::inputMultisetName(const std::string& prompt) {
    std::string name;
    std::cout << prompt;
    std::cin >> name;
    return name;
}

Multiset* CLIUI::getMultiset(const std::string& name) {
    auto it = multisets.find(name);
    if (it == multisets.end()) {
        return nullptr;
    }
    return it->second.get();
}

bool CLIUI::hasUniverse() const {
    return universe != nullptr;
}

bool CLIUI::hasMultiset(const std::string& name) const {
    return multisets.find(name) != multisets.end();
}

void CLIUI::run() {
    while (true) {
        showMainMenu();
    }
}

void CLIUI::showMainMenu() {
    clearScreen();

    if (!hasUniverse()) {
        std::cout << "  Универсум не создан!\n\n";
    }

    printSeparator();
    std::cout << "Главное меню:\n";
    printSeparator();

    std::cout << "  [1] Создать универсум\n";
    std::cout << "  [2] Создать мультимножество\n";
    std::cout << "  [3] Просмотреть мультимножества\n";
    std::cout << "  [4] Операции над множествами\n";
    std::cout << "  [5] Арифметические операции\n";
    std::cout << "  [6] Сравнить мультимножества\n";
    std::cout << "  [7] Показать все операции (сводка)\n";
    std::cout << "  [0] Выход\n";

    printSeparator();
    std::cout << "Ваш выбор: ";

    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n Ошибка ввода!\n";
        pause();
        return;
    }

    switch (choice) {
        case 1: createUniverseMenu(); break;
        case 2: createMultisetMenu(); break;
        case 3: viewMultisetsMenu(); break;
        case 4: operationsMenu(); break;
        case 5: arithmeticOperationsMenu(); break;
        case 6: compareMultisetsMenu(); break;
        case 7: showAllOperations(); break;
        case 0:
            std::cout << "\n До свидания!\n";
            exit(0);
        default:
            std::cout << "\n Неверный выбор!\n";
            pause();
    }
}

void CLIUI::createUniverseMenu() {
    clearScreen();
    printHeader("СОЗДАНИЕ УНИВЕРСУМА");

    bool isResetUniverse = false;

    if (hasUniverse()) {
        std::cout << "  Универсум уже существует!\n";
        std::cout << "Создать новый? (y/n): ";
        char confirm;
        std::cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            return;
        }
        isResetUniverse = true;
    }

    int depth, maxMult;

    std::cout << "Введите разрядность кода Грея (1-10): ";
    if (!(std::cin >> depth) || depth < 0 || depth > 10) {
        std::cout << "\n Некорректная разрядность!\n";
        pause();
        return;
    }

    if (depth > 0) {
        std::cout << "Введите максимальную кратность элементов: ";
        if (!(std::cin >> maxMult) || maxMult < 0) {
            std::cout << "\n Некорректная кратность!\n";
            pause();
            return;
        }
    }

    try {
        if (isResetUniverse) {
            universe.reset();
            multisets.clear();
            std::cout << " Старый универсум удалён\n\n";
        }

        universe = std::make_unique<Universe>(depth, maxMult);
        universe->printTable();
        pause();
    } catch (const std::exception& e) {
        std::cout << "\n Ошибка: " << e.what() << "\n";
        pause();
    }
}

void CLIUI::createMultisetMenu() {
    clearScreen();
    printHeader("СОЗДАНИЕ МУЛЬТИМНОЖЕСТВА");

    if (!hasUniverse()) {
        std::cout << " Универсум не создан!\n";
        pause();
        return;
    }

    std::string name = inputMultisetName("Введите имя мультимножества: ");

    if (hasMultiset(name)) {
        std::cout << " Мультимножество с таким именем уже существует!\n";
        pause();
        return;
    }

    auto ms = std::make_unique<Multiset>(*universe);

    if (ms->getDepth() == 0) {
        std::cout << "  Создано пустое мультимножество\n";
        multisets[name] = std::move(ms);
        pause();
        return;
    }

    std::cout << "\nВыберите способ заполнения:\n";
    std::cout << "  [1] Вручную\n";
    std::cout << "  [2] Автоматически (случайно)\n";
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;

    try {

        if (choice == 1) {
            int size;
            std::cout << "Количество уникальных элементов (1-"
                      << universe->size() << "): ";
            std::cin >> size;
            ms->fillManual(size);
        } else if (choice == 2) {
            ms->fillRandom();
        } else {
            std::cout << " Неверный выбор!\n";
            pause();
            return;
        }

        std::cout << "\n Мультимножество '" << name << "' создано:\n";
        ms->printTable();

        multisets[name] = std::move(ms);
        pause();

    } catch (const std::exception& e) {
        std::cout << "\n Ошибка: " << e.what() << "\n";
        pause();
    }
}

void CLIUI::viewMultisetsMenu() {
    clearScreen();
    printHeader("ПРОСМОТР МУЛЬТИМНОЖЕСТВ");

    if (multisets.empty()) {
        std::cout << "  Мультимножества не созданы!\n";
        pause();
        return;
    }

    std::cout << "Доступные мультимножества:\n\n";

    for (const auto& [name, ms] : multisets) {
        std::cout << "-> " << name << ":\n";
        ms->printTable();
        std::cout << "\n";
    }

    pause();
}

void CLIUI::operationsMenu() {
    clearScreen();
    printHeader("ОПЕРАЦИИ НАД МНОЖЕСТВАМИ");

    if (multisets.size() < 1) {
        std::cout << " Создайте хотя бы одно мультимножество!\n";
        pause();
        return;
    }

    std::cout << "Выберите операцию:\n";
    std::cout << "  [1] Объединение (A ∪ B)\n";
    std::cout << "  [2] Пересечение (A ∩ B)\n";
    std::cout << "  [3] Разность (A \\ B)\n";
    std::cout << "  [4] Симметрическая разность (A △ B)\n";
    std::cout << "  [5] Дополнение (A')\n";
    std::cout << "  [0] Назад\n";
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1: performBinaryOperation("Объединение", &Multiset::unionWith); break;
        case 2: performBinaryOperation("Пересечение", &Multiset::intersectionWith); break;
        case 3: performBinaryOperation("Разность", &Multiset::differenceWith); break;
        case 4: performBinaryOperation("Симметрическая разность", &Multiset::symmetricDifferenceWith); break;
        case 5: performUnaryOperation("Дополнение", &Multiset::complement); break;
        case 0: return;
        default:
            std::cout << " Неверный выбор!\n";
            pause();
    }
}

void CLIUI::arithmeticOperationsMenu() {
    clearScreen();
    printHeader("АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ");

    if (multisets.size() < 1) {
        std::cout << " Создайте хотя бы одно мультимножество!\n";
        pause();
        return;
    }

    std::cout << "Выберите операцию:\n";
    std::cout << "  [1] Сумма (A + B)\n";
    std::cout << "  [2] Разность (A - B)\n";
    std::cout << "  [3] Произведение (A * B)\n";
    std::cout << "  [4] Деление (A / B)\n";
    std::cout << "  [0] Назад\n";
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1: performBinaryOperation("Арифметическая сумма", &Multiset::arithmeticSum); break;
        case 2: performBinaryOperation("Арифметическая разность", &Multiset::arithmeticDifference); break;
        case 3: performBinaryOperation("Арифметическое произведение", &Multiset::arithmeticProduct); break;
        case 4: performBinaryOperation("Арифметическое деление", &Multiset::arithmeticDivision); break;
        case 0: return;
        default:
            std::cout << " Неверный выбор!\n";
            pause();
    }
}

void CLIUI::performBinaryOperation(
    const std::string& opName,
    Multiset (Multiset::*operation)(const Multiset&) const
) {
    if (multisets.size() < 2) {
        std::cout << "\n Нужно минимум два мультимножества!\n";
        pause();
        return;
    }

    std::cout << "\nДоступные мультимножества: ";
    for (const auto& [name, _] : multisets) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    std::string nameA = inputMultisetName("\nПервое мультимножество (A): ");
    std::string nameB = inputMultisetName("Второе мультимножество (B): ");

    Multiset* A = getMultiset(nameA);
    Multiset* B = getMultiset(nameB);

    if (!A || !B) {
        std::cout << " Одно из мультимножеств не найдено!\n";
        pause();
        return;
    }

    std::cout << "\n" << opName << ":\n\n";

    std::cout << "A = \n";
    A->printTable();
    std::cout << "\nB = \n";
    B->printTable();

    Multiset result = (A->*operation)(*B);

    std::cout << "\nРезультат:\n";
    result.printTable();

    std::cout << "\nСохранить результат? (y/n): ";
    char save;
    std::cin >> save;

    if (save == 'y' || save == 'Y') {
        std::string resultName = inputMultisetName("Имя мультимножества для результата: ");
        if (hasMultiset(resultName)) {
            std::cout << "  Перезаписать существующее? (y/n): ";
            char confirm;
            std::cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') {
                pause();
                return;
            }
        }
        multisets[resultName] = std::make_unique<Multiset>(result);
        std::cout << " Результат сохранён как '" << resultName << "'\n";
    }

    pause();
}

void CLIUI::performUnaryOperation(
    const std::string& opName,
    Multiset (Multiset::*operation)() const
) {
    std::cout << "\nДоступные мультимножества: ";
    for (const auto& [name, _] : multisets) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    std::string nameA = inputMultisetName("\nМультимножество: ");
    Multiset* A = getMultiset(nameA);

    if (!A) {
        std::cout << " Мультимножество не найдено!\n";
        pause();
        return;
    }

    std::cout << "\n" << opName << ":\n\n";

    std::cout << "A = ";
    A->printTable();

    Multiset result = (A->*operation)();

    std::cout << "\nРезультат:\n";
    result.printTable();

    std::cout << "\nСохранить результат? (y/n): ";
    char save;
    std::cin >> save;

    if (save == 'y' || save == 'Y') {
        std::string resultName = inputMultisetName("Имя мультимножества для результата: ");
        multisets[resultName] = std::make_unique<Multiset>(result);
        std::cout << " Результат сохранён как '" << resultName << "'\n";
    }

    pause();
}

void CLIUI::compareMultisetsMenu() {
    clearScreen();
    printHeader("СРАВНЕНИЕ МУЛЬТИМНОЖЕСТВ");

    if (multisets.size() < 2) {
        std::cout << " Нужно минимум два мультимножества!\n";
        pause();
        return;
    }

    std::cout << "Доступные мультимножества: ";
    for (const auto& [name, _] : multisets) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    std::string nameA = inputMultisetName("\nПервое мультимножество: ");
    std::string nameB = inputMultisetName("Второе мультимножество: ");

    Multiset* A = getMultiset(nameA);
    Multiset* B = getMultiset(nameB);

    if (!A || !B) {
        std::cout << " Одно из мультимножеств не найдено!\n";
        pause();
        return;
    }

    std::cout << "\n";
    A->printTable();
    std::cout << "\n";
    B->printTable();

    std::cout << "\nРезультат сравнения:\n";
    if (*A == *B) {
        std::cout << " Мультимножества равны (A == B)\n";
    } else {
        std::cout << " Мультимножества не равны (A != B)\n";
    }

    pause();
}

void CLIUI::showAllOperations() {
    clearScreen();
    printHeader("СВОДКА ВСЕХ ОПЕРАЦИЙ");

    if (multisets.size() < 2) {
        std::cout << " Нужно минимум два мультимножества!\n";
        pause();
        return;
    }

    std::cout << "Доступные мультимножества: ";
    for (const auto& [name, _] : multisets) {
        std::cout << name << " ";
    }
    std::cout << "\n";

    std::string nameA = inputMultisetName("\nПервое мультимножество (A): ");
    std::string nameB = inputMultisetName("Второе мультимножество (B): ");

    Multiset* A = getMultiset(nameA);
    Multiset* B = getMultiset(nameB);

    if (!A || !B) {
        std::cout << " Одно из мультимножеств не найдено!\n";
        pause();
        return;
    }

    clearScreen();
    printHeader("РЕЗУЛЬТАТЫ ВСЕХ ОПЕРАЦИЙ");

    std::cout << "A = \n";
    A->printTable();
    std::cout << "\nB = \n";
    B->printTable();

    printSeparator();
    std::cout << "Операции над мультимножествами:\n";
    printSeparator();

    std::cout << "\n1. Объединение (A ∪ B):\n";
    A->unionWith(*B).printTable();

    std::cout << "\n2. Пересечение (A ∩ B):\n";
    A->intersectionWith(*B).printTable();

    std::cout << "\n3. Разность (A \\ B):\n";
    A->differenceWith(*B).printTable();

    std::cout << "\n4. Разность (B \\ A):\n";
    B->differenceWith(*A).printTable();

    std::cout << "\n5. Симметрическая разность (A △ B):\n";
    A->symmetricDifferenceWith(*B).printTable();

    std::cout << "\n6. Дополнение A':\n";
    A->complement().printTable();

    std::cout << "\n7. Дополнение B':\n";
    B->complement().printTable();

    printSeparator();
    std::cout << "Арифметические операции:\n";
    printSeparator();

    std::cout << "\n8. Сумма (A + B):\n";
    A->arithmeticSum(*B).printTable();

    std::cout << "\n9. Разность (A - B):\n";
    A->arithmeticDifference(*B).printTable();

    std::cout << "\n10. Произведение (A * B):\n";
    A->arithmeticProduct(*B).printTable();

    std::cout << "\n11. Деление (A / B):\n";
    A->arithmeticDivision(*B).printTable();

    printSeparator();

    pause();
}
