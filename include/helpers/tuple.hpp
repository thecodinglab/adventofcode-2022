#pragma once

#include <utility>

#include "hash.hpp"

namespace std::impl {

template <class Tuple, size_t Index = tuple_size<Tuple>::value - 1>
struct tuple_hash_value {
  static void apply(size_t &seed, const Tuple &tuple) {
    tuple_hash_value<Tuple, Index - 1>::apply(seed, tuple);
    hash_combine(seed, get<Index>(tuple));
  }
};

template <class Tuple> struct tuple_hash_value<Tuple, 0> {
  static void apply(size_t &seed, const Tuple &tuple) {
    hash_combine(seed, get<0>(tuple));
  }
};

} // namespace std::impl

namespace std {

template <typename... Args> struct hash<tuple<Args...>> {
  size_t operator()(const tuple<Args...> &tt) const {
    size_t seed = 0;
    impl::tuple_hash_value<tuple<Args...>>::apply(seed, tt);
    return seed;
  }
};

} // namespace std
