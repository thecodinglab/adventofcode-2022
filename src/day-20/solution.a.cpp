#include "base.hpp"

struct node {
  int64_t idx;
  int64_t num;
};

ostream &operator<<(ostream &o, const node &n) { return o << n.num; }

void run(istream &stream) {
  vector<node> numbers;

  while (1) {
    int64_t num;
    stream >> num;

    if (!stream)
      break;

    numbers.push_back({
        (int64_t)numbers.size(),
        num,
    });
  }

  int64_t len = numbers.size();

  for (size_t i = 0; i < len; i++) {
    int64_t idx = find_if(numbers.begin(), numbers.end(),
                          [i](const node &n) { return n.idx == i; }) -
                  numbers.begin();

    node node = numbers[idx];

    int64_t new_idx = mod(idx + node.num, len - 1);

    if (new_idx > idx) {
      auto from = numbers.begin() + idx;
      auto to = numbers.begin() + new_idx;
      rotate(from, from + 1, to + 1);
    } else if (new_idx < idx) {
      auto from = numbers.rbegin() + (len - idx - 1);
      auto to = numbers.rbegin() + (len - new_idx - 1);
      rotate(from, from + 1, to + 1);
    }

    // cout << node.num << ": " << numbers << endl;
  }

  // cout << numbers << endl;

  vector<int64_t> indices = {
      1000,
      2000,
      3000,
  };

  int64_t zero_idx = find_if(numbers.begin(), numbers.end(),
                             [](const node &node) { return node.num == 0; }) -
                     numbers.begin();

  int64_t sum = 0;
  for (const auto &idx : indices) {
    int64_t i = zero_idx + idx;

    cout << idx << " " << numbers[i % numbers.size()].num << endl;
    sum += numbers[i % numbers.size()].num;
  }

  cout << sum << endl;
}

#include <fstream>

int main(int argc, char **argv) {
  if (argc == 1) {
    run(cin);
    return 0;
  }

  ifstream stream(argv[1]);
  run(stream);
  stream.close();

  return 0;
}
