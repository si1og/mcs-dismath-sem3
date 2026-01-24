#include "truth_table.h"

class ZhegalkinPolynomial : public TruthTable {
private:
    std::vector<int> coefficients;

    void buildCoefficients() {
        int numRows = 1 << numVars;
        std::vector<std::vector<int>> triangle(numRows);

        triangle[0] = values;

        for (int i = 1; i < numRows; ++i) {
            triangle[i].resize(numRows - i);
            for (int j = 0; j < numRows - i; ++j) {
                triangle[i][j] = triangle[i-1][j] ^ triangle[i-1][j+1];
            }
        }

        coefficients.resize(numRows);
        for (int i = 0; i < numRows; ++i) {
            coefficients[i] = triangle[i][0];
        }
    }

public:
    ZhegalkinPolynomial(int n, const std::vector<int>& vals) : TruthTable(n, vals) {
        buildCoefficients();
    }

    void displayPolynomial() {
        printHeader("ПОЛИНОМ ЖЕГАЛКИНА");

        std::string result;
        int numRows = 1 << numVars;
        bool first = true;

        for (int i = 0; i < numRows; ++i) {
            if (coefficients[i] == 1) {
                if (!first) result += " ⊕ ";
                first = false;

                if (i == 0) {
                    result += "1";
                } else {
                    std::string term;
                    for (int j = 0; j < numVars; ++j) {
                        if ((i >> (numVars - 1 - j)) & 1) {
                            term += "x" + std::to_string(j + 1);
                        }
                    }
                    result += term;
                }
            }
        }

        std::cout << "f = " << (result.empty() ? "0" : result) << "\n";
    }

    int evaluatePolynomial(const std::vector<int>& input) const {
        int result = 0;
        int numRows = 1 << numVars;

        for (int i = 0; i < numRows; ++i) {
            if (coefficients[i] == 1) {
                int term = 1;
                for (int j = 0; j < numVars; ++j) {
                    if ((i >> (numVars - 1 - j)) & 1) {
                        term &= input[j];
                    }
                }
                result ^= term;
            }
        }
        return result;
    }

    void interactiveEvaluate() {
        printHeader("ВЫЧИСЛЕНИЕ ПО ПОЛИНОМУ ЖЕГАЛКИНА");

        std::vector<int> input(numVars);
        std::cout << "Введите значения переменных:\n";
        for (int i = 0; i < numVars; ++i) {
            input[i] = getUserInput("x" + std::to_string(i + 1) + " (0 или 1): ", 0, 1);
        }

        int result = evaluatePolynomial(input);
        std::cout << "\nf(";
        for (int i = 0; i < numVars; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << input[i];
        }
        std::cout << ") = " << result << "\n";
    }
};
