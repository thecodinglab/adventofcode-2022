#include "base.hpp"

void run(istream &stream) {
  unordered_set<tuple<int, int, int>> cubes;

  while (1) {
    int x, y, z;
    char _;

    stream >> x >> _ >> y >> _ >> z;

    if (!stream)
      break;

    cubes.insert({x, y, z});
  }

  vector<tuple<int, int, int>> neighbors = {
      {0, 0, -1}, //
      {0, 0, +1}, //
      {0, -1, 0}, //
      {0, +1, 0}, //
      {-1, 0, 0}, //
      {+1, 0, 0}, //
  };

  constexpr int SIZE = 40;
  tuple<int, int, int> start = {-SIZE, -SIZE, -SIZE};

  int total = 0;

  queue<tuple<int, int, int>> q;
  unordered_set<tuple<int, int, int>> visited;

  q.push(start);

  while (!q.empty()) {
    auto pos = q.front();
    q.pop();

    int x = get<0>(pos);
    int y = get<1>(pos);
    int z = get<2>(pos);

    if (x < -SIZE || y < -SIZE || z < -SIZE || x >= SIZE || y >= SIZE ||
        z >= SIZE)
      continue;

    if (visited.contains(pos))
      continue;

    visited.insert(pos);

    int exterior_sides = 0;

    for (const auto &offset : neighbors) {
      tuple<int, int, int> neighbor = {
          get<0>(pos) + get<0>(offset),
          get<1>(pos) + get<1>(offset),
          get<2>(pos) + get<2>(offset),
      };

      if (cubes.contains(neighbor))
        exterior_sides++;
      else
        q.push(neighbor);
    }

    total += exterior_sides;
  }

  cout << total << endl;
}

#include <fstream>

int main(int argc, char **argv) {
  if (argc == 1) {
    run(cin);
    return 0;
  }

  ifstream stream(argv[1]);
  run(stream);
  stream.close();

  return 0;
}
