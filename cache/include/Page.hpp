#ifndef PAGE_HPP
#define PAGE_HPP

#include <string>

namespace Cache {

struct Page {
    using KeyT = int;

    KeyT id_;
    std::string data_;

    KeyT getId() const {
        return id_;
    }
};

}  // namespace Cache

#endif  // PAGE_HPP