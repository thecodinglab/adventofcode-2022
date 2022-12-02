#include <type_traits>

template <typename T, typename = typename std::enable_if<
                          std::is_arithmetic<T>::value, T>::type>
T mod(T a, T b) {
  T m = a % b;
  if (m < 0)
    m = (b < 0) ? (m - b) : (m + b);
  return m;
}
