#include <algorithm>
#include <fstream>
#include <iostream>

#include "commands.h"
#include "errors.h"
#include "memory.h"
#include "parser.h"

void printstate() {
  std::cout << "=" << read() << " (" << cursor << ")" << std::endl;
}

static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
          }).base(),
          s.end());
}

static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

int main(int argc, char *args[]) {
  initializeCommands();

  if (argc > 1) {
    std::ifstream inputFile(args[1]);

    if (!inputFile) {
      std::cerr << "Unable to open file" << std::endl;
      return 1;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
      program.push_back(line);
    }

    inputFile.close();
  }

  printstate();

  while (true) {
    if (program.size() > line) {
      character = 0;
      try {
        current = program.at(line++);
        if (current.size() == 0) {
          continue;
        }
        if (current[0] == '#') {
          continue;
        }
        run(current);
      } catch (ParseError &e) {
        std::cout << "Error: " << e.what() << std::endl;
      } catch (...) {
        throw;
      }
      printstate();
      continue;
    }

    std::string _line;
    std::cout << "> ";
    getline(std::cin, _line, '\n');
    trim(_line);
    program.push_back(_line);
  }
}
