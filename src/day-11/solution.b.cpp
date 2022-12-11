#include "base.hpp"

struct item {
  uint64_t worry_level;
};

struct monkey {
  int idx;
  uint64_t inspect_count;

  vector<item> items;

  function<uint64_t(uint64_t)> operation;
  int divider;

  int true_monkey_index;
  int false_monkey_index;
};

static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
  return s;
}

int main() {
  vector<monkey> monkeys;

  while (1) {
    string line;
    getline(cin, line);

    if (!cin)
      break;

    line = ltrim(line);
    if (line.empty())
      continue;

    if (line.find("Monkey") == 0) {
      monkey m{};
      m.idx = monkeys.size();
      monkeys.push_back(m);
      continue;
    }

    monkey &current = *monkeys.rbegin();

    if (line.find("Starting items") == 0) {
      string data = line.substr(string("Starting items:").length());

      while (1) {
        int idx = data.find(',');
        int count = (idx < 0) ? data.size() : idx;

        string item = data.substr(0, count);
        uint64_t worry_level = stoi(item);
        current.items.push_back({worry_level});

        if (idx < 0)
          break;

        data = data.substr(idx + 1);
      }
    } else if (line.find("Operation") == 0) {
      char op;
      char val[5];
      sscanf(line.c_str(), "Operation: new = old %c %s", &op, val);

      if (op == '*' && string(val) == "old") {
        current.operation = [](uint64_t old) { return old * old; };
      } else {
        uint64_t value = atoi(val);

        if (op == '*') {
          current.operation = [=](uint64_t old) { return old * value; };
        } else if (op == '+') {
          current.operation = [=](uint64_t old) { return old + value; };
        }
      }
    } else if (line.find("Test") == 0) {
      int divider;
      sscanf(line.c_str(), "Test: divisible by %d", &divider);
      current.divider = divider;
    } else if (line.find("If true") == 0) {
      int idx;
      sscanf(line.c_str(), "If true: throw to monkey %d", &idx);
      current.true_monkey_index = idx;
    } else if (line.find("If false") == 0) {
      int idx;
      sscanf(line.c_str(), "If false: throw to monkey %d", &idx);
      current.false_monkey_index = idx;
    }
  }

  int rounds = 10000;

  uint64_t least_multiple = monkeys[0].divider;
  for (int i = 1; i < monkeys.size(); i++)
    least_multiple = lcm(least_multiple, monkeys[i].divider);

  for (int round = 0; round < rounds; round++) {
    for (auto &monkey : monkeys) {
      // run current round
      for (auto item : monkey.items) {
        // inspect item
        monkey.inspect_count++;

        // increase worry level
        uint64_t prev = item.worry_level;
        item.worry_level = monkey.operation(item.worry_level) % least_multiple;

        // test for action
        int next_idx = (item.worry_level % monkey.divider == 0)
                           ? monkey.true_monkey_index
                           : monkey.false_monkey_index;

        // throw item to next monkey
        monkeys[next_idx].items.push_back(item);
      }

      monkey.items.clear();
    }
  }

  for (const auto &monkey : monkeys) {
    cout << monkey.idx << " (" << monkey.inspect_count
         << "): " << monkey.items.size() << endl;
  }

  sort(monkeys.begin(), monkeys.end(), [](const monkey &a, const monkey &b) {
    return a.inspect_count > b.inspect_count;
  });

  uint64_t monkey_business =
      monkeys[0].inspect_count * monkeys[1].inspect_count;
  cout << monkey_business << endl;
  return 0;
}
