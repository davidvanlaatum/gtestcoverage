#include "CLIException.h"

using namespace testing::coverage::cli;

CLIException::CLIException( ExitCode nCode, std::string nMessage ) : code( nCode ), message( std::move( nMessage ) ) {}

ExitCode CLIException::getCode() const {
  return code;
}

const char *CLIException::what() const noexcept {
  return message.c_str();
}
