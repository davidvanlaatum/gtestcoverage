#pragma once
#ifndef GTESTCOVERAGE_CLANGCOVERAGEEXCEPTION_H
#define GTESTCOVERAGE_CLANGCOVERAGEEXCEPTION_H
#include <stdexcept>

namespace testing {
  namespace coverage {
    namespace clang {
      class CLangCoverageException : public std::runtime_error {
      public:
        explicit CLangCoverageException( const std::string &msg );
        explicit CLangCoverageException( const char *msg );
      };
    }
  }
}

#endif //GTESTCOVERAGE_CLANGCOVERAGEEXCEPTION_H
