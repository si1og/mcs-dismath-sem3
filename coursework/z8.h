#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "constants.h"


class Z8 {
protected:
    std::map<char, char> plusOneRule;
    std::map<char, char> minusOneRule;
    std::vector<char> alphabet;

    // "0"
    char additiveIdentity;

    // "1"
    char multiplicativeIdentity;

public:
    Z8();
    Z8(int variant);
    virtual ~Z8();

    void initVariant();
    char next(char c) const;
    char prev(char c) const;

    bool isZero(char c) const;
    bool isOne(char c) const;

    char getZero() const;
    char getOne() const;

    const std::vector<char>& getAlphabet() const;
    int getBase() const;

    bool isValidChar(char c) const;

    void printPlusOneTable() const;
    void printInfo() const;
};
