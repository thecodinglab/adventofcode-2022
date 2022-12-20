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
        num * 811589153,
    });
  }

  int64_t len = numbers.size();

  for (int k = 0; k < 10; k++) {
    for (size_t i = 0; i < len; i++) {
      int64_t idx = find_if(numbers.begin(), numbers.end(),
                            [i](const node &n) { return n.idx == i; }) -
                    numbers.begin();

      node node = numbers[idx];

      int64_t new_idx = mod(idx + node.num, len - 1);

      numbers.erase(numbers.begin() + idx);
      numbers.insert(numbers.begin() + new_idx, node);

      // cout << node.num << ": " << numbers << endl;
    }

    cout << numbers << endl;
  }

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
