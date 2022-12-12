#include "base.hpp"

#include <queue>
#include <utility>

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

struct node {
  pair<int, int> pos;
  vector<pair<int, int>> path;
};

int dist(const pair<int, int> &a, const pair<int, int> &b) {
  int dx = a.first - b.first;
  int dy = a.second - b.second;
  return dx * dx + dy * dy;
}

node shortest_path(const vector<int> &heights, int width, int height,
                   const pair<int, int> &pos, const pair<int, int> &target) {
  vector<pair<int, int>> neighbor_offsets = {
      {-1, 0},
      {1, 0},
      {0, -1},
      {0, 1},
  };

  auto comparator = [=](const node &a, const node &b) {
    int a_dist = dist(a.pos, target);
    int b_dist = dist(b.pos, target);

    if (a_dist > b_dist)
      return true;

    if (a_dist == b_dist)
      return a.path.size() > b.path.size();

    return false;
  };

  priority_queue<node, vector<node>, function<bool(const node &, const node &)>>
      queue(comparator);
  unordered_map<pair<int, int>, node, pair_hash> visited;

  queue.push({pos, {}});

  node shortest_path;
  while (!queue.empty()) {
    auto node = queue.top();
    queue.pop();

    auto pos = node.pos;
    int pos_index = pos.first + pos.second * width;

    auto iter = visited.find(pos);
    if (iter != visited.end()) {
      auto prev = iter->second;
      if (node.path.size() >= prev.path.size())
        continue;
    }

    visited[pos] = node;

    if (pos.first == target.first && pos.second == target.second) {
      shortest_path = node;
      break;
    }

    for (auto offset : neighbor_offsets) {
      pair<int, int> neighbor_pos = {pos.first + offset.first,
                                     pos.second + offset.second};

      if (neighbor_pos.first < 0 || neighbor_pos.first >= width ||
          neighbor_pos.second < 0 || neighbor_pos.second >= height)
        continue;

      int neighbor_index = neighbor_pos.first + neighbor_pos.second * width;

      int diff = heights[neighbor_index] - heights[pos_index];
      if (diff > 1)
        continue;

      struct node neighbor = {neighbor_pos, vector<pair<int, int>>(node.path)};
      neighbor.path.push_back(neighbor_pos);

      queue.push(neighbor);
    }
  }

  return shortest_path;
}

int main() {
  vector<int> heights;
  int width = 0, height = 0;

  vector<pair<int, int>> positions;
  pair<int, int> target;

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    width = max(width, (int)line.size());

    for (int i = 0; i < line.size(); i++) {
      char c = line[i];
      int elevation = 0;

      if (c == 'S')
        elevation = 0;
      else if (c == 'E') {
        target = {i, height};
        elevation = 'z' - 'a';
      } else
        elevation = c - 'a';

      if (elevation == 0)
        positions.push_back({i, height});

      heights.push_back(elevation);
    }

    height++;
  }

  vector<node> paths;
  for (auto pos : positions) {
    node path = shortest_path(heights, width, height, pos, target);
    if (path.pos != pair<int, int>())
      paths.push_back(path);
  }

  sort(paths.begin(), paths.end(), [](const auto &a, const auto &b) {
    return a.path.size() < b.path.size();
  });

  cout << paths[0].path.size() << endl;
  return 0;
}
