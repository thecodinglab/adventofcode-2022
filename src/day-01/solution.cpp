#include "base.hpp"

int main() {
  vector<int> calories = {0};

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    int &current = calories[calories.size() - 1];

    if (line.empty()) {
      calories.push_back(0);
    } else {
      int num = atoi(line.c_str());
      current += num;
    }
  }

  sort(calories.begin(), calories.end());

  int total_top_three = 0;
  for (int i = 0; i < 3; i++) {
    total_top_three += calories[calories.size() - i - 1];
  }

  cout << total_top_three << endl;
  return 0;
}
