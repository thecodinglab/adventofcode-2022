#include "base.hpp"

int sign(int x) { return (x == 0) ? 0 : (x < 0) ? -1 : 1; }

int main() {
  pair<int, int> head_position = {0, 0};
  pair<int, int> tail_position = {0, 0};

  set<pair<int, int>> visited = {tail_position};

  while (1) {
    char direction;
    int steps;

    cin >> direction >> steps;

    if (!cin)
      break;

    for (int i = 0; i < steps; i++) {
      switch (direction) {
      case 'R':
        head_position.first++;
        break;

      case 'U':
        head_position.second++;
        break;

      case 'L':
        head_position.first--;
        break;

      case 'D':
        head_position.second--;
        break;

      default:
        break;
      }

      pair<int, int> dir = {
          (head_position.first - tail_position.first),
          (head_position.second - tail_position.second),
      };

      int dist = (dir.first * dir.first) + (dir.second * dir.second);

      if (dist >= 4) {
        pair<int, int> offset = {sign(dir.first), sign(dir.second)};
        tail_position.first += offset.first;
        tail_position.second += offset.second;

        visited.insert(tail_position);
      }
    }
  }

  cout << visited.size() << endl;
  return 0;
}
