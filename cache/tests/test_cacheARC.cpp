#include "CacheARC.hpp"
#include "Page.hpp"
#include "Util.hpp"

Cache::Page getPageSlow(Cache::Page::KeyT key) {
    return Cache::Page{key, "Data"};
}

int main() {
    constexpr bool verbose = false;

    size_t cacheSize;
    std::cin >> cacheSize;
    Utils::checkCin("Incorrect cache size!");

    Cache::CacheARC<Cache::Page, Cache::Page::KeyT> cache{cacheSize};
    size_t numPages;
    std::cin >> numPages;
    Utils::checkCin("Incorrect number od pages!");

    size_t hits = 0;
    for (size_t i = 0; i < numPages; ++i) {
        Cache::Page page;
        std::cin >> page.id_;
        Utils::checkCin("Incorrect page id!");
        if (cache.lookupUpdate(page.id_, getPageSlow)) {
            ++hits;
        }
        if (verbose) cache.print();
    }
    std::cout << hits << std::endl;
}