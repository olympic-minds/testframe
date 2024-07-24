#ifndef TEST_FRAME_H_
#define TEST_FRAME_H_

#include <vector>
#include <string>

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
    std::vector<T> perm(T size);
    
    // std::vector<int> perm(int size);

    /* Returns random permutation of the given size (values are between `first` and `first`+size-1)*/
    template<typename T, typename E>
    std::vector<E> perm(T size, E first);

    /* Returns random (unsorted) partition which is a representation of sum as a sum of integers not less than min_part. */
    template<typename T>
    std::vector<T> partition(int size, T sum, T min_part);

    /* Returns random (unsorted) partition which is a representation of sum as a sum of positive integers. */
    template<typename T>
    std::vector<T> partition(int size, T sum);

    /* Returns `size` unordered (unsorted) distinct numbers between `from` and `to`. */
    template<typename T>
    std::vector<T> distinct(int size, T from, T to);

    /* Returns `size` unordered (unsorted) distinct numbers between `0` and `upper`-1. */
    template<typename T>
    std::vector<T> distinct(int size, T upper);

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

#endif