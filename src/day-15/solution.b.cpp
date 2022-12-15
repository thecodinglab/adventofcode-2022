#include "base.hpp"

#include <atomic>
#include <thread>

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

  int width = 4000000;
  int height = 4000000;

  // width = 20;
  // height = 20;

  // compute outer edges
  set<pair<int, int>> outer_edges;
  auto add_outer_edge = [&](const pair<int, int> &edge) {
    if (edge.first < 0 || edge.first >= width || edge.second < 0 ||
        edge.second >= height)
      return;

    for (const auto &beacon : beacons) {
      if (edge == beacon)
        return;
    }

    outer_edges.insert(edge);
  };

  for (const auto &s : sensors) {
    int dist = s.dist + 1;
    for (int i = 0; i < dist; i++) {
      int dx = dist - i;
      int dy = i;

      add_outer_edge({
          s.position.first + dx,
          s.position.second + dy,
      });

      add_outer_edge({
          s.position.first - dx,
          s.position.second + dy,
      });

      add_outer_edge({
          s.position.first + dx,
          s.position.second - dy,
      });

      add_outer_edge({
          s.position.first - dx,
          s.position.second - dy,
      });
    }
  }

  cout << "outer edges computed: " << outer_edges.size() << endl;

  // find outer edge which is not blocked by another sensor
  for (const auto &pos : outer_edges) {
    bool blocked = false;
    for (const auto &s : sensors) {
      int d = dist(pos, s.position);
      if (d <= s.dist) {
        blocked = true;
        break;
      }
    }

    if (!blocked) {
      uint64_t value = pos.first * 4000000ULL + pos.second;
      cout << "found position " << pos << " " << value << endl;
      break;
    }
  }
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
