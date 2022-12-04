#include "base.hpp"

int main() {
  int total = 0;

  vector<set<char>> contents;

  while (1) {
    string content;
    cin >> content;

    if (!cin)
      break;

    set<char> symbols;
    for (int i = 0; i < content.length(); i++) {
      char a = content[i];
      symbols.insert(a);
    }

    contents.push_back(symbols);
  }

  for (int i = 0; i < contents.size(); i += 3) {
    set<char> a = contents[i];
    set<char> b = contents[i + 1];
    set<char> c = contents[i + 2];

    unordered_map<char, int> counts;

    for (char val : a)
      counts[val]++;

    for (char val : b)
      counts[val]++;

    for (char val : c)
      counts[val]++;

    cout << counts << endl;

    char common = 0;
    for (auto entry : counts) {
      if (entry.second == 3) {
        common = entry.first;
        break;
      }
    }

    int val = 0;
    if (common >= 'a')
      val = (common - 'a') + 1;
    else
      val = (common - 'A') + 27;

    cout << "found " << common << " " << val << endl;
    total += val;
  }

  cout << total << endl;
  return 0;
}
