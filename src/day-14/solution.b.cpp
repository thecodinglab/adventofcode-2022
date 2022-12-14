#include "base.hpp"

#define EMPTY ' '
#define ROCK '#'
#define SAND 'o'

int sign(int x) { return (x == 0) ? 0 : (x < 0) ? -1 : 1; }

int parse_int(string &str) {
  int i = 0;
  char c = str[i];

  string integer;
  while (c >= '0' && c <= '9') {
    integer += c;
    c = str[++i];
  }

  str = str.substr(i);

  return stoi(integer);
}

pair<int, int> parse_coordinate(string &str) {
  int x = parse_int(str);
  str = str.substr(1); // skip ,
  int y = parse_int(str);

  if (!str.empty())
    str = str.substr(4); // skip ->

  return {x, y};
}

void run(istream &stream) {
  int width = 1000, height = 200;
  vector<char> grid(width * height, EMPTY);

  int max_y = 0;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    auto last = parse_coordinate(line);
    max_y = max(max_y, last.second);
    while (!line.empty()) {
      auto current = parse_coordinate(line);
      max_y = max(max_y, current.second);

      pair<int, int> diff = {
          current.first - last.first,
          current.second - last.second,
      };

      pair<int, int> dir = {
          sign(diff.first),
          sign(diff.second),
      };

      auto pos = last;
      while (pos != current) {
        int idx = pos.second * width + pos.first;
        grid[idx] = ROCK;

        pos.first += dir.first;
        pos.second += dir.second;
      }

      int idx = pos.second * width + pos.first;
      grid[idx] = ROCK;

      last = current;
    }
  }

  max_y += 2;

  for (int x = 0; x < width; x++) {
    int idx = max_y * width + x;
    grid[idx] = ROCK;
  }

  int count = 0;

  while (1) {
    pair<int, int> sand_pos = {500, 0};

    while (sand_pos.second < height) {
      if (sand_pos.second + 1 >= height)
        break;

      int sand_idx = (sand_pos.second + 1) * width + sand_pos.first;
      if (grid[sand_idx] == EMPTY) {
        // found position below
        sand_pos.second++;
      } else {
        if (grid[sand_idx - 1] == EMPTY) {
          // found position diagonal left
          sand_pos.first--;
          sand_pos.second++;
        } else if (grid[sand_idx + 1] == EMPTY) {
          // found position diagonal right
          sand_pos.first++;
          sand_pos.second++;
        } else {
          // sand is blocked
          break;
        }
      }
    }

    int sand_idx = sand_pos.second * width + sand_pos.first;
    grid[sand_idx] = SAND;

    count++;

    if (sand_pos == pair<int, int>{500, 0})
      break;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int idx = y * width + x;
      cout << grid[idx];
    }

    cout << endl;
  }

  cout << count << endl;
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
