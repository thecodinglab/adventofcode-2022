#include "base.hpp"

int main() {
  vector<vector<char>> stacks;

  while (1) {
    string input;
    getline(cin, input);

    if (!cin || input.empty())
      break;

    int count = (input.length() + 1) / 4;
    stacks.resize(count);

    for (int i = 0; i < count; i++) {
      int idx = i * 4 + 1;
      char c = input[idx];

      if (c == ' ' || (c >= '0' && c <= '9'))
        continue;

      stacks[i].insert(stacks[i].end(), c);
    }
  }

  cout << stacks << endl;

  while (1) {
    string rearrangement;
    getline(cin, rearrangement);

    if (!cin)
      break;

    int count, from_idx, to_idx;
    sscanf(rearrangement.c_str(), "move %d from %d to %d", &count, &from_idx,
           &to_idx);

    auto &from = stacks[from_idx - 1];
    auto &to = stacks[to_idx - 1];

    for (int i = 0; i < count; i++) {
      char c = from[0];
      cout << "move " << c << " from " << from_idx << " to " << to_idx << endl;

      from.erase(from.begin());
      to.insert(to.begin(), c);
    }
  }

  for (auto &stack : stacks) {
    cout << stack[0];
  }
  cout << endl;
  return 0;
}
