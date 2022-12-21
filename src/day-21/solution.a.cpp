#include "base.hpp"

struct monkey_t {
  int64_t number;
  bool has_number;

  string monkey_operand_a;
  string monkey_operand_b;
  char op;
};

void run(istream &stream) {
  unordered_map<string, monkey_t> monkeys;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    auto idx = line.find(':');
    string name = line.substr(0, idx);
    string operation = line.substr(idx + 2);

    monkey_t monkey = {};

    int64_t number = atol(operation.c_str());
    if (number == 0) {
      monkey.monkey_operand_a = operation.substr(0, 4);
      monkey.monkey_operand_b = operation.substr(7, 4);
      monkey.op = operation[5];
    } else {
      monkey.number = number;
      monkey.has_number = true;
    }

    monkeys[name] = monkey;
  }

  monkey_t &root = monkeys["root"];

  while (!root.has_number) {
    for (auto &[name, monkey] : monkeys) {
      if (monkey.has_number)
        continue;

      const auto &a = monkeys[monkey.monkey_operand_a];
      const auto &b = monkeys[monkey.monkey_operand_b];

      if (!a.has_number || !b.has_number)
        continue;

      switch (monkey.op) {
      case '+': {
        monkey.number = a.number + b.number;
        monkey.has_number = true;
      } break;

      case '-': {
        monkey.number = a.number - b.number;
        monkey.has_number = true;
      } break;

      case '*': {
        monkey.number = a.number * b.number;
        monkey.has_number = true;
      } break;

      case '/': {
        monkey.number = a.number / b.number;
        monkey.has_number = true;
      } break;
      }
    }
  }

  cout << root.number << endl;
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
