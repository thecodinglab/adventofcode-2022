#pragma once

#include <ostream>
#include <utility>

template <class T1, class T2>
std::ostream &operator<<(std::ostream &out, const std::pair<T1, T2> &pair) {
  return out << '(' << pair.first << ", " << pair.second << ')';
}

template <template <class, class> class Container, class T,
          class Allocator = std::allocator<T>>
std::ostream &operator<<(std::ostream &out,
                         const Container<T, Allocator> &container) {
  out << '[';
  for (auto iter = container.begin(); iter != container.end(); ++iter) {
    if (iter != container.begin())
      out << ", ";

    out << *iter;
  }
  out << ']';

  return out;
}
