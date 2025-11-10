#pragma once
#include "universe.h"

class Multiset : public Universe {
private:
    std::map<std::string, int> elements;

public:
    Multiset(const Universe& u);

    void fillManual(int size);
    void fillRandom();

    int getMultiplicity(const std::string& element) const;
    void setMultiplicity(const std::string& element, int m);

    /**
    * Объединение мультимножеств (max кратностей)
    * A ∪ B: для каждого элемента берём максимальную кратность
    */
    Multiset unionWith(const Multiset& other) const;

    /**
    * Пересечение мультимножеств (min кратностей)
    * A ∩ B: для каждого элемента берём минимальную кратность
    */
    Multiset intersectionWith(const Multiset& other) const;

    /**
    * Разность мультимножеств
    * A \ B: кратность элемента = max(0, count_A - count_B)
    */
    Multiset differenceWith(const Multiset& other) const;

    /**
    * Симметрическая разность
    * A △ B = (A \ B) ∪ (B \ A)
    */
    Multiset symmetricDifferenceWith(const Multiset& other) const;

    /**
    * Дополнение до универсума
    * A' = U \ A, где U - универсум с максимальными кратностями
    */
    Multiset complement() const;

    /**
    * Арифметическая сумма
    * A + B: кратность элемента = count_A + count_B (с учётом maxMultiplicity)
    */
    Multiset arithmeticSum(const Multiset& other) const;

    /**
    * Арифметическая разность
    * A - B: кратность элемента = max(0, count_A - count_B)
    */
    Multiset arithmeticDifference(const Multiset& other) const;

    /**
    * Арифметическое произведение
    * A * B: кратность элемента = count_A * count_B
    */
    Multiset arithmeticProduct(const Multiset& other) const;

    /**
    * @brief Арифметическое деление
    * A / B: кратность элемента = count_A / count_B (целочисленное деление)
    * Если count_B = 0, элемент не включается в результат
    */
    Multiset arithmeticDivision(const Multiset& other) const;

    void printTable() const;

    bool isEmpty() const;
    bool operator!=(const Multiset& other) const;
    bool operator==(const Multiset& other) const;
};
