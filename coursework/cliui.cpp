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
    std::cout << "  [C] Режим калькулятора\n";
    std::cout << "  [0] Выход\n";

    std::cout << "\nВаш выбор: ";

    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        showSystemInfo();
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

void CLIUI::skipSpaces(const std::string& expr, size_t& pos) {
    while (pos < expr.length() && expr[pos] == ' ') {
        ++pos;
    }
}

std::string CLIUI::resultToString(const OperationResult& res) {
    if (res.isOverflow) {
        throw std::runtime_error("ПЕРЕПОЛНЕНИЕ");
    }
    return (res.isNegative ? "-" : "") + res.value;
}

std::string CLIUI::parseNumber(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    std::string num;
    
    if (pos < expr.length() && expr[pos] == '-') {
        num += '-';
        ++pos;
    }
    
    while (pos < expr.length() && ops->isValidChar(expr[pos])) {
        num += expr[pos];
        ++pos;
    }
    
    if (num.empty() || num == "-") {
        throw std::runtime_error("Ожидалось число");
    }
    
    return ops->normalize(num);
}

std::string CLIUI::parseFactor(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    bool negate = false;
    if (pos < expr.length() && expr[pos] == '-') {
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
        ++pos;
        result = parseExpression(expr, pos);
        skipSpaces(expr, pos);
        if (pos >= expr.length() || expr[pos] != ')') {
            throw std::runtime_error("Ожидалась закрывающая скобка ')'");
        }
        ++pos;
    } else {
        result = parseNumber(expr, pos);
    }
    
    if (negate) {
        result = ops->negate(result);
    }
    
    return result;
}

std::string CLIUI::parseTerm(const std::string& expr, size_t& pos) {
    std::string left = parseFactor(expr, pos);
    
    while (true) {
        skipSpaces(expr, pos);
        
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op != '*' && op != '/') break;
        
        ++pos;
        std::string right = parseFactor(expr, pos);
        
        if (op == '*') {
            OperationResult res = ops->multiply(left, right);
            left = resultToString(res);
        } else {
            DivisionResult res = ops->divide(left, right);
            if (res.isDivByZero) {
                throw std::runtime_error("Пустое множество");
            }
            if (res.isZeroByZero) {
                throw std::runtime_error("Неопределённость: результат - любое число в [" 
                    + ops->getMaxNegative() + ";" + ops->getMaxPositive() + "]");
            }
            if (!ops->isZeroNumber(res.remainder)) {
                std::cout << "  (остаток: " << res.remainder << ")\n";
            }
            left = res.quotient;
        }
    }
    
    return left;
}

std::string CLIUI::parseExpression(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    std::string left = parseTerm(expr, pos);
    
    while (true) {
        skipSpaces(expr, pos);
        
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op != '+' && op != '-') break;
        
        ++pos;
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

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "\n> ";

        std::string input;
        std::getline(std::cin, input);

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
            std::cout << e.what() << "\n";
        }
    }
}