#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <unordered_map>
#include <list>

namespace Utils {

inline void checkCin(const std::string& message = "") {
    if (std::cin.fail()) {
        std::cout << message << std::endl;
        std::abort();
    }
}

inline std::list<int>::const_iterator findMostFarPage(const std::vector<int>& pages, size_t start, const std::list<int>& cache) {
    std::list<int>::const_iterator res = cache.begin();
    size_t maxDist = 0;
    size_t i = 0;
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        while (start + i < pages.size() && pages[start + i] != *it) ++i;
        if (i > maxDist) {
            maxDist = i;
            res = it;
        }
    }
    return res;
}

inline size_t getHitsFromIdealCache(size_t cacheSize, const std::vector<int>& pages) {
    std::list<int> cache;
    std::unordered_map<int, std::list<int>::iterator> hashTable;
    size_t hits = 0;
    for (int i = 0; i < pages.size(); ++i) {
        auto hit = hashTable.find(pages[i]);
        if (hit == hashTable.end()) {
            if (cache.size() < cacheSize) {
                cache.push_front(pages[i]);
                hashTable[pages[i]] = cache.begin();
            } else {
                const auto farPage = findMostFarPage(pages, i + 1, cache);
                hashTable.erase(hashTable.find(*farPage));
                cache.erase(farPage);
                cache.push_front(pages[i]);
                hashTable[pages[i]] = cache.begin();
            }
        } else {
            ++hits;
        }
    }
    return hits;
}

}  // namespace Utils

#endif  // UTIL_HPP