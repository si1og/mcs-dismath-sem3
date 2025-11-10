#include "universe.h"

Universe::Universe() {};

Universe::Universe(int depth, int maxMultiplicity)
    : depth(depth), maxMultiplicity(maxMultiplicity) {

    if (depth <= 0 || depth > 20) {
        throw std::invalid_argument("Разрядность должна быть от 1 до 20");
    }

    if (maxMultiplicity <= 0) {
        throw std::invalid_argument("Максимальная кратность должна быть положительной");
    }

    elements = generateGrayCode(depth);

    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║              УНИВЕРСУМ УСПЕШНО СОЗДАН                  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    std::cout << "  Размер: " << elements.size() << " элементов (2^" << depth << ")\n";
    std::cout << "  Разрядность кода Грея: " << depth << "\n";
    std::cout << "  Максимальная кратность: " << maxMultiplicity << "\n\n";
}

Universe::~Universe() {};

std::vector<std::string> Universe::generateGrayCode(int n) {
    if (n <= 0) {
        throw std::invalid_argument("Разрядность должна быть положительной");
    }

    if (n > 20) {
        throw std::invalid_argument("Разрядность слишком большая (максимум 20)");
    }

    std::vector<std::string> result;
    int total = 1 << n; // 2^n элементов

    for (int i = 0; i < total; ++i) {
        // Формула кода Грея: gray = i XOR (i >> 1)
        int gray = i ^ (i >> 1);

        std::string binary;
        for (int j = n - 1; j >= 0; --j) {
            binary += ((gray >> j) & 1) ? '1' : '0';
        }
        result.push_back(binary);
    }

    return result;
}

int Universe::getDepth() const {
    return depth;
}

int Universe::getMaxMultiplicity() const {
    return maxMultiplicity;
}

const std::vector<std::string>& Universe::getElements() const {
    return elements;
}

bool Universe::contains(const std::string& element) const {
    return std::find(elements.begin(), elements.end(), element) != elements.end();
}

int Universe::size() const {
    return static_cast<int>(elements.size());
}

void Universe::print() const {
    std::cout << "U = { ";

    const int maxDisplay = 8; // Показываем максимум 8 элементов

    for (int i = 0; i < std::min(maxDisplay, size()); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << elements[i];
    }

    if (size() > maxDisplay) {
        std::cout << ", ... (" << (size() - maxDisplay) << " ещё)";
    }

    std::cout << " }\n";
    std::cout << "Максимальная кратность для всех элементов: " << maxMultiplicity << "\n";
}

void Universe::printTable() const {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      УНИВЕРСУМ                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    std::cout << "  Разрядность: " << depth << "\n";
    std::cout << "  Размер: " << size() << " элементов\n";
    std::cout << "  Максимальная кратность: " << maxMultiplicity << "\n\n";

    std::cout << "  ┌────────┬──────────────┐\n";
    std::cout << "  │   №    │   Элемент    │\n";
    std::cout << "  ├────────┼──────────────┤\n";

    for (int i = 0; i < size(); ++i) {
        std::cout << "  │ " << std::setw(6) << (i + 1) << " │ "
                  << std::setw(12) << elements[i] << " │\n";
    }

    std::cout << "  └────────┴──────────────┘\n\n";
    std::cout << "  Все элементы имеют максимальную кратность: "
              << maxMultiplicity << "\n\n";
}
