#pragma once

#include <array>

#include "hash.hpp"

namespace std {

template <class T, size_t N> struct hash<array<T, N>> {
  size_t operator()(const array<T, N> &key) const {
    hash<T> hasher;
    size_t result = 0;
    for (size_t i = 0; i < N; ++i) {
      impl::hash_combine(result, key[i]);
    }
    return result;
  }
};

} // namespace std
