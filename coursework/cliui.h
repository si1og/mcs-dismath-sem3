#pragma once

#include "operations.h"
#include <memory>
#include <iostream>
#include <limits>

class CLIUI {
private:
    std::unique_ptr<Operations> ops;

    void clearScreen();
    void pause();
    void printHeader(const std::string& title);
    void printSeparator();

    void showMainMenu();
    void showSystemInfo();
    void calculatorMode();
    
    std::string parseExpression(const std::string& expr, size_t& pos);
    std::string parseTerm(const std::string& expr, size_t& pos);
    std::string parseFactor(const std::string& expr, size_t& pos);
    std::string parseNumber(const std::string& expr, size_t& pos);
    void skipSpaces(const std::string& expr, size_t& pos);
    std::string resultToString(const OperationResult& res);

public:
    CLIUI();
    ~CLIUI();

    void run();
    bool hasSystem() const;
};