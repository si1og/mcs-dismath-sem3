#include "universe.h"

Universe::Universe() : depth(0), maxMultiplicity(0) {};

Universe::Universe(int depth, int maxMultiplicity)
    : depth(depth), maxMultiplicity(maxMultiplicity) {

    if (depth < 0) {
        throw std::invalid_argument("Разрядность должна быть неотрицательной");
    }

    if (depth > RECOMMENDED_MAX_DEPTH) {
        std::cout << "\n  Предупреждение: разрядность " << depth
                  << " превышает рекомендуемое значение " << RECOMMENDED_MAX_DEPTH << "\n";
        std::cout << "  Это может привести к большому потреблению памяти и времени выполнения.\n\n";
    }

    // При нулевой разрядности кратность тоже 0
    if (depth == 0) {
        this->maxMultiplicity = 0;
    }

    if (this->maxMultiplicity < 0) {
        throw std::invalid_argument("Максимальная кратность должна быть неотрицательной");
    }

    if (depth == 0 || this->maxMultiplicity == 0) {
        std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
        std::cout << "║              СОЗДАН ПУСТОЙ УНИВЕРСУМ                   ║\n";
        std::cout << "╚════════════════════════════════════════════════════════╝\n";
        std::cout << "   Разрядность: " << depth << "\n";
        std::cout << "   Максимальная кратность: " << this->maxMultiplicity << "\n\n";
        return;
    }

    elements = generateGrayCode(depth);

    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║              УНИВЕРСУМ УСПЕШНО СОЗДАН                  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    std::cout << "  Размер: " << elements.size() << " элементов (2^" << depth << ")\n";
    std::cout << "  Разрядность кода Грея: " << depth << "\n";
    std::cout << "  Максимальная кратность: " << this->maxMultiplicity << "\n\n";
}

Universe::~Universe() {};

std::vector<std::string> Universe::generateGrayCode(int n) {
    if (n < 0) {
        throw std::invalid_argument("Разрядность должна быть неотрицательной");
    }

    std::vector<std::string> result;

    if (n > 30) {
        throw std::invalid_argument("Разрядность слишком большая (максимум 30 для безопасной работы с int)");
    }

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

void Universe::printTable() const {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      УНИВЕРСУМ                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    if (maxMultiplicity == 0 || depth == 0) {
        std::cout << "   Пустой универсум\n\n";
        return;
    }

    std::cout << "  Разрядность: " << depth << "\n";
    std::cout << "  Размер: " << size() << " элементов\n";
    std::cout << "  Максимальная кратность: " << maxMultiplicity << "\n\n";

    // Для больших универсумов (>16 элементов) используем построчный вывод
    if (size() > 16) {
        printTablePaged();
    } else {
        printTableCompact();
    }

    std::cout << "  Все элементы имеют максимальную кратность: "
              << maxMultiplicity << "\n\n";
}

void Universe::printTableCompact() const {
    std::cout << "  ┌────────┬──────────────┐\n";
    std::cout << "  │   №    │   Элемент    │\n";
    std::cout << "  ├────────┼──────────────┤\n";

    for (int i = 0; i < size(); ++i) {
        std::cout << "  │ " << std::setw(6) << (i + 1) << " │ "
                  << std::setw(12) << elements[i] << " │\n";
    }

    std::cout << "  └────────┴──────────────┘\n\n";
}

void Universe::printTablePaged() const {
    const int pageSize = 20;
    int totalPages = (size() + pageSize - 1) / pageSize;

    std::cout << "  Элементы универсума (" << size() << " шт.):\n\n";

    // Выводим первые 10 элементов
    std::cout << "  Первые элементы:\n";
    for (int i = 0; i < std::min(10, size()); ++i) {
        std::cout << "    " << std::setw(6) << (i + 1) << ". " << elements[i] << "\n";
    }

    if (size() > 20) {
        std::cout << "\n    ... (" << (size() - 20) << " элементов пропущено) ...\n\n";
    }

    // Выводим последние 10 элементов
    if (size() > 10) {
        std::cout << "  Последние элементы:\n";
        int start = std::max(10, size() - 10);
        for (int i = start; i < size(); ++i) {
            std::cout << "    " << std::setw(6) << (i + 1) << ". " << elements[i] << "\n";
        }
    }

    std::cout << "\n";
}
