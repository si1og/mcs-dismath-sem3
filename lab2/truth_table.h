#include "cli_ui.h"
#include <vector>
#include <string>
#include <iomanip>

class TruthTable : public CliUI {
protected:
    int numVars;
    std::vector<int> values;

    void printTruthTable() const {
        std::cout << "| № |";
        for (int i = 1; i <= numVars; ++i) {
            std::cout << " x" << i << " |";
        }
        std::cout << " f |\n";

        std::cout << "|---|";
        for (int i = 0; i < numVars; ++i) {
            std::cout << "----|";
        }
        std::cout << "---|\n";

        int numRows = 1 << numVars;
        for (int i = 0; i < numRows; ++i) {
            std::cout << "|" << std::setw(2) << i << " |";
            for (int j = numVars - 1; j >= 0; --j) {
                std::cout << "  " << ((i >> j) & 1) << " |";
            }
            std::cout << " " << values[i] << " |\n";
        }
    }

public:
    TruthTable(int n, const std::vector<int>& vals) : numVars(n), values(vals) {}

    void displayTable() {
        printHeader("ТАБЛИЦА ИСТИННОСТИ");
        printTruthTable();
    }

    void displaySDNF() {
        printHeader("СДНФ");
        std::string sdnf;
        bool first = true;
        int numRows = 1 << numVars;

        for (int i = 0; i < numRows; ++i) {
            if (values[i] == 1) {
                if (!first) sdnf += " ∨ ";
                first = false;

                std::string term;
                for (int j = numVars - 1; j >= 0; --j) {
                    if (!term.empty()) term += "∧";
                    if ((i >> j) & 1) {
                        term += "x" + std::to_string(numVars - j);
                    } else {
                        term += "¬x" + std::to_string(numVars - j);
                    }
                }
                sdnf += "(" + term + ")";
            }
        }
        std::cout << "f = " << (sdnf.empty() ? "0" : sdnf) << "\n";
    }

    void displaySKNF() {
        printHeader("СКНФ");
        std::string sknf;
        bool first = true;
        int numRows = 1 << numVars;

        for (int i = 0; i < numRows; ++i) {
            if (values[i] == 0) {
                if (!first) sknf += " ∧ ";
                first = false;

                std::string clause;
                for (int j = numVars - 1; j >= 0; --j) {
                    if (!clause.empty()) clause += "∨";
                    if ((i >> j) & 1) {
                        clause += "¬x" + std::to_string(numVars - j);
                    } else {
                        clause += "x" + std::to_string(numVars - j);
                    }
                }
                sknf += "(" + clause + ")";
            }
        }
        std::cout << "f = " << (sknf.empty() ? "1" : sknf) << "\n";
    }

    int evaluate(const std::vector<int>& input) const {
        int index = 0;
        for (int i = 0; i < numVars; ++i) {
            index = (index << 1) | input[i];
        }
        return values[index];
    }

    int getNumVars() const { return numVars; }
    const std::vector<int>& getValues() const { return values; }
};
