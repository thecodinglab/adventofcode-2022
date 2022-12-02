#include "base.hpp"

int main() {
  int current_calories = 0;
  int max_calories = 0;

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    if (line.empty()) {
      max_calories = max(current_calories, max_calories);
      current_calories = 0;
    } else {
      int num = atoi(line.c_str());
      current_calories += num;
    }
  }

  cout << max_calories << "awd" << endl;
  return 0;
}
