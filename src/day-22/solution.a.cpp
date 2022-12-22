#include "base.hpp"

#include <exception>

void run(istream &stream) {
  constexpr char EMPTY = ' ';
  constexpr char PATH = '.';
  constexpr char WALL = '#';

  using vec2_t = pair<int64_t, int64_t>;

  vector<char> grid;

  int64_t grid_width = 1000, grid_height = 0;

  // parse grid
  while (1) {
    string line;
    getline(stream, line);

    if (line.empty())
      break;

    // TODO  width

    for (char c : line)
      grid.push_back(c);

    for (size_t i = line.size(); i < grid_width; i++)
      grid.push_back(EMPTY);

    grid_height++;
  }

  string direction;
  getline(stream, direction);

  vec2_t pos = {0, 0};
  vec2_t dir = {1, 0};

  for (int64_t i = 0; i < grid_width; i++) {
    if (grid[i] == PATH) {
      pos.first = i;
      break;
    }
  }

  auto print_grid = [&]() {
    for (int64_t y = 0; y < grid_height; y++) {
      for (int64_t x = 0; x < grid_width; x++) {
        if (x == pos.first && y == pos.second)
          cout << 'P';
        else {
          int64_t idx = y * grid_width + x;
          cout << grid[idx];
        }
      }

      cout << endl;
    }

    cout << endl;
  };

  auto get_char_at = [&](const vec2_t &pos) {
    int64_t idx = pos.second * grid_width + pos.first;
    return grid[idx];
  };

  constexpr int64_t IDX_RIGHT = 0;
  constexpr int64_t IDX_DOWN = 1;
  constexpr int64_t IDX_LEFT = 2;
  constexpr int64_t IDX_UP = 3;

  auto get_facing_index = [](vec2_t facing) {
    if (facing.first == 1 && facing.second == 0)
      return IDX_RIGHT;
    else if (facing.first == 0 && facing.second == 1)
      return IDX_DOWN;
    else if (facing.first == -1 && facing.second == 0)
      return IDX_LEFT;
    else if (facing.first == 0 && facing.second == -1)
      return IDX_UP;
    else {
      cout << "illegal facing value " << facing << endl;
      assert(false);
    }
  };

  auto move = [&](const vec2_t &pos, const vec2_t &dir) {
    vec2_t next = {
        mod(pos.first + dir.first, grid_width),
        mod(pos.second + dir.second, grid_height),
    };

    while (get_char_at(next) == EMPTY) {
      next.first = mod(next.first + dir.first, grid_width);
      next.second = mod(next.second + dir.second, grid_height);
    }

    if (get_char_at(next) == WALL)
      return pos;

    return next;
  };

  for (size_t i = 0; i < direction.size(); i++) {
    char c = direction[i];
    if (c >= '0' && c <= '9') {
      string number;
      while (i < direction.size() && c >= '0' && c <= '9') {
        number += c;
        c = direction[++i];
      }

      i--;

      int64_t steps = stol(number);
      for (int64_t i = 0; i < steps; i++) {
        auto next = move(pos, dir);
        if (next == pos)
          break;

        // cout << next << " " << get_char_at(next) << endl;

        pos = next;

        // print_grid();
      }

      // cout << "STEP END" << endl;

    } else {
      switch (c) {
      case 'L': {
        dir = {
            dir.second,
            -dir.first,
        };
      } break;

      case 'R': {
        dir = {
            -dir.second,
            dir.first,
        };
      } break;
      }
    }
  }

  int64_t facing_value = get_facing_index(dir);

  int64_t res = 1000 * (pos.second + 1) + 4 * (pos.first + 1) + facing_value;
  cout << res << endl;
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
