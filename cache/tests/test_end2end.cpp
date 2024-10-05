#include <iostream>

#include <gtest/gtest.h>

#include "CacheLRU.hpp"
#include "CacheARC.hpp"
#include "Page.hpp"
#include "Util.hpp"

Cache::Page getPageSlow(Cache::Page::KeyT key) {
    return Cache::Page{key, "Data"};
}

TEST(CACHE, END2END_TEST) {
    size_t cacheSize;
    std::cin >> cacheSize;
    Utils::checkCin("Incorrect size!");
    assert(cacheSize % 4 == 0);  // Need for ARC Cache
    Cache::CacheLRU<Cache::Page, Cache::Page::KeyT> cacheLRU(cacheSize);
    Cache::CacheARC<Cache::Page, Cache::Page::KeyT> cacheARC(cacheSize / 4);

    size_t numPages;
    std::cin >> numPages;
    Utils::checkCin("Incorrect number of pages!");

    std::vector<Cache::Page::KeyT> keys;  // Save keys for ideal cache
    keys.reserve(numPages);
    size_t hitsLRU = 0;
    size_t hitsARC = 0;
    for (size_t i = 0; i < numPages; ++i) {
        Cache::Page page;
        std::cin >> page.id_;
        keys.push_back(page.getId());
        Utils::checkCin("Incorrect page id!");
        if (cacheLRU.lookupUpdate(page.id_, getPageSlow)) {
            ++hitsLRU;
        }
        if (cacheARC.lookupUpdate(page.id_, getPageSlow)) {
            ++hitsARC;
        }
    }
    std::cout << "LRU Cache hits: " << hitsLRU << " out of " << numPages << std::endl;
    std::cout << "ARC Cache hits: " << hitsARC << " out of " << numPages << std::endl;
    std::cout << "Ideal Cache hits: " << Utils::getHitsFromIdealCache(cacheSize, keys) << " out of " << numPages << std::endl;
}