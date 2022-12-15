#include "base.hpp"

#define EMPTY '.'
#define SENSOR 'S'
#define BEACON 'B'

struct sensor {
  pair<int, int> position;
  pair<int, int> nearest_beacon;
  int dist;
};

int dist(const pair<int, int> &a, const pair<int, int> &b) {
  return abs(a.first - b.first) + abs(a.second - b.second);
}

void run(istream &stream) {
  vector<sensor> sensors;
  set<pair<int, int>> beacons;

  pair<int, int> min_bounds;
  pair<int, int> max_bounds;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    sensor current;
    sscanf(line.c_str(),
           "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
           &current.position.first, &current.position.second,
           &current.nearest_beacon.first, &current.nearest_beacon.second);

    current.dist = dist(current.position, current.nearest_beacon);
    sensors.push_back(current);

    beacons.insert(current.nearest_beacon);

    min_bounds = pair<int, int>{
        min(min_bounds.first,
            min(current.position.first, current.nearest_beacon.first)),
        min(min_bounds.second,
            min(current.position.second, current.nearest_beacon.second)),
    };

    max_bounds = pair<int, int>{
        max(max_bounds.first,
            max(current.position.first, current.nearest_beacon.first)),
        max(max_bounds.second,
            max(current.position.second, current.nearest_beacon.second)),
    };

    cout << current.position << ": " << current.nearest_beacon << endl;
  }

  int width = max_bounds.first - min_bounds.first;
  int height = max_bounds.second - min_bounds.second;

  int y = 2000000;
  int num_blocked = 0;

  for (int x = min_bounds.first - 10'000'000; x < max_bounds.first + 10'000'000;
       x++) {
    pair<int, int> pos = {x, y};
    if (beacons.find(pos) != beacons.end()) {
      // cout << "beacon blocks position at " << pos << endl;
      continue;
    }

    bool blocked = false;
    for (const auto &s : sensors) {
      int d = dist(pos, s.position);
      if (d <= s.dist) {
        // cout << "sensor at " << s.position << " blocks position " << pos
        //      << endl;
        blocked = true;
        break;
      }
    }

    if (blocked)
      num_blocked++;
  }

  cout << num_blocked << endl;
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
