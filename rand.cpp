#include <cassert>

#include "rand.hpp"


using IntType = Random::IntType;

IntType Random::intFromRange(IntType a, IntType b) {
    assert(a < b);
    std::uniform_int_distribution<IntType> dist(a, b - 1);
    return dist(engine);
}

std::vector<IntType> Random::intsFromRange(std::size_t n, IntType a, IntType b) {
    assert(a < b);
    std::vector<IntType> ret(n);
    std::uniform_int_distribution<IntType> dist(a, b - 1);
    while(n--) {
        ret[n] = dist(engine);
    }
    return ret;
}

double Random::doubleFromRange(double a, double b) {
    assert(std::isfinite(a) && std::isfinite(b) && a < b);
    std::uniform_real_distribution dist(a, b);
    return dist(engine);
}

std::vector<double> Random::doublesFromRange(std::size_t n, double a, double b) {
    assert(std::isfinite(a) && std::isfinite(b) && a < b);
    std::vector<double> ret(n);
    std::uniform_real_distribution dist(a, b);
    while(n--) {
        ret[n] = dist(engine);
    }
    return ret;
}

std::vector<IntType> Random::perm(std::size_t n, IntType a) noexcept {
    std::vector<IntType> ret(n);
    std::iota(std::begin(ret), std::end(ret), IntType{a});
    std::shuffle(std::begin(ret), std::end(ret), engine);
    return ret;
}

std::vector<IntType> Random::distinct(std::size_t n, IntType a, IntType b) noexcept {
    std::vector<IntType> ret;
    ret.reserve(n);

    std::vector<IntType> range(b - a);
    std::iota(std::begin(range), std::end(range), a);
    std::ranges::sample(range, std::back_inserter(ret), n, engine);

    return ret;
}

std::vector<IntType> Random::partition(std::size_t n, IntType sum, IntType min) noexcept {
    assert(sum >= n * min);
    IntType adjusted_sum = sum - n * min;

    std::vector<IntType> points = intsFromRange(n-1, adjusted_sum);
    points.emplace_back(IntType{0});
    points.emplace_back(adjusted_sum);
    
    std::sort(std::begin(points), std::end(points));

    std::vector<IntType> partitions(n);
    for(std::size_t i = 0; i < n; ++i) {
        partitions[i] = points[i + 1] - points[i] + min;
    }

    return shuffle(partitions);
}