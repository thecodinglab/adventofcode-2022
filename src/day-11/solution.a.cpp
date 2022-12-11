#include "base.hpp"

struct item {
  int worry_level;
};

struct monkey {
  int idx;
  int inspect_count;

  vector<item> items;

  function<int(int)> operation;
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
        int worry_level = stoi(item);
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
        current.operation = [](int old) { return old * old; };
      } else {
        int value = atoi(val);

        if (op == '*') {
          current.operation = [=](int old) { return old * value; };
        } else if (op == '+') {
          current.operation = [=](int old) { return old + value; };
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

  int rounds = 20;
  // int rounds = 1;

  for (int round = 0; round < rounds; round++) {
    for (auto &monkey : monkeys) {
      // run current round
      for (auto item : monkey.items) {
        // inspect item
        monkey.inspect_count++;

        // increase worry level
        item.worry_level = monkey.operation(item.worry_level);
        item.worry_level =
            item.worry_level / 3; // TODO this might not round down correctly

        // test for action
        int next_idx = (item.worry_level % monkey.divider == 0)
                           ? monkey.true_monkey_index
                           : monkey.false_monkey_index;

        // throw item to next monkey
        monkeys[next_idx].items.push_back(item);

        cout << "throw " << item.worry_level << " to " << next_idx << endl;
      }

      monkey.items.clear();
    }

    for (int i = 0; i < monkeys.size(); i++) {
      cout << i << ": " << monkeys[i].items.size() << endl;
    }
  }

  sort(monkeys.begin(), monkeys.end(), [](const monkey &a, const monkey &b) {
    return a.inspect_count > b.inspect_count;
  });

  for (const auto &monkey : monkeys) {
    cout << monkey.idx << " (" << monkey.inspect_count
         << "): " << monkey.items.size() << endl;
  }

  int monkey_business = monkeys[0].inspect_count * monkeys[1].inspect_count;
  cout << monkey_business << endl;
  return 0;
}
