#include <iostream>

#include "CacheIdeal.hpp"
#include "Page.hpp"
#include "Util.hpp"

Cache::Page getPageSlow(Cache::Page::KeyT key) {
    return Cache::Page{key, "Data"};
}

int main() {
    size_t cacheSize;
    std::cin >> cacheSize;
    Utils::checkCin("Incorrect size!");

    size_t numPages;
    std::cin >> numPages;
    Utils::checkCin("Incorrect number of pages!");

    std::vector<Cache::Page::KeyT> keys;  // Save keys for ideal cache
    keys.reserve(numPages);
    for (size_t i = 0; i < numPages; ++i) {
        Cache::Page page;
        std::cin >> page.id_;
        keys.push_back(page.getId());
        Utils::checkCin("Incorrect page id!");
    }

    Cache::CacheIdeal<Cache::Page, Cache::Page::KeyT> cacheIdeal(cacheSize, keys);

    std::cout << cacheIdeal.getHits() << std::endl;
}