#pragma once
#include "z8.h"
#include <string>

struct DivisionResult {
    std::string quotient;
    std::string remainder;
    bool isOverflow;
    bool isDivByZero;
    bool isZeroByZero;

    DivisionResult() : isOverflow(false), isDivByZero(false), isZeroByZero(false) {}
};

struct OperationResult {
    std::string value;
    bool isOverflow;
    bool isNegative;

    OperationResult() : isOverflow(false), isNegative(false) {}
    OperationResult(const std::string& v, bool neg = false, bool ovf = false)
        : value(v), isNegative(neg), isOverflow(ovf) {}
};

class Operations : public Z8 {
public:
    std::string trimLeadingZeros(const std::string& num) const;
    bool isZeroNumber(const std::string& num) const;
    int compareAbs(const std::string& a, const std::string& b) const;

    std::string addPositive(const std::string& a, const std::string& b) const;
    std::string subtractPositive(const std::string& a, const std::string& b) const;

    std::string multiplyByDigit(const std::string& num, char digit) const;
    std::string multiplyPositive(const std::string& a, const std::string& b) const;

    std::string increment(const std::string& num) const;
    std::string decrement(const std::string& num) const;

    std::string getMaxPositive() const;
    std::string getMaxNegative() const;

public:
    Operations();
    Operations(int variant);
    ~Operations();

    OperationResult add(const std::string& a, const std::string& b) const;
    OperationResult subtract(const std::string& a, const std::string& b) const;
    OperationResult multiply(const std::string& a, const std::string& b) const;
    DivisionResult divide(const std::string& a, const std::string& b) const;

    OperationResult power(const std::string& base, const std::string& exp) const;

    std::string gcd(const std::string& a, const std::string& b) const;
    OperationResult lcm(const std::string& a, const std::string& b) const;

    bool isValidNumber(const std::string& num) const;

    std::string normalize(const std::string& num) const;
    std::string abs(const std::string& num) const;

    bool isNegative(const std::string& num) const;
    std::string negate(const std::string& num) const;

    void printResult(const std::string& operation,
                     const std::string& a,
                     const std::string& b,
                     const OperationResult& result) const;
    void printDivisionResult(const std::string& a,
                             const std::string& b,
                             const DivisionResult& result) const;
};
