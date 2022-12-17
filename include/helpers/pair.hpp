#pragma once

#include <functional>
#include <ostream>
#include <utility>

namespace std {

template <class T1, class T2> struct hash<pair<T1, T2>> {
  size_t operator()(const pair<T1, T2> &pair) const {
    return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
  }
};

template <class T1, class T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &pair) {
  return out << '(' << pair.first << ", " << pair.second << ')';
}

} // namespace std
