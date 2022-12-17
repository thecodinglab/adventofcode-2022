#include "base.hpp"
#include <array>

void run(istream &stream) {
  vector<vector<pair<int64_t, int64_t>>> rocks = {
      {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},         // horizontal bar
      {{0, 0}, {0, 1}, {0, 2}, {-1, 1}, {1, 1}}, // plus
      {{-1, 0}, {0, 0}, {1, 0}, {1, 1}, {1, 2}}, // L shape
      {{-1, 0}, {-1, 1}, {-1, 2}, {-1, 3}},      // vertical bar
      {{-1, 0}, {0, 0}, {-1, 1}, {0, 1}},        // box
  };

  string jets_input;
  getline(stream, jets_input);

  vector<int> jets(jets_input.size(), 0);
  for (int i = 0; i < jets_input.size(); i++) {
    jets[i] = jets_input[i] == '>' ? +1 : -1;
  }

  constexpr int WIDTH = 7;
  constexpr char CHAR_BLOCKED = '#';
  constexpr char CHAR_EMPTY = ' ';
  vector<array<char, WIDTH>> grid;

  auto in_horizontal_bounds = [&](const pair<int64_t, int64_t> &pos) {
    return pos.first >= 0 && pos.first < WIDTH;
  };

  auto collides_in_grid = [&](const pair<int64_t, int64_t> &pos) {
    if (pos.second >= (int64_t)grid.size())
      return false;

    if (pos.second < 0)
      return true;

    return grid[pos.second][pos.first] == CHAR_BLOCKED;
  };

  auto can_move_horizontal = [&](const pair<int64_t, int64_t> &offset,
                                 const vector<pair<int64_t, int64_t>> &vertices,
                                 int amount) {
    for (const auto &vertex : vertices) {
      pair<int64_t, int64_t> next{
          offset.first + vertex.first + amount,
          offset.second + vertex.second,
      };

      if (!in_horizontal_bounds(next))
        return false;

      if (collides_in_grid(next))
        return false;
    }

    return true;
  };

  auto can_move_vertical = [&](const pair<int64_t, int64_t> &offset,
                               const vector<pair<int64_t, int64_t>> &vertices,
                               int amount) {
    for (const auto &vertex : vertices) {
      pair<int64_t, int64_t> next{
          offset.first + vertex.first,
          offset.second + vertex.second + amount,
      };

      if (collides_in_grid(next))
        return false;
    }

    return true;
  };

  auto insert_vertex = [&](const pair<int64_t, int64_t> &offset,
                           const pair<int64_t, int64_t> &vertex) {
    pair<int64_t, int64_t> pos{
        offset.first + vertex.first,
        offset.second + vertex.second,
    };

    while (pos.second >= grid.size()) {
      array<char, WIDTH> line;
      std::fill(line.begin(), line.end(), CHAR_EMPTY);
      grid.push_back(line);
    }

    grid[pos.second][pos.first] = CHAR_BLOCKED;
  };

  auto insert_shape = [&](const pair<int64_t, int64_t> &offset,
                          const vector<pair<int64_t, int64_t>> &vertices) {
    for (const auto &vertex : vertices) {
      insert_vertex(offset, vertex);
    }
  };

  auto print_grid = [&]() {
    for (auto iter = grid.rbegin(); iter != grid.rend(); ++iter) {
      for (int x = 0; x < WIDTH; x++) {
        cout << (*iter)[x];
      }

      cout << endl;
    }

    cout << endl << endl;
  };

  constexpr int BITMASK_ROWS_INTS = 5;
  constexpr int BITMASK_ROWS_PER_INT = 9;
  constexpr int BITMASK_ROWS = BITMASK_ROWS_INTS * BITMASK_ROWS_PER_INT;

  using grid_bitmask = array<uint64_t, BITMASK_ROWS_INTS>;

  auto build_bitmask_from_grid = [&]() {
    grid_bitmask res;

    for (int i = 0; i < BITMASK_ROWS_INTS; i++) {
      uint64_t mask = 0;

      for (int y = 0; y < BITMASK_ROWS_PER_INT; y++) {
        int idx = (BITMASK_ROWS_INTS - i - 1) * BITMASK_ROWS_PER_INT + y;
        const auto &row = grid[grid.size() - idx - 1];

        for (int x = 0; x < WIDTH; x++) {
          if (row[x] == CHAR_BLOCKED) {
            uint64_t bit = y * WIDTH + x;
            mask |= (1ULL << bit);
          }
        }
      }

      res[i] = mask;
    }

    return res;
  };

  auto build_grid_from_bitmask = [&](grid_bitmask masks) {
    grid.clear();

    for (int i = BITMASK_ROWS_INTS - 1; i >= 0; i--) {
      uint64_t bitmask = masks[i];

      for (int y = BITMASK_ROWS_PER_INT - 1; y >= 0; y--) {
        auto &row = grid.emplace_back();

        for (int x = 0; x < WIDTH; x++) {
          uint64_t bit = y * WIDTH + x;
          if (bitmask & (1ULL << bit)) {
            row[x] = CHAR_BLOCKED;
          } else {
            row[x] = CHAR_EMPTY;
          }
        }
      }
    }
  };

  auto simulate_in_current_grid = [&](int rock_idx, int jet_idx) {
    const vector<pair<int64_t, int64_t>> &vertices = rocks[rock_idx];
    pair<int64_t, int64_t> rock_position = {WIDTH / 2, grid.size() + 3};
    bool hit_bottom = false;

    while (!hit_bottom) {
      // move rock one position in jet direction
      int jet_dir = jets[jet_idx];
      if (can_move_horizontal(rock_position, vertices, jet_dir)) {
        rock_position.first += jet_dir;
      }

      // move rock one position down
      if (can_move_vertical(rock_position, vertices, -1)) {
        rock_position.second -= 1;
      } else {
        insert_shape(rock_position, vertices);
        hit_bottom = true;
      }

      jet_idx = (jet_idx + 1) % jets.size();
    }

    return jet_idx;
  };

  // (rock_idx, jet_idx, bitmask of top rows in grid)
  using state = tuple<int, int, grid_bitmask>;
  // (iter, total height)
  using result = tuple<uint64_t, uint64_t>;

  uint64_t number_of_rocks = 1'000'000'000'000ULL;

  uint64_t height = 0;
  uint64_t additional_height = 0;
  int rock_idx = 0;
  int jet_idx = 0;

  unordered_map<state, result> memory;

  for (uint64_t iter = 0; iter < number_of_rocks; iter++) {
    jet_idx = simulate_in_current_grid(rock_idx, jet_idx);
    rock_idx = (rock_idx + 1) % rocks.size();

    height = grid.size();

    if (height > BITMASK_ROWS) {
      grid_bitmask bitmask = build_bitmask_from_grid();
      // (rock_idx, jet_idx, bitmask of top rows in grid)
      state s{rock_idx, jet_idx, bitmask};

      auto memory_iter = memory.find(s);
      if (memory_iter != memory.end()) {
        auto [old_iter, old_height] = memory_iter->second;

        uint64_t delta_iter = iter - old_iter;
        uint64_t delta_height = height - old_height;

        uint64_t num_iterations = (number_of_rocks - iter) / delta_iter;

        additional_height += num_iterations * delta_height;
        iter += num_iterations * delta_iter;
      }

      memory[s] = {iter, height};
    }
  }

  cout << (height + additional_height) << endl;
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
