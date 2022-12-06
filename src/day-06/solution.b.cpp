#include "base.hpp"

int main() {
  string val;
  cin >> val;

  int start = 0;
  int window_size = 14;

  for (int j = 0; j < val.length() - window_size; j++) {
    set<char> characters;
    for (int i = start; i < (start + window_size); i++) {
      characters.insert(val[i]);
    }

    if (characters.size() == window_size) {
      cout << start + window_size << endl;
      break;
    } else {
      start++;
    }
  }

  return 0;
}
