#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>

/**
 * @brief Класс для работы с мультимножествами на основе кода Грея
 *
 * Мультимножество - множество, в котором элементы могут повторяться.
 * Каждый элемент характеризуется кратностью (multiplicity).
 */
class Multiset {
private:
    std::map<std::string, int> elements; // Элементы мультимножества и их кратности

    // Статические поля для универсума
    static std::vector<std::string> universe;  // Универсум всех возможных элементов
    static int maxMultiplicity;                // Максимальная кратность элементов

public:
    // ============================================================
    // КОНСТРУКТОРЫ И ДЕСТРУКТОР
    // ============================================================

    /**
     * @brief Конструктор по умолчанию - создаёт пустое мультимножество
     */
    Multiset();

    /**
     * @brief Конструктор копирования
     */
    Multiset(const Multiset& other);

    /**
     * @brief Оператор присваивания
     */
    Multiset& operator=(const Multiset& other);

    /**
     * @brief Деструктор
     */
    ~Multiset();

    // ============================================================
    // СТАТИЧЕСКИЕ МЕТОДЫ ДЛЯ НАСТРОЙКИ УНИВЕРСУМА
    // ============================================================

    /**
     * @brief Генерирует бинарный код Грея заданной разрядности
     * @param n Разрядность (количество бит)
     * @return Вектор строк с кодами Грея
     *
     * Код Грея - последовательность двоичных чисел, в которой
     * два соседних числа различаются ровно в одном бите.
     * Для n=3: 000, 001, 011, 010, 110, 111, 101, 100
     */
    static std::vector<std::string> generateGrayCode(int n);

    /**
     * @brief Создаёт универсум на основе кода Грея заданной разрядности
     * @param depth Разрядность кода Грея
     */
    static void createUniverse(int depth);

    /**
     * @brief Устанавливает максимальную кратность для элементов
     * @param limit Максимальная кратность (должна быть > 0)
     */
    static void setMaxMultiplicity(int limit);

    /**
     * @brief Возвращает максимальную кратность
     */
    static int getMaxMultiplicity();

    /**
     * @brief Возвращает ссылку на универсум
     */
    static const std::vector<std::string>& getUniverseElements();

    /**
     * @brief Возвращает универсум как мультимножество с макс. кратностями
     */
    static Multiset getUniverseAsMultiset();

    // ============================================================
    // МЕТОДЫ ДЛЯ ЗАПОЛНЕНИЯ МУЛЬТИМНОЖЕСТВА
    // ============================================================

    /**
     * @brief Ручное заполнение мультимножества
     * @param targetSize Желаемое количество уникальных элементов
     *
     * Пользователь вводит элементы и их кратности вручную
     */
    void fillManual(int targetSize);

    /**
     * @brief Автоматическое заполнение мультимножества случайными элементами
     * @param targetSize Желаемое количество уникальных элементов
     */
    void fillRandom(int targetSize);

    // ============================================================
    // МЕТОДЫ ДЛЯ РАБОТЫ С ЭЛЕМЕНТАМИ
    // ============================================================

    /**
     * @brief Возвращает кратность элемента в мультимножестве
     * @param element Элемент для проверки
     * @return Кратность элемента (0 если элемента нет)
     */
    int getMultiplicity(const std::string& element) const;

    /**
     * @brief Проверяет, принадлежит ли элемент универсуму
     */
    bool isInUniverse(const std::string& element) const;

    /**
     * @brief Проверяет, можно ли увеличить кратность элемента
     */
    bool canIncreaseMultiplicity(const std::string& element) const;

    /**
     * @brief Добавляет элемент с заданной кратностью
     * @param element Элемент для добавления
     * @param count Кратность (по умолчанию 1)
     * @return true если успешно, false если превышен лимит
     */
    bool add(const std::string& element, int count = 1);

    /**
     * @brief Удаляет элемент или уменьшает его кратность
     * @param element Элемент для удаления
     * @param count Насколько уменьшить кратность (по умолчанию 1)
     * @return true если успешно
     */
    bool remove(const std::string& element, int count = 1);

    /**
     * @brief Очищает мультимножество
     */
    void clear();

    /**
     * @brief Возвращает количество уникальных элементов
     */
    int size() const;

    /**
     * @brief Возвращает общую мощность (сумму всех кратностей)
     */
    int cardinality() const;

    /**
     * @brief Проверяет, пусто ли мультимножество
     */
    bool isEmpty() const;

    // ============================================================
    // ОПЕРАЦИИ НАД МУЛЬТИМНОЖЕСТВАМИ
    // ============================================================

    /**
     * @brief Объединение мультимножеств (max кратностей)
     * A ∪ B: для каждого элемента берём максимальную кратность
     */
    Multiset unionWith(const Multiset& other) const;

    /**
     * @brief Пересечение мультимножеств (min кратностей)
     * A ∩ B: для каждого элемента берём минимальную кратность
     */
    Multiset intersectionWith(const Multiset& other) const;

    /**
     * @brief Разность мультимножеств
     * A \ B: кратность элемента = max(0, count_A - count_B)
     */
    Multiset differenceWith(const Multiset& other) const;

    /**
     * @brief Симметрическая разность
     * A △ B = (A \ B) ∪ (B \ A)
     */
    Multiset symmetricDifferenceWith(const Multiset& other) const;

    /**
     * @brief Дополнение до универсума
     * A' = U \ A, где U - универсум с максимальными кратностями
     */
    Multiset complement() const;

    // ============================================================
    // АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ
    // ============================================================

    /**
     * @brief Арифметическая сумма
     * A + B: кратность элемента = count_A + count_B (с учётом maxMultiplicity)
     */
    Multiset arithmeticSum(const Multiset& other) const;

    /**
     * @brief Арифметическая разность
     * A - B: кратность = max(0, count_A - count_B)
     */
    Multiset arithmeticDifference(const Multiset& other) const;

    /**
     * @brief Арифметическое произведение
     * A * B: кратность элемента = count_A * count_B
     */
    Multiset arithmeticProduct(const Multiset& other) const;

    /**
     * @brief Арифметическое деление
     * A / B: кратность элемента = count_A / count_B (целочисленное деление)
     * Если count_B = 0, элемент не включается в результат
     */
    Multiset arithmeticDivision(const Multiset& other) const;

    // ============================================================
    // ОПЕРАТОРЫ
    // ============================================================

    /**
     * @brief Оператор == для сравнения мультимножеств
     */
    bool operator==(const Multiset& other) const;

    /**
     * @brief Оператор != для сравнения
     */
    bool operator!=(const Multiset& other) const;

    // ============================================================
    // МЕТОДЫ ВЫВОДА
    // ============================================================

    /**
     * @brief Выводит элементы универсума
     */
    static void printUniverseElements();

    /**
     * @brief Выводит универсум как мультимножество
     */
    void printUniverseAsMultiset() const;

    /**
     * @brief Выводит содержимое мультимножества
     */
    void print() const;

    /**
     * @brief Выводит подробную информацию о мультимножестве
     */
    void printDetailed() const;

    // ============================================================
    // ИТЕРАТОРЫ (для удобства перебора)
    // ============================================================

    auto begin() { return elements.begin(); }
    auto end() { return elements.end(); }
    auto begin() const { return elements.cbegin(); }
    auto end() const { return elements.cend(); }

    /**
     * @brief Возвращает const ссылку на внутренний map
     */
    const std::map<std::string, int>& getElements() const;
};

// ============================================================
// ИНИЦИАЛИЗАЦИЯ СТАТИЧЕСКИХ ЧЛЕНОВ
// ============================================================

std::vector<std::string> Multiset::universe;
int Multiset::maxMultiplicity = 1;
