#include "parser.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>

#include "commands.h"
#include "errors.h"
#include "memory.h"

std::vector<std::string> program;
std::string current = "";
unsigned int line = 0;
unsigned int character = 0;
std::map<float, unsigned int> labels;

float arg() {
  std::string parsed;

  for (int i = character; i < current.size(); i++) {
    if (current[i] == ' ' || current[i] == '\t') {
      if (parsed.size() > 0)
        break;
      continue;
    }

    parsed += current[i];
  }

  if (parsed.size() == 0)
    throw ParseError("Can not parse argument");

  try {
    switch (parsed[0]) {
      case 'm':
      case 'M': {
        unsigned int c = std::stoi(parsed.substr(1));

        if (c < 0 || c > _size - 1)
          throw ParseError("Out of range memory");

        return memory[c];
      }

      case '0': {
        if (parsed.size() > 1) {
          switch (parsed[1]) {
            case 'x':
            case 'X': {
              if (parsed.size() > 2)
                return std::stoi(parsed.substr(2), nullptr, 16);
            }

            case 'b':
            case 'B': {
              if (parsed.size() > 2)
                return std::stoi(parsed.substr(2), nullptr, 2);
            }

            default: {
              return std::stoi(parsed.substr(1), nullptr, 8);
            }
          }
        }
      }

      default: {
        return std::stof(parsed);
      }
    }
  } catch (std::invalid_argument) {
    throw ParseError("Failed parse argument");
  } catch (...) {
    throw;
  }
}

void run(const std::string &line) {
  for (auto it = commands.begin(); it != commands.end(); ++it) {
    auto cmd = it->first;
    auto _size = cmd.size();
    if (line.substr(0, _size) == cmd) {
      character = _size;
      it->second();
      return;
    }
  }

  std::cout << "Error: No found command " << line << std::endl;
}

void skip() {
  line++;
}

void label(float anchor) {
  labels.insert({anchor, line});
}

void golabel(float anchor) {
  line = labels.at(anchor);
}
