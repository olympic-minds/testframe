#pragma once
#ifndef OLYMPIC_MINDS_TESTFRAME_RAND_HPP
#define OLYMPIC_MINDS_TESTFRAME_RAND_HPP


#include <random>
#include <cstdint>
#include <algorithm>
#include <cassert>


struct [[nodiscard]] Random {
public:
    using IntType = std::uint64_t;
    // using IntType = std::int64_t;    // signed?

    IntType seed{};    
    std::mt19937_64 engine{};

public:
    explicit Random(IntType seed = 137) noexcept
        : seed(seed), engine(seed)
    {}

public:
    // one uniform IntType from interval [a, b)
    [[nodiscard]] IntType intFromRange(IntType a, IntType b) noexcept(false);
    // one uniform IntType from interval [0, b)
    [[nodiscard]] inline IntType intFromRange(IntType b) noexcept(false) {
        return intFromRange(0, b);
    }

    // n uniform ints from interval [a, b)
    [[nodiscard]] std::vector<IntType> intsFromRange(std::size_t n, IntType a, IntType b) noexcept(false);
    // n uniform ints from interval [0, b)
    [[nodiscard]] inline std::vector<IntType> intsFromRange(std::size_t n, IntType b) noexcept(false) {
        return intsFromRange(n, 0, b);
    }

    // one uniform double from interval [a, b)
    [[nodiscard]] double doubleFromRange(double a, double b) noexcept(false);
    // one uniform double from interval [0.0, b)
    [[nodiscard]] inline double doubleFromRange(double b) noexcept(false) {
        return doubleFromRange(0., b);
    }
    // one uniform double from interval [0.0, 1.0)
    [[nodiscard]] inline double doubleFrom01() noexcept {
        return doubleFromRange(0., 1.);
    }

    // n uniform doubles from interval [a, b)
    [[nodiscard]] std::vector<double> doublesFromRange(std::size_t n, double a, double b) noexcept(false);
    // n uniform doubles from interval [0.0, b)
    [[nodiscard]] inline std::vector<double> doublesFromRange(std::size_t n, double b) noexcept(false) {
        return doublesFromRange(n, 0.0, b);
    }
    // n uniform doubles from interval [0.0, 1.0)
    [[nodiscard]] inline std::vector<double> doublesFrom01(std::size_t n) noexcept {
        return doublesFromRange(n, 0.0, 1.0);
    }

    // shuffled permutation of [a, a+n)
    [[nodiscard]] std::vector<IntType> perm(std::size_t n, IntType a = 0) noexcept;

    // shuffle a range
    template<class RandomIt>
    inline void shuffle(RandomIt first, RandomIt last) noexcept(false) {
        std::shuffle(first, last, engine);
    }
    // shuffle a range
    template<class R>
    [[nodiscard]] inline R shuffle(R&& range) noexcept(false) {
        std::shuffle(std::begin(range), std::end(range), engine);
        return range;
    }

    // selects min(n, b-a) distinct numbers from [a, b); stable
    [[nodiscard]] std::vector<IntType> distinct(std::size_t n, IntType a, IntType b) noexcept(false);
    // selects min(n, b) distinct numbers from [0, b); stable
    [[nodiscard]] inline std::vector<IntType> distinct(std::size_t n, IntType b) noexcept(false) {
        return distinct(n, IntType{0}, b);
    }

    // splits sum into n parts such that each of them is at least min
    [[nodiscard]] std::vector<IntType> partition(std::size_t n, IntType sum, IntType min = 1)  noexcept(false);

    // gets a double from beta distribution with parameters alpha and beta
    [[nodiscard]] inline double betaDist(double alpha, double beta) noexcept(false);

    // gets a double from [a, b) with weighted distribution based on type: 
    // type = 0 -> uniform distribution
    // type > 0 -> max of type+1 values from uniform distributions
    // type < 0 -> min of -type-1 values from uniform distributions
    [[nodiscard]] inline IntType wnext(IntType a, IntType b, std::int64_t type) noexcept(false) {
        assert(a < b);
        return a + wnext(b - a, type);
    }

    // gets a double from [0, b) with weighted distribution based on type: 
    // type = 0 -> uniform distribution
    // type > 0 -> max of type+1 values from uniform distributions
    // type < 0 -> min of -type-1 values from uniform distributions
    [[nodiscard]] IntType wnext(IntType b, int64_t type) noexcept(false);
};

extern Random rndm;


#endif // OLYMPIC_MINDS_TESTFRAME_RAND_HPP
