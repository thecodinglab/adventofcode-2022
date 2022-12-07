#include "base.hpp"

#include <sstream>

int main() {
  unordered_map<string, uint64_t> directory_sizes;
  vector<string> current_dir;

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    if (line[0] == '$') {
      string cmd = line.substr(2, 2);
      if (cmd == "cd") {
        string dir = line.substr(5);

        if (dir == "/")
          current_dir = vector<string>();
        else if (dir == "..")
          current_dir.erase(current_dir.end());
        else
          current_dir.push_back(dir);

      } else if (cmd == "ls") {
      }
    } else {
      auto idx = line.find(' ');
      string size_or_type = line.substr(0, idx);
      string name = line.substr(idx + 1);

      if (size_or_type == "dir")
        continue;

      for (int i = 0; i < current_dir.size(); i++) {
        ostringstream imploded;
        copy(current_dir.begin(), current_dir.end() - i,
             ostream_iterator<string>(imploded, "/"));

        uint64_t size = stoi(size_or_type);
        directory_sizes[imploded.str()] += size;
      }
    }
  }

  uint64_t total_sum = 0;
  for (auto entry : directory_sizes) {
    if (entry.second <= 100000)
      total_sum += entry.second;
  }

  cout << total_sum << endl;
  return 0;
}
