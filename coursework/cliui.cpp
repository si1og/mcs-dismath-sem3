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
    std::cout << "║ " << std::left << title <<  "\n";
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
    std::cout << "  Ноль: " << ops->getZero() << ", Единица: " << ops->getOne() << "\n";
    std::cout << "  Правило +1: a→b→g→d→h→e→f→c→a\n\n";

    printSeparator();
    std::cout << "Главное меню:\n";

    std::cout << "  [1] Информация о системе\n";
    std::cout << "  [2] Сложение\n";
    std::cout << "  [3] Вычитание\n";
    std::cout << "  [4] Умножение\n";
    std::cout << "  [5] Деление\n";
    std::cout << "  [C] Режим калькулятора\n";
    std::cout << "  [0] Выход\n";

    std::cout << "\nВаш выбор: ";

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

void CLIUI::skipSpaces(const std::string& expr, size_t& pos) {
    while (pos < expr.length() && expr[pos] == ' ') {
        ++pos;
    }
}

std::string CLIUI::resultToString(const OperationResult& res) {
    return (res.isNegative ? "-" : "") + res.value;
}

std::string CLIUI::parseNumber(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    std::string num;
    
    // Проверяем унарный минус
    if (pos < expr.length() && expr[pos] == '-') {
        num += '-';
        ++pos;
    }
    
    // Читаем символы алфавита
    while (pos < expr.length() && ops->isValidChar(expr[pos])) {
        num += expr[pos];
        ++pos;
    }
    
    if (num.empty() || num == "-") {
        throw std::runtime_error("Ожидалось число");
    }
    
    return ops->normalize(num);
}

// Парсит фактор: число или выражение в скобках
std::string CLIUI::parseFactor(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    // Унарный минус перед скобкой
    bool negate = false;
    if (pos < expr.length() && expr[pos] == '-') {
        // Проверяем, не число ли это
        size_t nextPos = pos + 1;
        skipSpaces(expr, nextPos);
        if (nextPos < expr.length() && expr[nextPos] == '(') {
            negate = true;
            ++pos;
            skipSpaces(expr, pos);
        }
    }
    
    std::string result;
    
    if (pos < expr.length() && expr[pos] == '(') {
        ++pos; // пропускаем '('
        result = parseExpression(expr, pos);
        skipSpaces(expr, pos);
        if (pos >= expr.length() || expr[pos] != ')') {
            throw std::runtime_error("Ожидалась закрывающая скобка ')'");
        }
        ++pos; // пропускаем ')'
    } else {
        result = parseNumber(expr, pos);
    }
    
    if (negate) {
        result = ops->negate(result);
    }
    
    return result;
}

// Парсит терм: факторы, соединённые * или /
std::string CLIUI::parseTerm(const std::string& expr, size_t& pos) {
    std::string left = parseFactor(expr, pos);
    
    while (true) {
        skipSpaces(expr, pos);
        
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op != '*' && op != '/') break;
        
        ++pos; // пропускаем оператор
        std::string right = parseFactor(expr, pos);
        
        if (op == '*') {
            OperationResult res = ops->multiply(left, right);
            left = resultToString(res);
        } else {
            DivisionResult res = ops->divide(left, right);
            if (res.isDivByZero || res.isZeroByZero) {
                throw std::runtime_error("Деление на ноль");
            }
            // Выводим остаток, если он есть
            if (!ops->isZeroNumber(res.remainder)) {
                std::cout << "  (остаток: " << res.remainder << ")\n";
            }
            left = res.quotient;
        }
    }
    
    return left;
}

// Парсит выражение: термы, соединённые + или -
std::string CLIUI::parseExpression(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    // Обработка унарного минуса в начале выражения
    bool negateFirst = false;
    if (pos < expr.length() && expr[pos] == '-') {
        size_t nextPos = pos + 1;
        skipSpaces(expr, nextPos);
        // Если после минуса идёт скобка, это унарный минус
        if (nextPos < expr.length() && expr[nextPos] == '(') {
            negateFirst = false; // обработается в parseFactor
        }
    }
    
    std::string left = parseTerm(expr, pos);
    
    while (true) {
        skipSpaces(expr, pos);
        
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op != '+' && op != '-') break;
        
        ++pos; // пропускаем оператор
        std::string right = parseTerm(expr, pos);
        
        if (op == '+') {
            OperationResult res = ops->add(left, right);
            left = resultToString(res);
        } else {
            OperationResult res = ops->subtract(left, right);
            left = resultToString(res);
        }
    }
    
    return left;
}

void CLIUI::calculatorMode() {
    clearScreen();
    printHeader("РЕЖИМ КАЛЬКУЛЯТОРА");

    std::cout << "  Поддерживаются выражения со скобками!\n";
    std::cout << "  Примеры: g*(b+d), (bg+c)*g, ((a+b)+c)\n";
    std::cout << "  Операции: +, -, *, /\n";
    std::cout << "  Для выхода введите 'q'\n\n";

    printSeparator();

    // Очищаем буфер ввода
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "\n> ";

        std::string input;
        std::getline(std::cin, input);

        // Убираем пробелы в начале и конце
        while (!input.empty() && input[0] == ' ') input = input.substr(1);
        while (!input.empty() && input.back() == ' ') input.pop_back();

        if (input.empty()) continue;

        if (input == "q" || input == "Q" || input == "exit" || input == "quit") {
            break;
        }

        try {
            size_t pos = 0;
            std::string result = parseExpression(input, pos);
            
            skipSpaces(input, pos);
            if (pos < input.length()) {
                throw std::runtime_error("Неожиданный символ: '" + std::string(1, input[pos]) + "'");
            }
            
            std::cout << "  = " << result << "\n";
            
        } catch (const std::exception& e) {
            std::cout << "  Ошибка: " << e.what() << "\n";
        }
    }
}