#include "errors.h"

ParseError::ParseError(const std::string &msg) : message(msg) {}

const char *ParseError::what() const noexcept {
  return message.c_str();
}
