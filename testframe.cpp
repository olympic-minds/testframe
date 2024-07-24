#include "testframe.hpp"
#include <climits>
#include <cmath>

/* Returns `size` unordered (unsorted) distinct numbers between `0` and `upper`-1. */
template<typename T>
std::vector<T> Rand::distinct(int size, T upper) {
    if (size < 0) {
        // __testlib_fail("random_t::distinct expected size >= 0");
        exit(1);
    }
    if (size == 0)
        return std::vector<T>();

    if (upper <= 0) { 
        // __testlib_fail("random_t::distinct expected upper > 0");
        exit(1);
    }
    if (size > upper) {
        // __testlib_fail("random_t::distinct expected size <= upper");
        exit(1);
    }

    return distinct(size, T(0), upper - 1);
}

/* Returns random permutation of the given size (values are between `first` and `first`+size-1)*/
template<typename T, typename E>
std::vector<E> Rand::perm(T size, E first) {
    if (size < 0) {
        // __testlib_fail("random_t::perm(T size, E first = 0): size must non-negative");
        exit(1);
    }
    else if (size == 0)
        return std::vector<E>();
    std::vector<E> p(size);
    E current = first;
    for (T i = 0; i < size; i++)
        p[i] = current++;
    if (size > 1)
        for (T i = 1; i < size; i++)
            std::swap(p[i], p[next(i + 1)]);
    return p;
}

/* Returns `size` unordered (unsorted) distinct numbers between `from` and `to`. */
template<typename T>
std::vector<T> Rand::distinct(int size, T from, T to) {
    std::vector<T> result;
    if (size == 0)
        return result;

    if (from > to) {
        // __testlib_fail("random_t::distinct expected from <= to");   
        exit(1);
    }

    if (size < 0) {
        // __testlib_fail("random_t::distinct expected size >= 0");
        exit(1);
    }

    uint64_t nodes = to - from + 1;
    if (uint64_t(size) > nodes) {
        // __testlib_fail("random_t::distinct expected size <= to - from + 1");
        exit(1);
    }

    double expected = 0.0;
    for (int i = 1; i <= size; i++)
        expected += double(nodes) / double(nodes - i + 1);

    if (expected < double(nodes)) {
        std::set<T> vals;
        while (int(vals.size()) < size) {
            T x = T(next(from, to));
            if (vals.insert(x).second)
                result.push_back(x);
        }
    } else {
        if (nodes > 1000000000) {
            // __testlib_fail("random_t::distinct here expected to - from + 1 <= 1000000000");
            exit(1);
        }
        std::vector<T> p(perm(int(nodes), from));
        result.insert(result.end(), p.begin(), p.begin() + size);
    }

    return result;
}

/* Returns random (unsorted) partition which is a representation of sum as a sum of integers not less than min_part. */
template<typename T>
std::vector<T> Rand::partition(int size, T sum, T min_part) {
    if (size < 0) {
        // __testlib_fail("random_t::partition: size < 0");
        exit(1);
    }
    if (size == 0 && sum != 0) {
        // __testlib_fail("random_t::partition: size == 0 && sum != 0");
        exit(1);
    }
    if (min_part * size > sum) {
        // __testlib_fail("random_t::partition: min_part * size > sum");
        exit(1);
    }
    if (size == 0 && sum == 0)
        return std::vector<T>();

    T sum_ = sum;
    sum -= min_part * size;

    std::vector<T> septums(size);
    std::vector<T> d = distinct(size - 1, T(1), T(sum + size - 1));
    for (int i = 0; i + 1 < size; i++)
        septums[i + 1] = d[i];
    sort(septums.begin(), septums.end());

    std::vector<T> result(size);
    for (int i = 0; i + 1 < size; i++)
        result[i] = septums[i + 1] - septums[i] - 1;
    result[size - 1] = sum + size - 1 - septums.back();

    for (std::size_t i = 0; i < result.size(); i++)
        result[i] += min_part;

    T result_sum = 0;
    for (std::size_t i = 0; i < result.size(); i++)
        result_sum += result[i];
    if (result_sum != sum_) {
        // __testlib_fail("random_t::partition: partition sum is expected to be the given sum");
        exit(1);
    }

    if (*std::min_element(result.begin(), result.end()) < min_part) {
        // __testlib_fail("random_t::partition: partition min is expected to be no less than the given min_part");
        exit(1);
    }

    if (int(result.size()) != size || result.size() != (size_t) size) {
        //  __testlib_fail("random_t::partition: partition size is expected to be equal to the given size");
        exit(1);
    }
    return result;
}

/* Returns random (unsorted) partition which is a representation of sum as a sum of positive integers. */
template<typename T>
std::vector<T> Rand::partition(int size, T sum) {
    return partition(size, sum, T(1));
}

template<typename T>
std::vector<T> Rand::perm(T size) {
    return perm(size, T(0));
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

        return crop((int) (double(nodes) * p), 0, nodes);
    }
}

double Rand::next() {
    long long left = ((long long) (nextBits(26)) << 27);
    long long right = nextBits(27);
    return crop((double) (left + right) / (double) (1LL << 53), 0.0, 1.0);
}

static inline double crop(double value, double a, double b) {
    value = std::min(std::max(value, a), b);
    if (value >= b)
        value = std::nexttoward(b, a);
    return value;
}


const unsigned long long Rand::multiplier = 0x5DEECE66DLL;
const unsigned long long Rand::addend = 0xBLL;
const unsigned long long Rand::mask = (1LL << 48) - 1;
const int Rand::lim = 25;
