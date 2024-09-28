#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <vector>

using std::string;

const int _size = 32;
float memory[_size];
unsigned int cursor = 0;

float arg();
void move(unsigned int);
void write(float);
void skip();
void label(float);
void golabel(float);
float read();

std::map<float, unsigned int> labels;

std::vector<string> program;
string current = "";
unsigned int line = 0;
unsigned int character = 0;

class ParseError : public std::exception {
 private:
  std::string message;

 public:
  ParseError(const std::string &msg) : message(msg) {}

  virtual const char *what() const noexcept override {
    return message.c_str();
  }
};

std::map<string, std::function<void()>>
    commands = {
        // Main
        {":", []() {
           move(arg());
         }},
        {"=", []() {
           write(arg());
         }},

        // Base
        {"+", []() {
           write(read() + arg());
         }},
        {"-", []() {
           write(read() - arg());
         }},
        {"*", []() {
           write(read() * arg());
         }},
        {"/", []() {
           write(read() / arg());
         }},
        {"^", []() {
           write(std::pow(read(), arg()));
         }},
        {"%", []() {
           write(std::fmod(read(), arg()));
         }},

        // Math
        {"sin", []() {
           write(std::sin(read()));
         }},
        {"cos", []() {
           write(std::cos(read()));
         }},
        {"tan", []() {
           write(std::tan(read()));
         }},
        {"atan", []() {
           write(std::atan(read()));
         }},
        {"rand", []() {
           write(std::rand());
         }},

        // Condition
        {"lt", []() {
           if (read() >= arg()) skip();
         }},
        {"gt", []() {
           if (read() <= arg()) skip();
         }},
        {"lte", []() {
           if (read() > arg()) skip();
         }},
        {"gte", []() {
           if (read() < arg()) skip();
         }},
        {"eq", []() {
           if (read() != arg()) skip();
         }},

        // Processor
        {"(", []() {
           label(arg());
         }},
        {")", []() {
           golabel(arg());
         }},
        {"dump", []() {
           for (int i = 0; i < _size; i++) {
             std::cout << memory[i] << " ";
           }
           std::cout << std::endl;
         }},
        {"exit", []() {
           exit(0);
         }},
};

float read() {
  return memory[cursor];
}

float arg() {
  string parsed;

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

void run(const string &line) {
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

void printstate() {
  std::cout << "=" << read() << " (" << cursor << ")" << std::endl;
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

int main(int argc, char *args[]) {
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

    string _line;
    std::cout << "> ";
    getline(std::cin, _line, '\n');
    trim(_line);
    program.push_back(_line);
  }
}