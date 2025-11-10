#include "multiset.h"

Multiset::Multiset(const Universe& u) : Universe(u) {};

void Multiset::fillManual(int targetSize) {
    if (targetSize <= 0 || targetSize > size()) {
        throw std::invalid_argument("Размер должен быть от 1 до размера универсума");
    }

    elements.clear();

    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║          РУЧНОЕ ЗАПОЛНЕНИЕ МУЛЬТИМНОЖЕСТВА             ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Доступные элементы универсума:\n";
    printTable();

    std::cout << "\nВведите " << targetSize << " уникальных элементов:\n\n";

    for (int i = 0; i < targetSize; ++i) {
        std::string element;
        int multiplicity;

        std::cout << "  Элемент " << (i + 1) << "/" << targetSize << "\n";
        std::cout << "  Код Грея: ";
        std::cin >> element;

        if (!contains(element)) {
            std::cout << "  Ошибка: элемент не принадлежит универсуму!\n";
            std::cout << "  Попробуйте снова.\n\n";
            --i;
            continue;
        }

        if (elements.count(element)) {
            std::cout << "  Ошибка: элемент уже добавлен!\n";
            std::cout << "  Выберите другой элемент.\n\n";
            --i;
            continue;
        }

        std::cout << "  Кратность (0-" << getMaxMultiplicity() << "): ";

        if (!(std::cin >> multiplicity)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Ошибка: введите число!\n";
            std::cout << "  Попробуйте снова.\n\n";
            --i;
            continue;
        }

        if (multiplicity < 0 || multiplicity > getMaxMultiplicity()) {
            std::cout << "  Ошибка: кратность должна быть от 0 до "
                      << getMaxMultiplicity() << "!\n";
            std::cout << "  Попробуйте снова.\n\n";
            --i;
            continue;
        }

        elements[element] = multiplicity;
        std::cout << "   Элемент " << element << " с кратностью "
                  << multiplicity << " добавлен!\n\n";
    }

    std::cout << "  Мультимножество успешно заполнено!\n\n";
}

void Multiset::fillRandom() {
    elements.clear();

    int minRandom = 1;

    if (getMaxMultiplicity() == 0) minRandom = 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> sizeDistrib(minRandom, size());
    int targetSize = sizeDistrib(gen);

    std::vector<std::string> allElements = getElements();
    std::shuffle(allElements.begin(), allElements.end(), gen);

    std::uniform_int_distribution<> multDistrib(minRandom, getMaxMultiplicity());

    std::cout << "\n  Автоматическое заполнение...\n";

    for (int i = 0; i < targetSize; ++i) {
        int mult = multDistrib(gen);
        elements[allElements[i]] = mult;
        std::cout << "  • " << allElements[i] << " → кратность: " << mult << "\n";
    }

    std::cout << "\n  Мультимножество заполнено случайно!\n\n";
}

int Multiset::getMultiplicity(const std::string& element) const {
    auto it = elements.find(element);
    return (it != elements.end()) ? it->second : 0;
}

void Multiset::setMultiplicity(const std::string& element, int m) {
    if (!contains(element)) {
        throw std::invalid_argument("Элемент не принадлежит универсуму");
    }

    if (m < 0 || m > getMaxMultiplicity()) {
        throw std::invalid_argument("Кратность должна быть от 0 до максимальной");
    }

    if (m == 0) {
        elements.erase(element);
    } else {
        elements[element] = m;
    }
}

Multiset Multiset::unionWith(const Multiset& other) const {
    Multiset result(*this);

    for (const auto& [element, count] : other.elements) {
        int currentCount = result.getMultiplicity(element);
        result.elements[element] = std::max(currentCount, count);
    }

    return result;
}

Multiset Multiset::intersectionWith(const Multiset& other) const {
    Multiset result(*this);
    result.elements.clear();

    for (const auto& [element, count] : elements) {
        int otherCount = other.getMultiplicity(element);
        if (otherCount > 0) {
            result.elements[element] = std::min(count, otherCount);
        }
    }

    return result;
}

Multiset Multiset::differenceWith(const Multiset& other) const {
    Multiset result(*this);
    result.elements.clear();

    for (const auto& element : this->getElements()) {
        int countA = getMultiplicity(element);
        int countB = other.getMultiplicity(element);

        int diff = std::max(countA - countB, 0);
        result.elements[element] = diff;
    }

    return result;
}

Multiset Multiset::symmetricDifferenceWith(const Multiset& other) const {
    Multiset diff1 = this->differenceWith(other);
    Multiset diff2 = other.differenceWith(*this);
    return diff1.unionWith(diff2);
}

Multiset Multiset::complement() const {
    Multiset result(*this);
    result.elements.clear();

    for (const auto& element : this->getElements()) {
        int currentMult = getMultiplicity(element);

        int complementMult = this->getMaxMultiplicity() - currentMult;
        result.elements[element] = complementMult;
    }

    return result;
}

Multiset Multiset::arithmeticSum(const Multiset& other) const {
    Multiset result(*this);

    for (const auto& [element, count] : other.elements) {
        int currentCount = result.getMultiplicity(element);
        int newCount = std::min(currentCount + count, getMaxMultiplicity());
        result.elements[element] = newCount;
    }

    return result;
}

Multiset Multiset::arithmeticDifference(const Multiset& other) const {
    return differenceWith(other);
}

Multiset Multiset::arithmeticProduct(const Multiset& other) const {
    Multiset result(*this);
    result.elements.clear();

    for (const auto& [element, count] : elements) {
        int otherCount = other.getMultiplicity(element);
        if (otherCount > 0) {
            int product = std::min(count * otherCount, getMaxMultiplicity());
            result.elements[element] = product;
        }
    }

    return result;
}

Multiset Multiset::arithmeticDivision(const Multiset& other) const {
    Multiset result(*this);
    result.elements.clear();

    for (const auto& [element, countA] : elements) {
        int countB = other.getMultiplicity(element);

        if (countB == 0) {
            continue;
        }

        int quotient = countA / countB;

        if (quotient > 0) {
            result.elements[element] = quotient;
        }
    }

    return result;
}

bool Multiset::operator==(const Multiset& other) const {
    return elements == other.elements;
}

bool Multiset::operator!=(const Multiset& other) const {
    return !(*this == other);
}

void Multiset::printTable() const {
    if (elements.empty() || isEmpty()) {
        std::cout << "\n Пустое мультимножество\n\n";
        return;
    }

    std::cout << "  ┌──────┬──────────────┬──────────────┐\n";
    std::cout << "  │  №   │  Код Грея    │  Кратность   │\n";
    std::cout << "  ├──────┼──────────────┼──────────────┤\n";

    int index = 1;
    for (const auto& [element, count] : elements) {
        if (count > 0) {
            std::cout << "  │ " << std::setw(4) << index++ << " │ "
                      << std::setw(12) << element << " │ "
                      << std::setw(12) << count << " │\n";
        }
    }

    std::cout << "  └──────┴──────────────┴──────────────┘\n\n";
}

bool Multiset::isEmpty() const {
    for (const auto& [element, count] : elements) {
        if (count > 0) {
            return false;
        }
    }

    return true;
}
