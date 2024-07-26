#ifndef UTILS_H_
#define UTILS_H_

#include <filesystem>
#include <map>
#include <vector>
#include <iostream>


// map for keeping the names of directories
// promptInputDirectory - directory for in files formatted for prompt
// promptInputDirectory - directory for in files formatted for the solution
const std::map<std::string, std::string> dirs = {{"promptInputDirectory", "in"},
                                                 {"solutionInputDirectory", "solution-in"}};

template <typename F, typename S>
std::ostream &operator<<(std::ostream &os, const std::pair<F, S> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "{";
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            os << ", ";
        }
        os << *it;
    }
    return os << "}";
}

void println();

template <typename Head, typename... Tail>
void println(Head H, Tail... T);

void setupDirectories();

template <typename T>
concept ConvertibleToInt64_t = std::convertible_to<T, int64_t>;

template<ConvertibleToInt64_t T>
int64_t changeVectorToInt64_t(std::vector<T> &v);

template<ConvertibleToInt64_t T>
int64_t changeVectorOfPairsToInt64_t(std::vector<std::pair<T, T>> &v);

#endif

