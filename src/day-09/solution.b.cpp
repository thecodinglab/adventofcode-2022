#include "base.hpp"

int sign(int x) { return (x == 0) ? 0 : (x < 0) ? -1 : 1; }

int main() {
  vector<pair<int, int>> knots(10, {0, 0});

  set<pair<int, int>> visited = {knots[0]};

  while (1) {
    char direction;
    int steps;

    cin >> direction >> steps;

    if (!cin)
      break;

    for (int i = 0; i < steps; i++) {
      switch (direction) {
      case 'R':
        knots[0].first++;
        break;

      case 'U':
        knots[0].second++;
        break;

      case 'L':
        knots[0].first--;
        break;

      case 'D':
        knots[0].second--;
        break;

      default:
        break;
      }

      for (int i = 1; i < knots.size(); i++) {
        pair<int, int> dir = {
            (knots[i - 1].first - knots[i].first),
            (knots[i - 1].second - knots[i].second),
        };

        int dist = (dir.first * dir.first) + (dir.second * dir.second);

        if (dist >= 4) {
          pair<int, int> offset = {sign(dir.first), sign(dir.second)};
          knots[i].first += offset.first;
          knots[i].second += offset.second;

          if (i == knots.size() - 1)
            visited.insert(knots[i]);
        }
      }
    }
  }

  cout << visited.size() << endl;
  return 0;
}
