#pragma once

struct ascending {
  template <typename T> bool operator()(const T &a, const T &b) const {
    return a < b;
  }
};

struct descending {
  template <typename T> bool operator()(const T &a, const T &b) const {
    return a > b;
  }
};
