#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

extern std::vector<std::string> program;
extern std::string current;
extern unsigned int line;
extern unsigned int character;

float arg();
void run(const std::string &line);
void skip();
void label(float anchor);
void golabel(float anchor);

#endif  // PARSER_H
