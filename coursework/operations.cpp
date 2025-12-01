#include "operations.h"
#include <algorithm>

Operations::Operations() : Z8() {}
Operations::~Operations() {}

std::string Operations::trimLeadingZeros(const std::string& num) const {
    if (num.empty()) return std::string(1, getZero());

    size_t start = 0;
    while (start < num.length() - 1 && num[start] == getZero()) {
        ++start;
    }

    return num.substr(start);
}

std::string Operations::abs(const std::string& num) const {
    if (num.empty()) return std::string(1, getZero());
    if (num[0] == '-') return num.substr(1);
    return num;
}

bool Operations::isZeroNumber(const std::string& num) const {
    std::string n = abs(num);
    n = trimLeadingZeros(n);
    return n.length() == 1 && n[0] == getZero();
}

bool Operations::isNegative(const std::string& num) const {
    return !num.empty() && num[0] == '-';
}

std::string Operations::negate(const std::string& num) const {
    if (isZeroNumber(num)) return std::string(1, getZero());

    if (isNegative(num)) {
        return num.substr(1);
    } else {
        return "-" + num;
    }
}

// пользуемся, чтобы привести числа к «стандартному» виду
std::string Operations::normalize(const std::string& num) const {
    if (num.empty()) return std::string(1, getZero());

    bool neg = isNegative(num);
    std::string absNum = neg ? num.substr(1) : num;

    absNum = trimLeadingZeros(absNum);

    if (absNum.length() == 1 && absNum[0] == getZero()) {
        return std::string(1, getZero());
    }

    return neg ? "-" + absNum : absNum;
}

bool Operations::isValidNumber(const std::string& num) const {
    if (num.empty()) return false;

    size_t start = 0;
    if (num[0] == '-') {
        if (num.length() == 1) return false;
        start = 1;
    }

    for (size_t i = start; i < num.length(); ++i) {
        if (!isValidChar(num[i])) return false;
    }

    return true;
}

int Operations::compareAbs(const std::string& a, const std::string& b) const {
    // нормализуем по модулю
    std::string normA = trimLeadingZeros(abs(a));
    std::string normB = trimLeadingZeros(abs(b));

    if (normA.length() != normB.length()) {
        return normA.length() > normB.length() ? 1 : -1;
    }

    for (size_t i = 0; i < normA.length(); ++i) {
        if (normA[i] == normB[i]) continue;

        char ca = normA[i];
        char cb = normB[i];

        int posA = 0, posB = 0;
        char cur = getZero();
        for (int j = 0; j < getBase(); ++j) {
            if (cur == ca) posA = j;
            if (cur == cb) posB = j;
            cur = next(cur);
        }

        // выше запоминаем последние позиции, которые совпали с cur:
        // cur двигается по цепочке a -> ...
        //
        // => у кого позиция больше, тот больше

        return posA > posB ? 1 : -1;
    }

    return 0;
}

std::string Operations::getMaxPositive() const {
    char maxDigit = prev(getZero());
    return std::string(MAX_DIGITS, maxDigit);
}

std::string Operations::getMaxNegative() const {
    return "-" + getMaxPositive();
}

// «база», на которой строим сложение
std::string Operations::increment(const std::string& num) const {
    if (num.empty()) return std::string(1, getOne());

    std::string result = num;

    // берем последнюю цифру:
    // 1) берем след. цифру по цепочке от последней цифры (п. a -> b)
    // 2) записываем обновленную цифру в result на позицию последней цифры
    // а) если обновлённая цифра 0 - повторяем для предпоследней цифры ... и т.д.
    // б) если не 0 - возвращаем result
    for (int i = static_cast<int>(result.length()) - 1; i >= 0; --i) {
        char nextChar = next(result[i]);
        result[i] = nextChar;

        if (nextChar != getZero()) {
            return result;
        }
    }

    // если все цифры в результате операции 0, возвращаем 1 + нули
    return std::string(1, getOne()) + result;
}


// ан-но, как с increment, но с prev
std::string Operations::decrement(const std::string& num) const {
    if (isZeroNumber(num)) {
        return "-" + std::string(1, getOne());
    }

    std::string result = num;

    for (int i = static_cast<int>(result.length()) - 1; i >= 0; --i) {
        if (result[i] != getZero()) {
            result[i] = prev(result[i]);
            return trimLeadingZeros(result);
        }
        result[i] = prev(getZero());
    }

    return trimLeadingZeros(result);
}

std::string Operations::addPositive(const std::string& a, const std::string& b) const {
    std::string numA = trimLeadingZeros(a);
    std::string numB = trimLeadingZeros(b);

    size_t maxLen = std::max(numA.length(), numB.length());
    while (numA.length() < maxLen) numA = std::string(1, getZero()) + numA;
    while (numB.length() < maxLen) numB = std::string(1, getZero()) + numB;

    std::string result(maxLen, getZero());
    std::string carry(1, getZero());

    for (int i = static_cast<int>(maxLen) - 1; i >= 0; --i) {
        char sum = numA[i];

        char counter = numB[i];
        while (counter != getZero()) {
            char nextSum = next(sum);
            if (nextSum == getZero()) {
                carry = increment(carry);
            }
            sum = nextSum;
            counter = prev(counter);
        }

        char carryCounter = carry[carry.length() - 1];
        carry = (carry.length() > 1) ? carry.substr(0, carry.length() - 1) : std::string(1, getZero());

        while (carryCounter != getZero()) {
            char nextSum = next(sum);
            if (nextSum == getZero()) {
                carry = increment(carry);
            }
            sum = nextSum;
            carryCounter = prev(carryCounter);
        }

        result[i] = sum;
    }

    if (!isZeroNumber(carry)) {
        result = carry + result;
    }

    return trimLeadingZeros(result);
}

std::string Operations::subtractPositive(const std::string& a, const std::string& b) const {
    std::string numA = trimLeadingZeros(a);
    std::string numB = trimLeadingZeros(b);

    if (compareAbs(numA, numB) < 0) {
        throw std::logic_error("subtractPositive: a < b");
    }

    size_t maxLen = numA.length();
    while (numB.length() < maxLen) numB = std::string(1, getZero()) + numB;

    std::string result(maxLen, getZero());
    std::string borrow(1, getZero());

    for (int i = static_cast<int>(maxLen) - 1; i >= 0; --i) {
        char diff = numA[i];

        char counter = numB[i];
        while (counter != getZero()) {
            if (diff == getZero()) {
                borrow = increment(borrow);
                diff = prev(getZero());
            } else {
                diff = prev(diff);
            }
            counter = prev(counter);
        }

        // Вычитаем заём
        char borrowCounter = borrow[borrow.length() - 1];
        borrow = (borrow.length() > 1) ? borrow.substr(0, borrow.length() - 1) : std::string(1, getZero());

        while (borrowCounter != getZero()) {
            if (diff == getZero()) {
                borrow = increment(borrow);
                diff = prev(getZero());
            } else {
                diff = prev(diff);
            }
            borrowCounter = prev(borrowCounter);
        }

        result[i] = diff;
    }

    return trimLeadingZeros(result);
}

// ============== Умножение ==============

std::string Operations::multiplyByDigit(const std::string& num, char digit) const {
    if (digit == getZero()) return std::string(1, getZero());
    if (digit == getOne()) return num;

    std::string result(1, getZero());

    char counter = digit;
    while (counter != getZero()) {
        result = addPositive(result, num);
        counter = prev(counter);
    }

    return result;
}

std::string Operations::multiplyPositive(const std::string& a, const std::string& b) const {
    if (isZeroNumber(a) || isZeroNumber(b)) {
        return std::string(1, getZero());
    }

    std::string numA = trimLeadingZeros(a);
    std::string numB = trimLeadingZeros(b);

    std::string result(1, getZero());

    for (size_t i = 0; i < numB.length(); ++i) {
        char digit = numB[numB.length() - 1 - i];

        if (digit != getZero()) {
            std::string partial = multiplyByDigit(numA, digit);

            for (size_t j = 0; j < i; ++j) {
                partial += getZero();
            }

            result = addPositive(result, partial);
        }
    }

    return trimLeadingZeros(result);
}

// ============== ОСНОВНЫЕ ОПЕРАЦИИ ==============

OperationResult Operations::add(const std::string& a, const std::string& b) const {
    OperationResult res;

    std::string normA = normalize(a);
    std::string normB = normalize(b);

    bool negA = isNegative(normA);
    bool negB = isNegative(normB);
    std::string absA = abs(normA);
    std::string absB = abs(normB);

    if (!negA && !negB) {
        res.value = addPositive(absA, absB);
        res.isNegative = false;
    } else if (negA && negB) {
        res.value = addPositive(absA, absB);
        res.isNegative = true;
    } else if (!negA && negB) {
        int cmp = compareAbs(absA, absB);
        if (cmp >= 0) {
            res.value = subtractPositive(absA, absB);
            res.isNegative = false;
        } else {
            res.value = subtractPositive(absB, absA);
            res.isNegative = true;
        }
    } else {
        int cmp = compareAbs(absB, absA);
        if (cmp >= 0) {
            res.value = subtractPositive(absB, absA);
            res.isNegative = false;
        } else {
            res.value = subtractPositive(absA, absB);
            res.isNegative = true;
        }
    }

    if (res.value.length() > MAX_DIGITS) {
        res.isOverflow = true;
    }

    res.value = trimLeadingZeros(res.value);
    if (isZeroNumber(res.value)) {
        res.isNegative = false;
    }

    return res;
}

OperationResult Operations::subtract(const std::string& a, const std::string& b) const {
    return add(a, negate(b));
}

OperationResult Operations::multiply(const std::string& a, const std::string& b) const {
    OperationResult res;

    std::string normA = normalize(a);
    std::string normB = normalize(b);

    if (isZeroNumber(normA) || isZeroNumber(normB)) {
        res.value = std::string(1, getZero());
        res.isNegative = false;
        return res;
    }

    bool negA = isNegative(normA);
    bool negB = isNegative(normB);

    res.value = multiplyPositive(abs(normA), abs(normB));
    res.isNegative = (negA != negB);

    if (res.value.length() > MAX_DIGITS) {
        res.isOverflow = true;
    }

    if (isZeroNumber(res.value)) {
        res.isNegative = false;
    }

    return res;
}

DivisionResult Operations::divide(const std::string& a, const std::string& b) const {
    DivisionResult res;

    std::string normA = normalize(a);
    std::string normB = normalize(b);

    bool negA = isNegative(normA);
    bool negB = isNegative(normB);
    std::string absA = abs(normA);
    std::string absB = abs(normB);

    // Деление на ноль
    if (isZeroNumber(normB)) {
        if (isZeroNumber(normA)) {
            res.isZeroByZero = true;
            res.quotient = "[" + getMaxNegative() + "; " + getMaxPositive() + "]";
            res.remainder = std::string(1, getZero());
        } else {
            res.isDivByZero = true;
            res.quotient = "Ø";
            res.remainder = "Ø";
        }
        return res;
    }

    // 0 / x = 0
    if (isZeroNumber(normA)) {
        res.quotient = std::string(1, getZero());
        res.remainder = std::string(1, getZero());
        return res;
    }

    // Деление в столбик
    std::string quotient(1, getZero());
    std::string remainder = absA;

    while (compareAbs(remainder, absB) >= 0) {
        remainder = subtractPositive(remainder, absB);
        quotient = increment(quotient);
    }

    // Особый случай: -a / b с остатком
    if (negA && !negB && !isZeroNumber(remainder)) {
        quotient = increment(quotient);
        remainder = subtractPositive(absB, remainder);
    }

    bool negResult = (negA != negB);

    res.quotient = trimLeadingZeros(quotient);
    res.remainder = trimLeadingZeros(remainder);

    if (negResult && !isZeroNumber(res.quotient)) {
        res.quotient = "-" + res.quotient;
    }

    if (abs(res.quotient).length() > MAX_DIGITS) {
        res.isOverflow = true;
    }

    return res;
}

OperationResult Operations::power(const std::string& base, const std::string& exp) const {
    OperationResult res;

    std::string normBase = normalize(base);
    std::string normExp = normalize(exp);

    if (isNegative(normExp)) {
        std::string absBase = abs(normBase);
        if (absBase == std::string(1, getOne())) {
            res.value = std::string(1, getOne());
            res.isNegative = false;
        } else {
            res.value = std::string(1, getZero());
            res.isNegative = false;
        }
        return res;
    }

    if (isZeroNumber(normExp)) {
        res.value = std::string(1, getOne());
        res.isNegative = false;
        return res;
    }

    if (isZeroNumber(normBase)) {
        res.value = std::string(1, getZero());
        res.isNegative = false;
        return res;
    }

    if (normExp == std::string(1, getOne())) {
        res.value = abs(normBase);
        res.isNegative = isNegative(normBase);
        return res;
    }

    std::string result(1, getOne());
    std::string counter = normExp;
    std::string absBase = abs(normBase);

    while (!isZeroNumber(counter)) {
        OperationResult mulRes = multiply(result, absBase);
        if (mulRes.isOverflow) {
            res.isOverflow = true;
            res.value = mulRes.value;
            return res;
        }
        result = mulRes.value;
        counter = decrement(counter);
    }

    res.value = result;

    if (isNegative(normBase)) {
        std::string two(1, next(getOne()));
        DivisionResult divRes = divide(normExp, two);
        if (!isZeroNumber(divRes.remainder)) {
            res.isNegative = true;
        }
    }

    if (res.value.length() > MAX_DIGITS) {
        res.isOverflow = true;
    }

    return res;
}

std::string Operations::gcd(const std::string& a, const std::string& b) const {
    std::string x = abs(normalize(a));
    std::string y = abs(normalize(b));

    if (isZeroNumber(x)) return y;
    if (isZeroNumber(y)) return x;

    while (!isZeroNumber(y)) {
        DivisionResult divRes = divide(x, y);
        x = y;
        y = divRes.remainder;
    }

    return x;
}

OperationResult Operations::lcm(const std::string& a, const std::string& b) const {
    OperationResult res;

    std::string x = abs(normalize(a));
    std::string y = abs(normalize(b));

    if (isZeroNumber(x) || isZeroNumber(y)) {
        res.value = std::string(1, getZero());
        return res;
    }

    std::string g = gcd(x, y);

    // НОК = |a| / НОД * |b| (делим сначала, чтобы избежать переполнения)
    DivisionResult divRes = divide(x, g);
    if (divRes.isOverflow) {
        res.isOverflow = true;
        return res;
    }

    OperationResult mulRes = multiply(divRes.quotient, y);
    res.value = mulRes.value;
    res.isOverflow = mulRes.isOverflow;

    return res;
}

// ============== Вывод результатов ==============

void Operations::printResult(const std::string& operation,
                              const std::string& a,
                              const std::string& b,
                              const OperationResult& result) const {
    std::cout << "\n  " << a << " " << operation << " " << b << " = ";

    if (result.isOverflow) {
        std::cout << "ПЕРЕПОЛНЕНИЕ (";
        if (result.isNegative) std::cout << "-";
        std::cout << result.value << ")\n";
    } else {
        if (result.isNegative) std::cout << "-";
        std::cout << result.value << "\n";
    }
}

void Operations::printDivisionResult(const std::string& a,
                                      const std::string& b,
                                      const DivisionResult& result) const {
    std::cout << "\n  " << a << " ÷ " << b << " = ";

    if (result.isDivByZero) {
        std::cout << "Ø (деление на ноль)\n";
    } else if (result.isZeroByZero) {
        std::cout << result.quotient << " (неопределённость 0/0)\n";
    } else if (result.isOverflow) {
        std::cout << "ПЕРЕПОЛНЕНИЕ\n";
    } else {
        std::cout << result.quotient;
        if (!isZeroNumber(result.remainder)) {
            std::cout << "(" << result.remainder << ")";
        }
        std::cout << "\n";
    }
}
