#ifndef MEMORY_H
#define MEMORY_H

const int _size = 32;
extern float memory[_size];
extern unsigned int cursor;

float read();
void write(float value);
void move(unsigned int c);

#endif  // MEMORY_H
