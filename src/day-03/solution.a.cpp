#include "base.hpp"

int main() {
  int total = 0;

  while (1) {
    string content;
    cin >> content;

    if (!cin)
      break;

    string compartment_a = content.substr(0, content.length() / 2);
    string compartment_b = content.substr(content.length() / 2);

    for (int i = 0; i < compartment_a.length(); i++) {
      char a = compartment_a[i];
      bool found = false;

      for (int j = 0; j < compartment_a.length(); j++) {
        char b = compartment_b[j];

        if (a == b) {
          int val = 0;

          if (a >= 'a')
            val = (a - 'a') + 1;
          else
            val = (a - 'A') + 27;

          cout << "found " << a << " " << b << " " << val << endl;
          total += val;

          found = true;
          break;
        }
      }

      if (found)
        break;
    }
  }

  cout << total << endl;
  return 0;
}
