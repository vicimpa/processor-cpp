#include "memory.h"

#include <iostream>

float memory[_size];
unsigned int cursor = 0;

float read() {
  return memory[cursor];
}

void write(float value) {
  memory[cursor] = value;
}

void move(unsigned int c) {
  if (c < 0 || c > _size - 1) {
    std::cout << "Error: Out of range cursor 0.." << (_size - 1) << std::endl;
    return;
  }
  cursor = c;
}
