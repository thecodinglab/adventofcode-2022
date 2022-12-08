#include "base.hpp"

int main() {
  vector<int> tree_heights;
  int width = 0;
  int height = 0;

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    width = max(width, (int)line.size());
    height++;

    for (char c : line)
      tree_heights.push_back(c - '0');
  }

  int max_score = 0;

  for (int i = 0; i < tree_heights.size(); i++) {
    int row = i / width;
    int col = i % width;

    if (row == 0 || row == height - 1 || col == 0 || col == width - 1)
      continue;

    int a = 0, b = 0, c = 0, d = 0;

    // left
    for (int x = col - 1; x >= 0; x--) {
      a++;
      if (tree_heights[row * width + x] >= tree_heights[i])
        break;
    }

    // right
    for (int x = col + 1; x < width; x++) {
      b++;
      if (tree_heights[row * width + x] >= tree_heights[i])
        break;
    }

    // up
    for (int y = row - 1; y >= 0; y--) {
      c++;
      if (tree_heights[y * width + col] >= tree_heights[i])
        break;
    }

    // down
    for (int y = row + 1; y < height; y++) {
      d++;
      if (tree_heights[y * width + col] >= tree_heights[i])
        break;
    }

    int score = a * b * c * d;
    max_score = max(max_score, score);
  }

  cout << max_score << endl;
  return 0;
}
