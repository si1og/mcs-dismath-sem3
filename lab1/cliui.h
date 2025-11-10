#pragma once

#include "universe.h"
#include "multiset.h"
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <limits>

class CLIUI {
private:
    std::unique_ptr<Universe> universe;
    std::map<std::string, std::unique_ptr<Multiset>> multisets;

    void clearScreen();
    void pause();
    void printHeader(const std::string& title);
    void printSeparator();
    std::string inputMultisetName(const std::string& prompt);
    Multiset* getMultiset(const std::string& name);

    void showMainMenu();
    void createUniverseMenu();
    void createMultisetMenu();
    void viewMultisetsMenu();
    void operationsMenu();
    void arithmeticOperationsMenu();
    void compareMultisetsMenu();

    void performBinaryOperation(
        const std::string& opName,
        Multiset (Multiset::*operation)(const Multiset&) const
    );

    void performUnaryOperation(
        const std::string& opName,
        Multiset (Multiset::*operation)() const
    );

    void showAllOperations();

public:
    CLIUI();
    ~CLIUI();

    void run();
    bool hasUniverse() const;
    bool hasMultiset(const std::string& name) const;
};
