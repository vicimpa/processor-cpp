#include "commands.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "memory.h"
#include "parser.h"

std::map<std::string, std::function<void()>> commands;

void initializeCommands() {
  commands = {
      // Main
      {":", []() { move(arg()); }},
      {"=", []() { write(arg()); }},

      // Base
      {"+", []() { write(read() + arg()); }},
      {"-", []() { write(read() - arg()); }},
      {"*", []() { write(read() * arg()); }},
      {"/", []() { write(read() / arg()); }},
      {"&", []() { write((int)read() & (int)arg()); }},
      {"|", []() { write((int)read() | (int)arg()); }},
      {"~", []() { write(~(int)read()); }},
      {"<", []() { write((int)read() << (int)arg()); }},
      {">", []() { write((int)read() >> (int)arg()); }},
      {"^", []() { write(std::pow(read(), arg())); }},
      {"%", []() { write(std::fmod(read(), arg())); }},

      // Math
      {"sin", []() { write(std::sin(read())); }},
      {"cos", []() { write(std::cos(read())); }},
      {"tan", []() { write(std::tan(read())); }},
      {"atan", []() { write(std::atan(read())); }},
      {"rand", []() { write(std::rand()); }},
      {"round", []() { write(std::round(read())); }},
      {"floor", []() { write(std::floor(read())); }},
      {"ceil", []() { write(std::ceil(read())); }},

      // Condition
      {"lt", []() { if (read() >= arg()) skip(); }},
      {"gt", []() { if (read() <= arg()) skip(); }},
      {"lte", []() { if (read() > arg()) skip(); }},
      {"gte", []() { if (read() < arg()) skip(); }},
      {"eq", []() { if (read() != arg()) skip(); }},
      {"neq", []() { if (read() == arg()) skip(); }},

      // Processor
      {"(", []() { label(arg()); }},
      {")", []() { golabel(arg()); }},
      {"dump", []() {
         for (int i = 0; i < _size; i++)
           std::cout << memory[i] << " ";
         std::cout << std::endl;
       }},
      {"exit", []() { exit(0); }},
  };
}
