#ifndef TEST_FRAME_H_
#define TEST_FRAME_H_

#include <vector>
#include <string>
#include <set>

class Rand {
private:
    unsigned long long seed;
    static const int lim;
    static const unsigned long long multiplier;
    static const unsigned long long addend;
    static const unsigned long long mask;
public:
    /* Returns random permutation of the given size (values are between 0 and size-1)*/
    
    template<typename T>
    std::vector<T> perm(T size) {
        return perm(size, T(0));
    }

    /* Returns random permutation of the given size (values are between `first` and `first`+size-1)*/
    template<typename T, typename E>
    std::vector<E> perm(T size, E first) {
        if (size < 0) {
            exit(1);
            // __testlib_fail("random_t::perm(T size, E first = 0): size must non-negative");
        }
        else if (size == 0)
            return std::vector<E>();
        std::vector<E> p(size);
        E current = first;
        for (T i = 0; i < size; i++)
            p[i] = current++;
        if (size > 1)
            for (T i = 1; i < size; i++)
                std::swap(p[i], p[next((int64_t)i + 1LL)]);
        return p;
    }

    /* Returns random (unsorted) partition which is a representation of sum as a sum of integers not less than min_part. */
    template<typename T>
    std::vector<T> partition(int size, T sum, T min_part) {
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
    std::vector<T> partition(int size, T sum) {
        return partition(size, sum, T(1));
    }

    /* Returns `size` unordered (unsorted) distinct numbers between `from` and `to`. */
    template<typename T>
    std::vector<T> distinct(int size, T from, T to) {
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

    /* Returns `size` unordered (unsorted) distinct numbers between `0` and `upper`-1. */
    template<typename T>
    std::vector<T> distinct(int size, T upper) {
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

    long long nextBits(int bits);

    /* Random double value in range [0, 1). */
    int next(int nodes);

    /*
    * Weighted next. If type == 0 than it is usual "next()".
    *
    * If type = 1, than it returns "max(next(), next())"
    * (the number of "max" functions equals to "type").
    *
    * If type < 0, than "max" function replaces with "min".
    */
    int wnext(int nodes, int type);
    
    /* Random double value in range [0, 1). */
    double next();

    /* Returns random value in range [from,to]. */
    int next(int from, int to);

    /* Random value in range [0, nodes-1]. */
    long long next(long long nodes);

    int wnext(int from, int to, int type);

    template<typename _RandomAccessIter>
    void shuffle(_RandomAccessIter __first, _RandomAccessIter __last);
};

extern Rand rnd;

template<typename _RandomAccessIter>
void Rand::shuffle(_RandomAccessIter __first, _RandomAccessIter __last) {
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        std::iter_swap(__i, __first + rnd.next(int(__i - __first) + 1));
}

#endif