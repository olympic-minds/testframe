#include <random>
#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>
#include <ranges>
#include <algorithm>

// random_shuffle z zakresu
// partition
// shuffle

class Random {
public:
    using IntType = std::uint64_t;
    class trueRandomSeed_t {};

    IntType seed;    
    std::mt19937_64 engine;

public:
    Random(IntType seed = 137)
        : seed(seed), engine(seed)
    {}

    // seeds with random device
    Random(trueRandomSeed_t)
        : seed(std::random_device{}()), engine(seed)
    {}

public:
    // one uniform IntType from interval [a, b)
    IntType intFromRange(IntType a, IntType b) {
        assert(a < b);
        std::uniform_int_distribution<IntType> dist(a, b - 1);
        return dist(engine);
    }
    // one uniform IntType from interval [0, b)
    IntType intFromRange(IntType b) {
        return intFromRange(0, b);
    }

    // n uniform ints from interval [a, b)
    std::vector<IntType> intsFromRange(std::size_t n, IntType a, IntType b) {
        assert(a < b);
        std::vector<IntType> ret(n);
        std::uniform_int_distribution<IntType> dist(a, b - 1);
        while(n--) {
            ret[n] = dist(engine);
        }
        return ret;
    }
    // n uniform ints from interval [0, b)
    std::vector<IntType> intsFromRange(std::size_t n, IntType b) {
        assert(0 < b);
        return intsFromRange(n, 0, b);
    }

    // one uniform double from interval [a, b)
    double doubleFromRange(double a, double b) {
        assert(std::isfinite(a) && std::isfinite(b) && a < b);
        std::uniform_real_distribution dist(a, b);
        return dist(engine);
    }
    // one uniform double from interval [0.0, b)
    double doubleFromRange(double b) {
        return doubleFromRange(0., b);
    }
    // one uniform double from interval [0.0, 1.0)
    double doubleFrom01() {
        return doubleFromRange(0., 1.);
    }

    // n uniform doubles from interval [a, b)
    std::vector<double> doublesFromRange(std::size_t n, double a, double b) {
        assert(std::isfinite(a) && std::isfinite(b) && a < b);
        std::vector<double> ret(n);
        std::uniform_real_distribution dist(a, b);
        while(n--) {
            ret[n] = dist(engine);
        }
        return ret;
    }
    // n uniform doubles from interval [0.0, b)
    std::vector<double> doublesFromRange(std::size_t n, double b) {
        return doublesFromRange(n, 0.0, b);
    }
    // n uniform doubles from interval [0.0, 1.0)
    std::vector<double> doublesFrom01(std::size_t n) {
        return doublesFromRange(n, 0.0, 1.0);
    }

    // shuffled permutation of [0, n)
    std::vector<IntType> perm(std::size_t n) {
        return perm(n, IntType{0});
    }
    // shuffled permutation of [from, from+n)
    std::vector<IntType> perm(std::size_t n, IntType from) {
        std::vector<IntType> ret(n);
        std::iota(std::begin(ret), std::end(ret), IntType{from});
        std::shuffle(std::begin(ret), std::end(ret), engine);
        return ret;
    }

    // shuffle a range
    template<typename RandomIt>
    void shuffle(RandomIt first, RandomIt last) {
        std::shuffle(first, last, engine);
    }



};
