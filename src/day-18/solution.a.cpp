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

  int total = 0;

  for (const auto &cube : cubes) {
    int sides = 6;
    for (const auto &offset : neighbors) {
      tuple<int, int, int> neighbor = {
          get<0>(cube) + get<0>(offset),
          get<1>(cube) + get<1>(offset),
          get<2>(cube) + get<2>(offset),
      };

      if (cubes.contains(neighbor))
        sides--;
    }

    total += sides;
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
