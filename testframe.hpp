#ifndef TEST_FRAME_H_
#define TEST_FRAME_H_

#include <vector>

/* Returns random permutation of the given size (values are between 0 and size-1)*/
template<typename T>
std::vector<T> perm(T size);

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

#endif