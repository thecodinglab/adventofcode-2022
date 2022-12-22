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

  auto grid_in_bounds = [&](const vec2_t &pos) {
    return pos.first >= 0 && pos.second < grid_width && pos.second >= 0 &&
           pos.second < grid_height;
  };

  // INPUT
  // . 5 6
  // . 4 .
  // 2 3 .
  // 1 . .

  auto face = [&](const vec2_t &pos) -> int64_t {
    auto x = pos.first / 50;
    auto y = pos.second / 50;

    if (x == 0 && y == 3)
      return 1;

    if (x == 0 && y == 2)
      return 2;

    if (x == 1 && y == 2)
      return 3;

    if (x == 1 && y == 1)
      return 4;

    if (x == 1 && y == 0)
      return 5;

    if (x == 2 && y == 0)
      return 6;

    cout << "invalid face: " << pos << " -> " << x << " " << y << endl;
    assert(false);
  };

  auto wrap = [&](const vec2_t &pos,
                  const vec2_t &dir) -> pair<vec2_t, vec2_t> {
    auto f = face(pos);

    auto rel_x = pos.first % 50;
    auto rel_y = pos.second % 50;

    if (dir.first == 1) {
      // RIGHT
      if (f == 1)
        return {{50 + rel_y, 149}, {0, -1}};

      if (f == 3)
        return {{149, 0 + (49 - rel_y)}, {-1, 0}};

      if (f == 4)
        return {{100 + rel_y, 49}, {0, -1}};

      if (f == 6)
        return {{99, 100 + (49 - rel_y)}, {-1, 0}};

    } else if (dir.second == 1) {
      // DOWN
      if (f == 1)
        return {{100 + rel_x, 0}, {0, 1}};

      if (f == 3)
        return {{49, 150 + rel_x}, {-1, 0}};

      if (f == 6)
        return {{99, 50 + rel_x}, {-1, 0}};

    } else if (dir.first == -1) {
      // LEFT
      if (f == 1)
        return {{50 + rel_y, 0}, {0, 1}};

      if (f == 2)
        return {{50, 0 + (49 - rel_y)}, {1, 0}};

      if (f == 4)
        return {{0 + rel_y, 100}, {0, 1}};

      if (f == 5)
        return {{0, 100 + (49 - rel_y)}, {1, 0}};

    } else if (dir.second == -1) {
      // UP
      if (f == 2)
        return {{50, 50 + rel_x}, {1, 0}};

      if (f == 5)
        return {{0, 150 + rel_x}, {1, 0}};

      if (f == 6)
        return {{0 + rel_x, 199}, {0, -1}};
    }

    assert(false);
  };

  auto move = [&](const vec2_t &pos, const vec2_t &dir) {
    vec2_t next_pos = {
        pos.first + dir.first,
        pos.second + dir.second,
    };

    vec2_t next_dir = dir;

    if (!grid_in_bounds(next_pos) || get_char_at(next_pos) == EMPTY) {
      // calculate position and direction for next
      auto [a, b] = wrap(pos, dir);
      next_pos = a;
      next_dir = b;

      assert(b.first != 0 || b.second != 0);
    }

    if (get_char_at(next_pos) == WALL)
      return make_pair(pos, dir);

    return make_pair(next_pos, next_dir);
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
        if (next.first == pos)
          break;

        // cout << next << " " << get_char_at(next.first) << endl;

        pos = next.first;
        dir = next.second;

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
