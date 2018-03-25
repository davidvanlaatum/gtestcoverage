#pragma once
#ifndef GTESTCOVERAGE_CLIEXCEPTION_H
#define GTESTCOVERAGE_CLIEXCEPTION_H

#include <stdexcept>
#include <sysexits.h>
#include <string>

namespace testing {
  namespace coverage {
    namespace cli {
      enum ExitCode {
        OK = 0,
        Failure,
        NoInput = EX_NOINPUT,
        Usage = EX_USAGE
      };

      class CLIException : public std::exception {
      public:
        CLIException( ExitCode nCode, std::string nMessage );
        ExitCode getCode() const;
        const char *what() const noexcept override;
      protected:
        ExitCode code;
        std::string message;
      };
    }
  }
}

#endif //GTESTCOVERAGE_CLIEXCEPTION_H
