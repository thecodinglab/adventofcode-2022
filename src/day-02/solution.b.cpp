#include "base.hpp"

int main() {
  int total = 0;

  while (1) {
    char opponent, target;
    cin >> opponent >> target;
    if (!cin)
      break;

    int opponent_idx = opponent - 'A';
    int target_offset = target - 'Y';

    int self_idx = mod(opponent_idx + target_offset, 3);

    total += self_idx + 1;

    if (opponent_idx == self_idx)
      total += 3;

    int winning_idx = mod((opponent_idx + 1), 3);
    if (self_idx == winning_idx)
      total += 6;
  }

  cout << total << endl;
  return 0;
}
