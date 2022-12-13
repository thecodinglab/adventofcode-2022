#include "base.hpp"

#include <cassert>
#include <memory>
#include <stack>
#include <vector>

struct node {
  bool list;

  int value;
  vector<shared_ptr<node>> children;
};

shared_ptr<node> read() {
  string line;
  getline(cin, line);

  cout << "read line " << line << endl;

  if (!cin)
    return nullptr;

  stack<shared_ptr<node>> nodes;
  shared_ptr<node> current = nullptr;

  for (int i = 0; i < line.size(); i++) {
    char c = line[i];

    if (c >= '0' && c <= '9') {
      string integer;

      while (c >= '0' && c <= '9') {
        integer += c;
        c = line[++i];
      }

      i--;

      auto value = make_shared<node>();
      value->list = false;
      value->value = stoi(integer);
      current->children.push_back(value);
    } else if (c == ',') {
      // next value
    } else if (c == '[') {
      cout << "push list " << i << endl;
      nodes.push(current);

      shared_ptr<node> next = make_shared<node>();
      next->list = true;

      if (current) {
        current->children.push_back(next);
      }

      current = next;
    } else if (c == ']') {
      cout << "pop list " << i << endl;
      auto top = nodes.top();
      nodes.pop();

      if (top)
        current = top;
    }
  }

  assert(nodes.empty());

  return current;
}

int compare(const shared_ptr<node> &a, const shared_ptr<node> &b) {
  if (!a->list && !b->list) {
    // both values are integers
    cout << "compare " << a->value << " " << b->value << endl;
    return b->value - a->value;
  }

  vector<shared_ptr<node>> a_list = a->children;
  vector<shared_ptr<node>> b_list = b->children;

  if (!a->list) {
    cout << "convert " << a->value << " (a) to list" << endl;
    a_list = vector<shared_ptr<node>>{a};
  }

  if (!b->list) {
    cout << "convert " << a->value << " (b) to list" << endl;
    b_list = vector<shared_ptr<node>>{b};
  }

  cout << "check " << a_list << " " << b_list << endl;

  int size = min(a_list.size(), b_list.size());
  int cmp;

  for (int i = 0; i < size; i++) {
    cmp = compare(a_list[i], b_list[i]);
    if (cmp != 0)
      return cmp;
  }

  cout << "size: " << a_list.size() << " " << b_list.size() << endl;

  if (a_list.size() == b_list.size())
    return 0;

  if (a_list.size() < b_list.size())
    return 1;

  return -1;
}

int main() {
  int idx = 0;
  int sum = 0;

  while (1) {
    auto left = read();
    auto right = read();

    string line;
    getline(cin, line);

    if (!left || !right) {
      cout << left << " " << right << endl;
      break;
    }

    idx++;

    int cmp = compare(left, right);
    cout << idx << " " << cmp << endl << endl;

    if (cmp >= 0) {
      sum += idx;
    }
  }

  cout << sum << endl;
  return 0;
}
