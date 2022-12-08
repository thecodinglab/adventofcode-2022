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

  int visible = 0;

  for (int i = 0; i < tree_heights.size(); i++) {
    int row = i / width;
    int col = i % width;

    int max_height = 0;

    // left
    max_height = -1;
    for (int x = 0; x < col; x++)
      max_height = max(max_height, tree_heights[row * width + x]);

    if (tree_heights[i] > max_height) {
      cout << i << ": visible left" << endl;
      visible++;
      continue;
    }

    // right
    max_height = -1;
    for (int x = col + 1; x < width; x++)
      max_height = max(max_height, tree_heights[row * width + x]);

    if (tree_heights[i] > max_height) {
      cout << i << ": visible right" << endl;
      visible++;
      continue;
    }

    // up
    max_height = -1;
    for (int y = 0; y < row; y++)
      max_height = max(max_height, tree_heights[y * width + col]);

    if (tree_heights[i] > max_height) {
      cout << i << ": visible up" << endl;
      visible++;
      continue;
    }

    // down
    max_height = -1;
    for (int y = row + 1; y < height; y++)
      max_height = max(max_height, tree_heights[y * width + col]);

    if (tree_heights[i] > max_height) {
      cout << i << ": visible down" << endl;
      visible++;
      continue;
    }
  }

  cout << visible << endl;
  return 0;
}
