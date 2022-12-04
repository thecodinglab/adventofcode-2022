#include "base.hpp"

std::pair<int, int> parse(const std::string &val) {
  auto idx = val.find('-');
  int start = atoi(val.substr(0, idx).c_str());
  int end = atoi(val.substr(idx + 1).c_str());
  return {start, end};
}

int main() {
  int count = 0;

  while (1) {
    string value;
    cin >> value;

    if (!cin)
      break;

    auto separator_idx = value.find(',');

    auto a = parse(value.substr(0, separator_idx));
    auto b = parse(value.substr(separator_idx + 1));

    if (max(a.first, b.first) <= min(a.second, b.second))
      count++;
  }

  cout << count << endl;
  return 0;
}
