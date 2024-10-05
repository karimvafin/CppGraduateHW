#ifndef CACHELRU_HPP
#define CACHELRU_HPP

#include <list>
#include <unordered_map>

namespace Cache {

template <typename T, typename KeyT = int>
struct CacheLRU {
private:
    using ListIt = typename std::list<T>::iterator;

    const size_t size_;
    std::list<T> cache_;
    std::unordered_map<KeyT, ListIt> hashTable_;

public:
    CacheLRU(size_t size);

    [[nodiscard]] bool full() const;

    template <typename F>
    bool lookupUpdate(KeyT key, F getPageSlow);
};

template <typename T, typename KeyT>
CacheLRU<T, KeyT>::CacheLRU(size_t size) : size_(size) {}

template <typename T, typename KeyT>
bool CacheLRU<T, KeyT>::full() const {
    return (cache_.size() == size_);
}

template <typename T, typename KeyT>
template <typename F>
bool CacheLRU<T, KeyT>::lookupUpdate(KeyT key, F getPageSlow) {
    const auto hit = hashTable_.find(key);
    if (hit == hashTable_.end()) {
        if (full()) {
            hashTable_.erase(cache_.back().getId());
            cache_.pop_back();
        }
        cache_.push_front(getPageSlow(key));
        hashTable_[key] = cache_.begin();
        return false;
    }
    auto itPage = hit->second;
    if (itPage != cache_.begin()) {
        cache_.splice(cache_.begin(), cache_, itPage, std::next(itPage));
    }
    return true;
}

}  // namespace Cache

#endif  // CACHELRU_HPP