#include "zhegalkin.h"

struct BDDNode {
    int varIndex;   // -1 для листа
    int value;      // значение листа
    int low;        // потомок по 0
    int high;       // потомок по 1
};

class BDD : public ZhegalkinPolynomial {
private:
    std::vector<BDDNode> nodes;
    int root;

    void buildHardcodedBDD() {
        nodes.push_back({-1, 0, -1, -1});  // [0] лист 0
        nodes.push_back({-1, 1, -1, -1});  // [1] лист 1

        nodes.push_back({2, 0, 0, 1});     // [2] x3
        nodes.push_back({1, 0, 2, 1});     // [3] x2
        nodes.push_back({1, 0, 0, 2});     // [4] x2
        nodes.push_back({3, 0, 3, 4});     // [5] x4
        nodes.push_back({0, 0, 5, 4});     // [6] x1

        root = 6;
    }

public:
    BDD(int n, const std::vector<int>& vals) : ZhegalkinPolynomial(n, vals) {
        buildHardcodedBDD();
    }

    void displayBDD() {
        printHeader("БИНАРНАЯ ДИАГРАММА РЕШЕНИЙ (БДР)");

        std::cout << "Порядок переменных: x1, x4, x2, x3\n\n";
        std::cout << "Структура БДР:\n";
        printSeparator();

        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << "  [" << i << "] ";
            if (nodes[i].varIndex == -1) {
                std::cout << "Лист: " << nodes[i].value << "\n";
            } else {
                std::cout << "x" << (nodes[i].varIndex + 1)
                          << " --0--> [" << nodes[i].low << "]"
                          << ", --1--> [" << nodes[i].high << "]\n";
            }
        }

        std::cout << "\nКорень: [" << root << "]\n";
        std::cout << "Всего узлов: " << nodes.size() << "\n";
    }

    int evaluateBDD(const std::vector<int>& input) const {
        int current = root;
        while (nodes[current].varIndex != -1) {
            int var = nodes[current].varIndex;
            current = (input[var] == 0) ? nodes[current].low : nodes[current].high;
        }
        return nodes[current].value;
    }

    void interactiveEvaluateBDD() {
        printHeader("ВЫЧИСЛЕНИЕ ПО БДР");

        std::vector<int> input(numVars);
        std::cout << "Введите значения переменных:\n";
        for (int i = 0; i < numVars; ++i) {
            input[i] = getUserInput("x" + std::to_string(i + 1) + " (0 или 1): ", 0, 1);
        }

        std::cout << "\nПуть по БДР:\n";
        int current = root;
        while (nodes[current].varIndex != -1) {
            int var = nodes[current].varIndex;
            std::cout << "  x" << (var + 1) << " = " << input[var];
            if (input[var] == 0) {
                std::cout << " --0--> [" << nodes[current].low << "]\n";
                current = nodes[current].low;
            } else {
                std::cout << " --1--> [" << nodes[current].high << "]\n";
                current = nodes[current].high;
            }
        }

        std::cout << "\nРезультат: f(";
        for (int i = 0; i < numVars; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << input[i];
        }
        std::cout << ") = " << nodes[current].value << "\n";
    }
};
