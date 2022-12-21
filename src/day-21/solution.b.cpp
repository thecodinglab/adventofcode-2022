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
  root.op = '=';

  monkey_t &human = monkeys["humn"];
  human.number = 0;
  human.has_number = true;

  auto build_eq = y_combinator([&](auto build_eq, const monkey_t &m) {
    if (m.has_number && m.number == 0) {
      cout << "x";
      return;
    }

    if (m.has_number) {
      cout << m.number;
      return;
    }

    const auto &a = monkeys[m.monkey_operand_a];
    const auto &b = monkeys[m.monkey_operand_b];

    cout << "(";
    build_eq(a);

    if (m.op == '=')
      cout << '-';
    else
      cout << m.op;

    build_eq(b);
    cout << ")";
  });

  build_eq(root);
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
