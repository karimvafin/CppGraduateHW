#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <cstdlib>

namespace Utils {

inline void checkCin(const std::string& message = "") {
    if (std::cin.fail()) {
        std::cout << message << std::endl;
        std::abort();
    }
}

}  // namespace Utils

#endif  // UTIL_HPP