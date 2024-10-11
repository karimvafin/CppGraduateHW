#ifndef CACHEIDEAL_HPP
#define CACHEIDEAL_HPP

#include <list>
#include <unordered_map>

namespace Cache {

template <typename T, typename KeyT = int>
struct CacheIdeal {
private:
    using ListIt = typename std::list<KeyT>::iterator;

    const size_t size_;
    std::list<KeyT> cache_;
    std::unordered_map<KeyT, ListIt> hashTable_;
    std::vector<KeyT> pages_;

public:
    CacheIdeal(size_t size, const std::vector<KeyT>& pages) : size_(size), pages_(pages) {}

    size_t getHits() {
        size_t hits = 0;
        for (int i = 0; i < pages_.size(); ++i) {
            auto hit = hashTable_.find(pages_[i]);
            if (hit == hashTable_.end()) {
                if (cache_.size() < size_) {
                    cache_.push_front(pages_[i]);
                    hashTable_[pages_[i]] = cache_.begin();
                } else {
                    ListIt farPage = findMostFarPage(i + 1, pages_[i]);
                    if (farPage == cache_.end()) continue;
                    hashTable_.erase(hashTable_.find(*farPage));
                    cache_.erase(farPage);
                    cache_.push_front(pages_[i]);
                    hashTable_[pages_[i]] = cache_.begin();
                }
            } else {
                ++hits;
            }
        }
        return hits;
    }

private:
    ListIt findMostFarPage(size_t start, KeyT newPage) {
        ListIt res = cache_.end();
        size_t i = 0;
        while (start + i < pages_.size() && pages_[start + i] != newPage) ++i;
        size_t maxDist = i;
        for (ListIt it = cache_.begin(); it != cache_.end(); ++it) {
            i = 0;
            while (start + i < pages_.size() && pages_[start + i] != *it) ++i;
            if (i > maxDist) {
                maxDist = i;
                res = it;
            }
        }
        return res;
    }
};

}  // namespace Cache

#endif  // CACHEIDEAL_HPP