#include "testframe.hpp"
#include <climits>
#include <cmath>
#include <set>

Rand rnd;

static inline double crop(double value, double a, double b) {
    value = std::min(std::max(value, a), b);
    if (value >= b)
        value = std::nexttoward(b, a);
    return value;
}

int64_t Rand::nextBits(int bits) {
    if (bits <= 48) {
        seed = (seed * multiplier + addend) & mask;
        return (int64_t)(seed >> (48 - bits));
    } else {
        if (bits > 63) {
            // __testlib_fail("random_t::nextBits(int bits): nodes must be less than 64");
            exit(1);
        }

        // int lowerBitCount = (random_t::version == 0 ? 31 : 32);
        int lowerBitCount = 32;

        int64_t left = (nextBits(31) << 32);
        int64_t right = nextBits(lowerBitCount);

        return left ^ right;
    }
}

/* Random value in range [0, nodes-1]. */
int Rand::next(int nodes) {
    if (nodes <= 0) {
        // __testlib_fail("random_t::next(int nodes): nodes must be positive");
        exit(1);
    }

    if ((nodes & -nodes) == nodes)  // nodes is a power of 2
        return (int)((nodes * (int64_t)nextBits(31)) >> 31);

    const int64_t limit = INT_MAX / nodes * nodes;

    int64_t bits;
    do {
        bits = nextBits(31);
    } while (bits >= limit);

    return int(bits % nodes);
}

int Rand::wnext(int nodes, int type) {
    if (nodes <= 0) {
        // __testlib_fail("random_t::wnext(int nodes, int type): nodes must be positive");
        exit(1);
    }

    if (abs(type) < Rand::lim) {
        int result = next(nodes);

        for (int i = 0; i < +type; i++)
            result = std::max(result, next(nodes));

        for (int i = 0; i < -type; i++)
            result = std::min(result, next(nodes));

        return result;
    } else {
        double p;

        if (type > 0)
            p = std::pow(next() + 0.0, 1.0 / (type + 1));
        else
            p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

        return (int)crop(nodes * p, 0., static_cast<int>(nodes));
    }
}

double Rand::next() {
    int64_t left = ((int64_t)(nextBits(26)) << 27);
    int64_t right = nextBits(27);
    return crop((double)(left + right) / (double)(1LL << 53), 0.0, 1.0);
}

int Rand::next(int from, int to) {
    return int(next((int64_t)to - from + 1) + from);
}

/* Random value in range [0, nodes-1]. */
int64_t Rand::next(int64_t nodes) {
    if (nodes <= 0) {
        // __testlib_fail("random_t::next(int64_t nodes): nodes must be positive");
        exit(1);
    }

    const int64_t limit = LLONG_MAX / nodes * nodes;

    int64_t bits;
    do {
        bits = nextBits(63);
    } while (bits >= limit);

    return bits % nodes;
}

int64_t Rand::next(int64_t from, int64_t to) {
    return int(next(to - from + 1) + from);
}

/* Returns weighted random value in range [from, to]. */
int Rand::wnext(int from, int to, int type) {
    if (from > to) {
        exit(1);
        // __testlib_fail("random_t::wnext(int from, int to, int type): from can't not exceed to");
    }
    return wnext(to - from + 1, type) + from;
}

const uint64_t Rand::multiplier = 0x5DEECE66DLL;
const uint64_t Rand::addend = 0xBLL;
const uint64_t Rand::mask = (1LL << 48) - 1;
const int Rand::lim = 25;
