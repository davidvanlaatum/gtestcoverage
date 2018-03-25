#include "CLIException.h"

using namespace testing::coverage::cli;

CLIException::CLIException( ExitCode nCode, const std::string &nMessage ) : code( nCode ), message( nMessage ) {}

ExitCode CLIException::getCode() const {
  return code;
}

const char *CLIException::what() const noexcept {
  return message.c_str();
}
