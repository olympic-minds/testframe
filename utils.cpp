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
