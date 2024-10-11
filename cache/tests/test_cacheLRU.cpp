#include <iostream>

#include "CacheLRU.hpp"
#include "Page.hpp"
#include "Util.hpp"

Cache::Page getPageSlow(Cache::Page::KeyT key) {
    return Cache::Page{key, "Data"};
}

int main() {
    size_t cacheSize;
    std::cin >> cacheSize;
    Utils::checkCin("Incorrect size!");
    Cache::CacheLRU<Cache::Page, Cache::Page::KeyT> cacheLRU(cacheSize);

    size_t numPages;
    std::cin >> numPages;
    Utils::checkCin("Incorrect number of pages!");

    size_t hits = 0;
    for (size_t i = 0; i < numPages; ++i) {
        Cache::Page page;
        std::cin >> page.id_;
        Utils::checkCin("Incorrect page id!");
        if (cacheLRU.lookupUpdate(page.id_, getPageSlow)) {
            ++hits;
        }
    }

    std::cout << hits << std::endl;
}