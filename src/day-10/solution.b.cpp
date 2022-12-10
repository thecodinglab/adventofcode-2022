#include "base.hpp"

int main() {
  int reg_x = 1;
  int cycle = 0;

  int width = 40;

  while (1) {
    string instruction;
    getline(cin, instruction);

    if (!cin)
      break;

    auto tick = [&]() {
      int pos = cycle % 40;

      if (pos == reg_x - 1 || pos == reg_x || pos == reg_x + 1) {
        cout << "#";
      } else {
        cout << " ";
      }

      cycle++;

      if (cycle != 0 && cycle % width == 0)
        cout << endl;
    };

    tick();

    if (instruction.find("add") == 0) {
      char reg;
      int diff;
      sscanf(instruction.c_str(), "add%c %d", &reg, &diff);

      tick();
      reg_x += diff;
    }
  }

  return 0;
}
