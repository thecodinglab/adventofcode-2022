#include "base.hpp"

using vec2_t = pair<int64_t, int64_t>;

struct elve_t {
  vec2_t position;

  vec2_t proposed_position;
  bool can_move;
};

void run(istream &stream) {
  vector<elve_t> elves;
  int y = 0;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    for (size_t i = 0; i < line.size(); i++) {
      char c = line[i];
      if (c == '#') {
        vec2_t position = {i, y};
        elve_t elve = {position};
        elves.push_back(elve);
      }
    }

    y++;
  }

  array<array<vec2_t, 3>, 4> dirs = {
      array<vec2_t, 3>{vec2_t{0, -1}, vec2_t{-1, -1}, vec2_t{+1, -1}}, // north
      array<vec2_t, 3>{vec2_t{0, +1}, vec2_t{-1, +1}, vec2_t{+1, +1}}, // south
      array<vec2_t, 3>{vec2_t{-1, 0}, vec2_t{-1, -1}, vec2_t{-1, +1}}, // west
      array<vec2_t, 3>{vec2_t{+1, 0}, vec2_t{+1, -1}, vec2_t{+1, +1}}, // east
  };

  int dir_offset = 0;

  auto is_occupied = [&](const vec2_t &pos) {
    for (const auto &elve : elves) {
      if (elve.position == pos)
        return true;
    }
    return false;
  };

  auto round = [&]() {
    unordered_map<vec2_t, int64_t> proposed_locations;

    // propose locations
    for (auto &elve : elves) {
      elve.can_move = false;

      // check if in any other adjacent tile is another elve
      bool has_adj_elve = false;
      for (const auto &d : dirs) {
        for (const auto &dir : d) {
          vec2_t adj = {
              elve.position.first + dir.first,
              elve.position.second + dir.second,
          };

          if (is_occupied(adj)) {
            has_adj_elve = true;
            break;
          }
        }

        if (has_adj_elve)
          break;
      }

      if (!has_adj_elve)
        continue;

      // propose moving
      for (int i = 0; i < dirs.size(); i++) {
        int dir_idx = (i + dir_offset) % dirs.size();
        bool blocked = false;

        for (const auto &dir : dirs[dir_idx]) {
          vec2_t adj = {
              elve.position.first + dir.first,
              elve.position.second + dir.second,
          };

          if (is_occupied(adj)) {
            blocked = true;
            break;
          }
        }

        if (!blocked) {
          vec2_t proposed = {
              elve.position.first + dirs[dir_idx][0].first,
              elve.position.second + dirs[dir_idx][0].second,
          };

          elve.proposed_position = proposed;
          proposed_locations[proposed]++;
          elve.can_move = true;
          break;
        }
      }
    }

    // move to prposed locations
    bool moved = false;
    for (auto &elve : elves) {
      if (elve.can_move && proposed_locations[elve.proposed_position] == 1) {
        elve.position = elve.proposed_position;
        moved = true;
      }
    }

    dir_offset = (dir_offset + 1) % dirs.size();

    return moved;
  };

  auto bounds = [&]() -> pair<vec2_t, vec2_t> {
    vec2_t rect_min = elves[0].position;
    vec2_t rect_max = elves[0].position;

    for (const auto &elve : elves) {
      rect_min = {
          min(rect_min.first, elve.position.first),
          min(rect_min.second, elve.position.second),
      };

      rect_max = {
          max(rect_max.first, elve.position.first),
          max(rect_max.second, elve.position.second),
      };
    }

    return {rect_min, rect_max};
  };

  auto print_board = [&]() {
    auto [rect_min, rect_max] = bounds();
    cout << rect_min << " " << rect_max << endl;

    for (int y = rect_min.second; y <= rect_max.second; y++) {
      for (int x = rect_min.first; x <= rect_max.first; x++) {
        bool has_elve = is_occupied({x, y});
        if (has_elve)
          cout << '#';
        else
          cout << '.';
      }

      cout << endl;
    }

    cout << endl;
  };

  int64_t i = 1;
  while (round()) {
    i++;
  }

  cout << i << endl;
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
