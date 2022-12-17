#include "base.hpp"
#include <array>

void run(istream &stream) {
  vector<vector<pair<int, int>>> rocks = {
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

  int number_of_rocks = 2022;
  int rock_idx = 0;
  int jet_idx = 0;

  constexpr int WIDTH = 7;
  constexpr char CHAR_BLOCKED = '#';
  vector<array<char, WIDTH>> grid;

  auto in_horizontal_bounds = [&](const pair<int, int> &pos) {
    return pos.first >= 0 && pos.first < WIDTH;
  };

  auto collides_in_grid = [&](const pair<int, int> &pos) {
    if (pos.second >= (int)grid.size())
      return false;

    if (pos.second < 0)
      return true;

    return grid[pos.second][pos.first] == CHAR_BLOCKED;
  };

  auto can_move_horizontal = [&](const pair<int, int> &offset,
                                 const vector<pair<int, int>> &vertices,
                                 int amount) {
    for (const auto &vertex : vertices) {
      pair<int, int> next{
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

  auto can_move_vertical = [&](const pair<int, int> &offset,
                               const vector<pair<int, int>> &vertices,
                               int amount) {
    for (const auto &vertex : vertices) {
      pair<int, int> next{
          offset.first + vertex.first,
          offset.second + vertex.second + amount,
      };

      if (collides_in_grid(next))
        return false;
    }

    return true;
  };

  auto insert_vertex = [&](const pair<int, int> &offset,
                           const pair<int, int> &vertex) {
    pair<int, int> pos{
        offset.first + vertex.first,
        offset.second + vertex.second,
    };

    while (pos.second >= grid.size()) {
      array<char, WIDTH> line;
      std::fill(line.begin(), line.end(), ' ');
      grid.push_back(line);
    }

    grid[pos.second][pos.first] = CHAR_BLOCKED;
  };

  auto insert_shape = [&](const pair<int, int> &offset,
                          const vector<pair<int, int>> &vertices) {
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

  for (int i = 0; i < number_of_rocks; i++) {
    const vector<pair<int, int>> &vertices = rocks[rock_idx];
    pair<int, int> rock_position = {WIDTH / 2, grid.size() + 3};
    bool hit_bottom = false;

    while (!hit_bottom) {
      // move rock one position in jet direction
      int jet_dir = jets[jet_idx];
      if (can_move_horizontal(rock_position, vertices, jet_dir)) {
        rock_position.first += jet_dir;
        // cout << "PUSH " << jet_dir << ": " << rock_position << endl;
      }

      // move rock one position down
      if (can_move_vertical(rock_position, vertices, -1)) {
        rock_position.second -= 1;
        // cout << "DOWN: " << rock_position << endl;
      } else {
        insert_shape(rock_position, vertices);
        hit_bottom = true;

        // cout << "HIT BOTTOM: " << rock_position << endl;
      }

      jet_idx = (jet_idx + 1) % jets.size();
    }

    rock_idx = (rock_idx + 1) % rocks.size();
  }

  // print_grid();

  cout << grid.size() << endl;
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
