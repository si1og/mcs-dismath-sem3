#include <iostream>
#include <string>

class CliUI {
protected:
    void printHeader(const std::string& title) const {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << title << "\n";
        std::cout << std::string(50, '=') << "\n";
    }

    void printSeparator() const {
        std::cout << std::string(50, '-') << "\n";
    }

    int getUserInput(const std::string& prompt, int minVal, int maxVal) const {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                return value;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Ошибка! Введите число от " << minVal << " до " << maxVal << "\n";
        }
    }

public:
    virtual ~CliUI() = default;
};
