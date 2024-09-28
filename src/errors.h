#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <string>

class ParseError : public std::exception {
 private:
  std::string message;

 public:
  ParseError(const std::string &msg);
  virtual const char *what() const noexcept override;
};

#endif  // ERRORS_H
