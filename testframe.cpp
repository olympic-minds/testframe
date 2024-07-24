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


long long Rand::nextBits(int bits) {
    if (bits <= 48) {
        
        seed = (seed * multiplier + addend) & mask;
        return (long long) (seed >> (48 - bits));
    } else {
        if (bits > 63) {
            // __testlib_fail("random_t::nextBits(int bits): nodes must be less than 64");
            exit(1);
        }

        // int lowerBitCount = (random_t::version == 0 ? 31 : 32);
        int lowerBitCount = 32;

        long long left = (nextBits(31) << 32);
        long long right = nextBits(lowerBitCount);

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
        return (int) ((nodes * (long long) nextBits(31)) >> 31);

    const long long limit = INT_MAX / nodes * nodes;

    long long bits;
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

        return crop(nodes * p, 0., static_cast<int>(nodes));
    }
}

double Rand::next() {
    long long left = ((long long) (nextBits(26)) << 27);
    long long right = nextBits(27);
    return crop((double) (left + right) / (double) (1LL << 53), 0.0, 1.0);
}

int Rand::next(int from, int to) {
    return int(next((long long) to - from + 1) + from);
}

/* Random value in range [0, nodes-1]. */
long long Rand::next(long long nodes) {
    if (nodes <= 0) {
        // __testlib_fail("random_t::next(long long nodes): nodes must be positive");
        exit(1);
    }

    const long long limit = LLONG_MAX / nodes * nodes;

    long long bits;
    do {
        bits = nextBits(63);
    } while (bits >= limit);

    return bits % nodes;
}


/* Returns weighted random value in range [from, to]. */
int Rand::wnext(int from, int to, int type) {
    if (from > to) {
        exit(1);
        // __testlib_fail("random_t::wnext(int from, int to, int type): from can't not exceed to");
    }
    return wnext(to - from + 1, type) + from;
}

const unsigned long long Rand::multiplier = 0x5DEECE66DLL;
const unsigned long long Rand::addend = 0xBLL;
const unsigned long long Rand::mask = (1LL << 48) - 1;
const int Rand::lim = 25;
