#ifndef COMMANDS_H
#define COMMANDS_H

#include <functional>
#include <map>
#include <string>

extern std::map<std::string, std::function<void()>> commands;

void initializeCommands();

#endif  // COMMANDS_H
