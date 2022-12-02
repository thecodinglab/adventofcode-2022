#include "base.hpp"

int main() {
  int total = 0;

  while (1) {
    char opponent, self;
    cin >> opponent >> self;
    if (!cin)
      break;

    int opponent_idx = opponent - 'A';
    int self_idx = self - 'X';

    total += self_idx + 1;

    if (opponent_idx == self_idx)
      total += 3;

    int winning_idx = ((opponent_idx + 1) % 3);
    if (self_idx == winning_idx)
      total += 6;
  }

  cout << total << endl;
  return 0;
}
