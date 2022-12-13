#include "base.hpp"

void run(istream &stream) {
  while (1) {
    // TODO read input line
    if (!stream)
      break;

    // TODO do calculation
  }

  // TODO output calculation
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
