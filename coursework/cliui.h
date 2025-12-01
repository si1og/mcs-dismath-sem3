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

    std::string inputNumber(const std::string& prompt);

    void showMainMenu();
    void additionMenu();
    void subtractionMenu();
    void multiplicationMenu();
    void divisionMenu();
    void powerMenu();
    void gcdMenu();
    void lcmMenu();
    void showSystemInfo();
    void calculatorMode();

public:
    CLIUI();
    ~CLIUI();

    void run();
    bool hasSystem() const;
};
