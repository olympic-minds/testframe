#include "utils.hpp"

void println() { std::cout << '\n'; }

template <typename Head, typename... Tail>
void println(Head H, Tail... T) {
    std::cout << ' ' << H;
    println(T...);
}

void setupDirectories() {
    for (const auto &dir : dirs) {
        if (!std::filesystem::create_directory(dir.second)) {
            // Only warning, because you can fail to create a directory if it exists.
            std::cerr << "Warning: Could not create directory " << dir << std::endl;
        }
    }
}


template<ConvertibleToInt64_t T>
std::int64_t changeVectorToInt64_t(std::vector<T> &v) {
    std::int64_t result = 0;
    for (std::int64_t i = 0; i < v.size(); i++) {
        result += (i + 1) * v[i];
    }
    return result;
}

template<ConvertibleToInt64_t T>
std::int64_t changeVectorOfPairsToInt64_t(std::vector<std::pair<T, T>> &v) {
    std::int64_t result = 0;
    for (std::int64_t i = 0; i < v.size(); i++) {
        result += (2*i + 1) * v[i].first + (2*i + 2) * v[i].second;
    }
    return result;
}
