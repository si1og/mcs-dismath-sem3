#include "cliui.h"
#include <iomanip>

CLIUI::CLIUI() {}

CLIUI::~CLIUI() {}

void CLIUI::clearScreen() {
    system("clear");
}

void CLIUI::pause() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void CLIUI::printHeader(const std::string& title) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << std::left << std::setw(54) << title << " ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
}

void CLIUI::printSeparator() {
    std::cout << "──────────────────────────────────────────────────────────\n";
}

bool CLIUI::hasSystem() const {
    return ops != nullptr;
}

std::string CLIUI::inputNumber(const std::string& prompt) {
    std::string num;
    std::cout << prompt;
    std::cin >> num;

    if (!ops->isValidNumber(num)) {
        std::cout << "  ⚠ Некорректное число! Допустимые символы: ";
        for (char c : ops->getAlphabet()) {
            std::cout << c << " ";
        }
        std::cout << "\n  (для отрицательных чисел используйте '-' в начале)\n";
        return "";
    }

    std::string absNum = ops->abs(num);
    if (absNum.length() > MAX_DIGITS) {
        std::cout << "  ⚠ Число слишком длинное! Максимум " << MAX_DIGITS << " разрядов.\n";
        return "";
    }

    return ops->normalize(num);
}

void CLIUI::run() {
    while (true) {
        showMainMenu();
    }
}

void CLIUI::showMainMenu() {
    clearScreen();

    printHeader("КАЛЬКУЛЯТОР КОНЕЧНОЙ АРИФМЕТИКИ Z8 (Вариант 49)");

    if (!hasSystem()) {
        ops = std::make_unique<Operations>();
    }

    std::cout << "  Алфавит: {a, b, c, d, e, f, g, h}\n";
    std::cout << "  Ноль: " << ops->getZero() << ", Единица: " << ops->getOne() << "\n\n";

    printSeparator();
    std::cout << "Главное меню:\n";

    std::cout << "  [1] Информация о системе\n";
    std::cout << "  [2] Сложение (+)\n";
    std::cout << "  [3] Вычитание (-)\n";
    std::cout << "  [4] Умножение (*)\n";
    std::cout << "  [5] Деление (÷)\n";
    std::cout << "  [6] Возведение в степень\n";
    std::cout << "  [7] НОД (наибольший общий делитель)\n";
    std::cout << "  [8] НОК (наименьшее общее кратное)\n";
    std::cout << "  [C] Режим калькулятора\n";
    std::cout << "  [0] Выход\n";

    std::cout << "Ваш выбор: ";

    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        showSystemInfo();
    } else if (choice == "2") {
        additionMenu();
    } else if (choice == "3") {
        subtractionMenu();
    } else if (choice == "4") {
        multiplicationMenu();
    } else if (choice == "5") {
        divisionMenu();
    } else if (choice == "6") {
        powerMenu();
    } else if (choice == "7") {
        gcdMenu();
    } else if (choice == "8") {
        lcmMenu();
    } else if (choice == "c" || choice == "C") {
        calculatorMode();
    } else if (choice == "0") {
        std::cout << "\n  До свидания!\n";
        exit(0);
    } else {
        std::cout << "\n  Неверный выбор!\n";
        pause();
    }
}

void CLIUI::showSystemInfo() {
    clearScreen();
    ops->printInfo();
    pause();
}

void CLIUI::additionMenu() {
    clearScreen();
    printHeader("СЛОЖЕНИЕ");

    std::string a = inputNumber("Введите первое число: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите второе число: ");
    if (b.empty()) { pause(); return; }

    OperationResult result = ops->add(a, b);
    ops->printResult("+", a, b, result);

    pause();
}

void CLIUI::subtractionMenu() {
    clearScreen();
    printHeader("ВЫЧИТАНИЕ");

    std::string a = inputNumber("Введите уменьшаемое: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите вычитаемое: ");
    if (b.empty()) { pause(); return; }

    OperationResult result = ops->subtract(a, b);
    ops->printResult("-", a, b, result);

    pause();
}

void CLIUI::multiplicationMenu() {
    clearScreen();
    printHeader("УМНОЖЕНИЕ");

    std::string a = inputNumber("Введите первый множитель: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите второй множитель: ");
    if (b.empty()) { pause(); return; }

    OperationResult result = ops->multiply(a, b);
    ops->printResult("*", a, b, result);

    pause();
}

void CLIUI::divisionMenu() {
    clearScreen();
    printHeader("ДЕЛЕНИЕ С ОСТАТКОМ");

    std::string a = inputNumber("Введите делимое: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите делитель: ");
    if (b.empty()) { pause(); return; }

    DivisionResult result = ops->divide(a, b);
    ops->printDivisionResult(a, b, result);

    pause();
}

void CLIUI::powerMenu() {
    clearScreen();
    printHeader("ВОЗВЕДЕНИЕ В СТЕПЕНЬ");

    std::string base = inputNumber("Введите основание: ");
    if (base.empty()) { pause(); return; }

    std::string exp = inputNumber("Введите показатель степени: ");
    if (exp.empty()) { pause(); return; }

    OperationResult result = ops->power(base, exp);
    ops->printResult("^", base, exp, result);

    pause();
}

void CLIUI::gcdMenu() {
    clearScreen();
    printHeader("НОД (НАИБОЛЬШИЙ ОБЩИЙ ДЕЛИТЕЛЬ)");

    std::string a = inputNumber("Введите первое число: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите второе число: ");
    if (b.empty()) { pause(); return; }

    std::string result = ops->gcd(a, b);
    std::cout << "\n  НОД(" << a << ", " << b << ") = " << result << "\n";

    pause();
}

void CLIUI::lcmMenu() {
    clearScreen();
    printHeader("НОК (НАИМЕНЬШЕЕ ОБЩЕЕ КРАТНОЕ)");

    std::string a = inputNumber("Введите первое число: ");
    if (a.empty()) { pause(); return; }

    std::string b = inputNumber("Введите второе число: ");
    if (b.empty()) { pause(); return; }

    OperationResult result = ops->lcm(a, b);

    std::cout << "\n  НОК(" << a << ", " << b << ") = ";
    if (result.isOverflow) {
        std::cout << "ПЕРЕПОЛНЕНИЕ (" << result.value << ")\n";
    } else {
        std::cout << result.value << "\n";
    }

    pause();
}

void CLIUI::calculatorMode() {
    clearScreen();
    printHeader("РЕЖИМ КАЛЬКУЛЯТОРА");

    std::cout << "  Формат ввода: <число1> <операция> <число2>\n";
    std::cout << "  Операции: +, -, *, /\n";
    std::cout << "  Для выхода введите 'q'\n\n";

    printSeparator();

    while (true) {
        std::cout << "\n> ";

        std::string input;
        std::getline(std::cin >> std::ws, input);

        if (input == "q" || input == "Q" || input == "exit" || input == "quit") {
            break;
        }

        // Парсим ввод
        std::string a, op, b;
        size_t pos1 = 0;

        // Находим первое число (может начинаться с -)
        if (input[0] == '-') {
            pos1 = input.find_first_of("+-*/", 1);
        } else {
            pos1 = input.find_first_of("+-*/");
        }

        if (pos1 == std::string::npos) {
            std::cout << "  ⚠ Неверный формат! Используйте: число операция число\n";
            continue;
        }

        a = input.substr(0, pos1);
        op = input[pos1];

        // Убираем пробелы
        while (!a.empty() && a.back() == ' ') a.pop_back();

        std::string rest = input.substr(pos1 + 1);
        while (!rest.empty() && rest[0] == ' ') rest = rest.substr(1);
        b = rest;
        while (!b.empty() && b.back() == ' ') b.pop_back();

        if (!ops->isValidNumber(a)) {
            std::cout << "  ⚠ Некорректное первое число: " << a << "\n";
            continue;
        }

        if (!ops->isValidNumber(b)) {
            std::cout << "  ⚠ Некорректное второе число: " << b << "\n";
            continue;
        }

        a = ops->normalize(a);
        b = ops->normalize(b);

        if (op == "+") {
            OperationResult result = ops->add(a, b);
            ops->printResult("+", a, b, result);
        } else if (op == "-") {
            OperationResult result = ops->subtract(a, b);
            ops->printResult("-", a, b, result);
        } else if (op == "*") {
            OperationResult result = ops->multiply(a, b);
            ops->printResult("*", a, b, result);
        } else if (op == "/") {
            DivisionResult result = ops->divide(a, b);
            ops->printDivisionResult(a, b, result);
        } else {
            std::cout << "  ⚠ Неизвестная операция: " << op << "\n";
        }
    }
}
