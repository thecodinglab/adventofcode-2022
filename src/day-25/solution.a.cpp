#include "base.hpp"

int64_t from_base_5(const string &str) {
  int64_t val = 0;

  for (const char c : str) {
    val *= 5;

    switch (c) {
    case '2':
      val += 2;
      break;

    case '1':
      val += 1;
      break;

    case '0':
      val += 0;
      break;

    case '-':
      val -= 1;
      break;

    case '=':
      val -= 2;
      break;

    default:
      cout << "INVALID VALUE: " << c << endl;
      continue;
    }
  }

  return val;
}

string to_base_5(int64_t value) {
  ostringstream o;

  int64_t carry = 0;

  while (value) {
    int64_t digit = value % 5 + carry;
    // cout << (value % 5) << " " << digit << " " << o.str() << endl;

    if (digit <= 2) {
      char c = ('0' + digit);
      o << c;
      carry = 0;
    } else if (digit == 3) {
      o << '=';
      carry = 1;
    } else if (digit == 4) {
      o << '-';
      carry = 1;
    } else if (digit == 5) {
      o << '0';
      carry = 1;
    }

    value = value / 5;
  }

  string res = o.str();
  reverse(res.begin(), res.end());
  return res;
}

void run(istream &stream) {
  int64_t sum = 0;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    int64_t val = from_base_5(line);

    sum += val;
    // cout << line << " " << val << endl;
  }

  cout << to_base_5(sum) << endl;
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
