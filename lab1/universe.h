#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <random>
#include <algorithm>

//TODO: пофиксить инициализацию с 0 разрядностью
class Universe {
    private:
        int depth;
        int maxMultiplicity;
        std::vector<std::string> elements;

        static const int MAX_DEPTH = 20;

    public:
        Universe();
        Universe(int depth, int maxMultiplicity);
        ~Universe();

        static std::vector<std::string> generateGrayCode(int n);

        int getDepth() const;
        int getMaxMultiplicity() const;
        const std::vector<std::string>& getElements() const;

        bool contains(const std::string& element) const;
        int size() const;

        void print() const;
        void printTable() const;
};
