#pragma once

#include <functional>

namespace std::impl {

template <class T> inline void hash_combine(size_t &seed, const T &v) {
  seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace std::impl
