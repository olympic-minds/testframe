#include "rand.hpp"

#include <iostream>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {

    // Random r(argc != 1 ? std::stoi(argv[1]) : 137);
    Random r(Random::random_seed);

    for(int i = 0; i < 10; ++i)
        std::cout << r.intFromRange(0, 4) << ' ';
    std::cout << '\n';
    for(auto&& it : r.intsFromRange(10, 3, 6))
        std::cout << it<< ' ';
    std::cout << '\n';
    for(auto&& it : r.intsFromRange(10, 10))
        std::cout << it<< ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i)
        std::cout << r.doubleFromRange(0, 4) << ' ';
    std::cout << '\n';
    for(auto&& it : r.doublesFromRange(10, 3, 6))
        std::cout << it<< ' ';
    std::cout << '\n';
    for(auto&& it : r.doublesFromRange(10, 10))
        std::cout << it<< ' ';
    std::cout << '\n';

    for(int i = 0; i < 10; ++i)
        std::cout << r.doubleFrom01() << ' ';
    std::cout << '\n';
    for(auto&& it : r.doublesFrom01(10))
        std::cout << it<< ' ';
    std::cout << '\n';
    
    for(auto&& it : r.perm(10))
        std::cout << it<< ' ';
    std::cout << '\n';

    for(auto&& it : r.distinct(10, 0, 20))
        std::cout << it<< ' ';
    std::cout << '\n';

    for(int i = 0; i < 4; ++i) {
        for(auto&& it : r.partition(4, 30, 4))
            std::cout << it << ' ';
        std::cout << '\n';
    }

}