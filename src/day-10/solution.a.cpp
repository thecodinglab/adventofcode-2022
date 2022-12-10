#include "base.hpp"

int main() {
  int reg_x = 1;
  int cycle = 0;

  int sum = 0;

  while (1) {
    string instruction;
    getline(cin, instruction);

    if (!cin)
      break;

    auto tick = [&]() {
      cycle++;

      if ((cycle - 20) % 40 == 0) {
        cout << "cycle " << cycle << ": " << reg_x << endl;
        sum += cycle * reg_x;
      }
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

  cout << sum << endl;
  return 0;
}
