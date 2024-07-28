#pragma once
#ifndef OLYMPIC_MINDS_TESTFRAME_RAND_HPP
#define OLYMPIC_MINDS_TESTFRAME_RAND_HPP


#include <random>
#include <cstdint>
#include <algorithm>


struct [[nodiscard]] Random {
public:
    using IntType = std::uint64_t;

    IntType seed{};    
    std::mt19937_64 engine{};

public:
    explicit Random(IntType seed) noexcept
        : seed(seed), engine(seed)
    {}

public:
    // one uniform IntType from interval [a, b)
    [[nodiscard]] IntType intFromRange(IntType a, IntType b);
    // one uniform IntType from interval [0, b)
    [[nodiscard]] inline IntType intFromRange(IntType b) {
        return intFromRange(0, b);
    }

    // n uniform ints from interval [a, b)
    [[nodiscard]] std::vector<IntType> intsFromRange(std::size_t n, IntType a, IntType b);
    // n uniform ints from interval [0, b)
    [[nodiscard]] inline std::vector<IntType> intsFromRange(std::size_t n, IntType b) {
        return intsFromRange(n, 0, b);
    }

    // one uniform double from interval [a, b)
    [[nodiscard]] double doubleFromRange(double a, double b);
    // one uniform double from interval [0.0, b)
    [[nodiscard]] inline double doubleFromRange(double b) {
        return doubleFromRange(0., b);
    }
    // one uniform double from interval [0.0, 1.0)
    [[nodiscard]] inline double doubleFrom01() {
        return doubleFromRange(0., 1.);
    }

    // n uniform doubles from interval [a, b)
    [[nodiscard]] std::vector<double> doublesFromRange(std::size_t n, double a, double b);
    // n uniform doubles from interval [0.0, b)
    [[nodiscard]] inline std::vector<double> doublesFromRange(std::size_t n, double b) {
        return doublesFromRange(n, 0.0, b);
    }
    // n uniform doubles from interval [0.0, 1.0)
    [[nodiscard]] inline std::vector<double> doublesFrom01(std::size_t n) {
        return doublesFromRange(n, 0.0, 1.0);
    }

    // shuffled permutation of [a, a+n)
    [[nodiscard]] std::vector<IntType> perm(std::size_t n, IntType a = 0) noexcept;

    // shuffle a range
    template<typename RandomIt>
    inline void shuffle(RandomIt first, RandomIt last) {
        std::shuffle(first, last, engine);
    }
    // shuffle a range
    template<class R>
    [[nodiscard]] inline R shuffle(R&& range) {
        std::shuffle(std::begin(range), std::end(range), engine);
        return range;
    }

    // selects min(n, b-a) distinct numbers from [a, b); stable
    [[nodiscard]] std::vector<IntType> distinct(std::size_t n, IntType a, IntType b) noexcept;
    // selects min(n, b) distinct numbers from [0, b); stable
    [[nodiscard]] inline std::vector<IntType> distinct(std::size_t n, IntType b) noexcept {
        return distinct(n, IntType{0}, b);
    }

    // splits sum into n parts such that each of them is at least min
    [[nodiscard]] std::vector<IntType> partition(std::size_t n, IntType sum, IntType min = 1) noexcept;
};


#endif // OLYMPIC_MINDS_TESTFRAME_RAND_HPP
