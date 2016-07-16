#include <iostream>
#include "Tetris.hh"

int main() {
  try {
    Tetris tetris;
    tetris.run();
  }
  catch (const char *ex) {
    std::cout << "Error while initialising tetris: " << ex << std::endl;
    return -1;
  }
  return 0;
}
