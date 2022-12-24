#include "base.hpp"

using vec2_t = pair<int64_t, int64_t>;

bool dist(const vec2_t &a, const vec2_t &b) {
  int64_t dx = a.first - b.first;
  int64_t dy = a.second - b.second;
  return dx * dx + dy * dy;
}

enum direction_t { UP, DOWN, LEFT, RIGHT };

struct blizzard_t {
  vec2_t pos;
  direction_t dir;
};

void run(istream &stream) {
  int64_t width = 0, height = 0;
  vector<blizzard_t> blizzards;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    width = max(width, (int64_t)line.size());

    for (int64_t x = 0; x < line.size(); x++) {
      char c = line[x];

      vec2_t pos = {x, height};
      direction_t dir;

      switch (c) {
      case '^':
        dir = UP;
        break;

      case 'v':
        dir = DOWN;
        break;

      case '<':
        dir = LEFT;
        break;

      case '>':
        dir = RIGHT;
        break;

      default:
        continue;
      }

      blizzards.push_back({pos, dir});
    }

    height++;
  }

  unordered_map<direction_t, vec2_t> offsets = {
      {UP, {0, -1}},
      {DOWN, {0, +1}},
      {LEFT, {-1, 0}},
      {RIGHT, {+1, 0}},
  };

  auto calculate_blizzard_locations =
      [&offsets, width, height](const vector<blizzard_t> &blizzards,
                                int64_t steps) -> set<vec2_t> {
    set<vec2_t> locations;

    for (const auto &blizzard : blizzards) {
      vec2_t off = offsets[blizzard.dir];

      vec2_t pos = {
          1 + mod(blizzard.pos.first + steps * off.first - 1, width - 2),
          1 + mod(blizzard.pos.second + steps * off.second - 1, height - 2),
      };

      locations.insert(pos);
    }

    return locations;
  };

  vec2_t starting_pos = {1, 0};
  vec2_t ending_pos = {width - 2, height - 1};

  auto in_bounds = [&starting_pos, &ending_pos, width,
                    height](const vec2_t &pos) -> bool {
    if (pos == starting_pos || pos == ending_pos)
      return true;

    vec2_t rel = {
        pos.first - 1,
        pos.second - 1,
    };

    return rel.first >= 0 && rel.first < (width - 2) && rel.second >= 0 &&
           rel.second < (height - 2);
  };

  // (position, number of steps)
  using state_t = tuple<vec2_t, int64_t>;
  constexpr int POSITION = 0;
  constexpr int STEPS = 1;

  auto path_find = [&](const state_t &from, const vec2_t &dest) -> int64_t {
    auto comparator = [&](const state_t &a, const state_t &b) {
      auto a_steps = get<STEPS>(a);
      auto b_steps = get<STEPS>(b);

      if (a_steps == b_steps) {
        auto a_dist = dist(get<POSITION>(a), dest);
        auto b_dist = dist(get<POSITION>(b), dest);

        return a_dist > b_dist;
      }

      return a_steps > b_steps;
    };

    priority_queue<state_t, vector<state_t>,
                   function<bool(const state_t &, const state_t &)>>
        queue(comparator);

    queue.push(from);

    unordered_set<state_t> visited;

    while (!queue.empty()) {
      auto curr = queue.top();
      queue.pop();

      const auto &curr_pos = get<POSITION>(curr);
      const auto &curr_steps = get<STEPS>(curr);

      // cout << curr_pos << " " << curr_steps << ": " << dist(curr_pos,
      // ending_pos)
      //      << endl;

      if (curr_pos == dest) {
        // fastest path found
        return curr_steps;
      }

      if (visited.contains(curr))
        continue;

      visited.insert(curr);

      auto next_blizzard_locations =
          calculate_blizzard_locations(blizzards, curr_steps + 1);

      if (!next_blizzard_locations.contains(curr_pos)) {
        // wait
        queue.push({curr_pos, curr_steps + 1});
      }

      for (const auto &[_, off] : offsets) {
        vec2_t pos = {
            curr_pos.first + off.first,
            curr_pos.second + off.second,
        };

        if (!in_bounds(pos))
          continue;

        if (next_blizzard_locations.contains(pos))
          continue;

        queue.push({pos, curr_steps + 1});
      }
    }

    return -1;
  };

  // int64_t steps = path_find({starting_pos, 0}, ending_pos);
  // cout << "start -> end: " << steps << endl;

  int64_t steps = 260; // from part 1 calculation

  steps = path_find({ending_pos, steps}, starting_pos);
  cout << "end -> start: " << steps << endl;

  steps = path_find({starting_pos, steps}, ending_pos);
  cout << "start -> end: " << steps << endl;

  cout << steps << endl;
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
