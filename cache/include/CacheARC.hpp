#ifndef CACHEARC_HPP
#define CACHEARC_HPP

#include <list>
#include <unordered_map>
#include <iostream>

namespace Cache {

template <typename T, typename KeyT = int>
struct CacheARC {
private:
    using ListIt = typename std::list<T>::iterator;
    using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

    size_t sizeB1_;
    size_t sizeB2_;
    size_t sizeT1_;
    size_t sizeT2_;
    std::list<T> T1_;
    std::list<T> T2_;
    std::list<T> B1_;
    std::list<T> B2_;
    std::unordered_map<KeyT, ListIt> hashT1_;
    std::unordered_map<KeyT, ListIt> hashT2_;
    std::unordered_map<KeyT, ListIt> hashB1_;
    std::unordered_map<KeyT, ListIt> hashB2_;

private:
    void moveToT2(HashIt hit);
    void moveFromB1(HashIt hit);
    void moveFromB2(HashIt hit);

public:
    CacheARC(size_t size);

    template <typename F>
    bool lookupUpdate(KeyT key, F getPageSlow);

    void print() const;
};

template <typename T, typename KeyT>
CacheARC<T, KeyT>::CacheARC(size_t size) {
    sizeT1_ = 0;
    sizeT2_ = 0;
    sizeB1_ = 0;
    sizeB2_ = 0;
    while (size != 0) {
        sizeT1_++;
        size--;
        if (size != 0) {
            sizeT2_++;
            size--;
        }
        if (size != 0) {
            sizeB1_++;
            size--;
        }
        if (size != 0) {
            sizeB2_++;
            size--;
        }
    }
}

template <typename T, typename KeyT>
template <typename F>
bool CacheARC<T, KeyT>::lookupUpdate(KeyT key, F getPageSlow) {
    auto hit = hashT1_.find(key);
    if (hit != hashT1_.end()) {
        moveToT2(hit);
        return true;
    }
    hit = hashT2_.find(key);
    if (hit != hashT2_.end()) {
        if (hit->second != T2_.begin())
            T2_.splice(T2_.begin(), T2_, hit->second, std::next(hit->second));
        return true;
    }
    hit = hashB1_.find(key);
    if (hit != hashB1_.end()) {
        moveFromB1(hit);
        return true;
    }
    hit = hashB2_.find(key);
    if (hit != hashB2_.end()) {
        moveFromB2(hit);
        return true;
    }
    T1_.push_front(getPageSlow(key));
    hashT1_[key] = T1_.begin();
    if (T1_.size() > sizeT1_) {
        const KeyT moveKey = T1_.back().getId();
        hashT1_.erase(hashT1_.find(moveKey));
        B1_.splice(B1_.begin(), T1_, std::prev(T1_.end()), T1_.end());
        hashB1_[moveKey] = B1_.begin();
        if (B1_.size() > sizeB1_) {
            hashB1_.erase(hashB1_.find(B1_.back().getId()));
            B1_.pop_back();
        }
    }
    return false;
}

template <typename T, typename KeyT>
void CacheARC<T, KeyT>::moveToT2(HashIt hit) {
    const KeyT key = hit->first;
    const ListIt pageIt = hit->second;
    T2_.splice(T2_.begin(), T1_, pageIt, std::next(pageIt));
    hashT1_.erase(hit);
    hashT2_[key] = T2_.begin();
    if (!B1_.empty()) {
        hashB1_.erase(hashB1_.find(B1_.begin()->getId()));
        T1_.splice(T1_.end(), B1_, B1_.begin(), std::next(B1_.begin()));
        hashT1_[T1_.rbegin()->getId()] = std::prev(T1_.end());
    }
    if (T2_.size() > sizeT2_) {
        hashT2_.erase(hashT2_.find(T2_.rbegin()->getId()));
        B2_.splice(B2_.begin(), T2_, std::prev(T2_.end()), T2_.end());
        hashB2_[B2_.begin()->getId()] = B2_.begin();
    }
    if (B2_.size() > sizeB2_) {
        hashB2_.erase(hashB2_.find(B2_.back().getId()));
        B2_.pop_back();
    }
}

template <typename T, typename KeyT>
void CacheARC<T, KeyT>::moveFromB1(HashIt hit) {
    T1_.splice(T1_.begin(), B1_, hit->second, std::next(hit->second));
    hashT1_[hit->first] = T1_.begin();
    hashB1_.erase(hit);
    if (T1_.size() == (sizeT1_ + sizeT2_)) {
        hashT1_.erase(hashT1_.find(T1_.back().getId()));
        B1_.splice(B1_.begin(), T1_, std::prev(T1_.end()), T1_.end());
        hashB1_[B1_.front().getId()] = B1_.begin();
        if (B1_.size() > sizeB1_) {
            hashB1_.erase(hashB1_.find(B1_.back().getId()));
            B1_.pop_back();
        }
    } else {
        ++sizeT1_;
        --sizeT2_; 

        if (T2_.size() > sizeT2_) {
            hashT2_.erase(hashT2_.find(T2_.back().getId()));
            B2_.splice(B2_.begin(), T2_, std::prev(T2_.end()), T2_.end());
            hashB2_[B2_.front().getId()] = B2_.begin();
            if (B2_.size() > sizeB2_) {
                hashB2_.erase(hashB2_.find(B2_.back().getId()));
                B2_.pop_back();
            }
        }

    }
}

template <typename T, typename KeyT>
void CacheARC<T, KeyT>::moveFromB2(HashIt hit) {
    T2_.splice(T2_.begin(), B2_, hit->second, std::next(hit->second));
    hashT2_[hit->first] = T2_.begin();
    hashB2_.erase(hit);
    if (T2_.size() == (sizeT1_ + sizeT2_)) {
        hashT2_.erase(hashT2_.find(T2_.back().getId()));
        B2_.splice(B2_.begin(), T2_, std::prev(T2_.end()), T2_.end());
        hashB2_[B2_.front().getId()] = B2_.begin();
        if (B2_.size() > sizeB2_) {
            hashB2_.erase(hashB2_.find(B2_.back().getId()));
            B2_.pop_back();
        }
    } else {
        ++sizeT2_;
        --sizeT1_; 

        if (T1_.size() > sizeT1_) {
            hashT1_.erase(hashT1_.find(T1_.back().getId()));
            B1_.splice(B1_.begin(), T1_, std::prev(T1_.end()), T1_.end());
            hashB1_[B1_.front().getId()] = B1_.begin();
            if (B1_.size() > sizeB1_) {
                hashB1_.erase(hashB1_.find(B1_.back().getId()));
                B1_.pop_back();
            }
        }
    }
}

template <typename T, typename KeyT>
void CacheARC<T, KeyT>::print() const {
    for (size_t i = 0; i < (sizeB1_ - B1_.size()); ++i) {
        std::cout << ". ";
    }
    for (auto it = B1_.rbegin(), itEnd = B1_.rend(); it != itEnd; ++it) {
        std::cout << it->getId() << " ";
    }
    std::cout << "| ";
    for (size_t i = 0; i < (sizeT1_ - T1_.size()); ++i) {
        std::cout << ". ";
    }
    for (auto it = T1_.rbegin(), itEnd = T1_.rend(); it != itEnd; ++it) {
        std::cout << it->getId() << " ";
    }
    std::cout << "| ";
    for (auto it = T2_.begin(), itEnd = T2_.end(); it != itEnd; ++it) {
        std::cout << it->getId() << " ";
    }
    for (size_t i = 0; i < (sizeT2_ - T2_.size()); ++i) {
        std::cout << ". ";
    }
    std::cout << "| ";
    for (auto it = B2_.begin(), itEnd = B2_.end(); it != itEnd; ++it) {
        std::cout << it->getId() << " ";
    }
    for (size_t i = 0; i < (sizeB2_ - B2_.size()); ++i) {
        std::cout << ". ";
    }
    std::cout << std::endl;
}

}  // namespace Cache

#endif  // CACHEARC_HPP