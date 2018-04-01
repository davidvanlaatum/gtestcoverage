#pragma once
#ifndef GTESTCOVERAGE_SOURCEFILE_H
#define GTESTCOVERAGE_SOURCEFILE_H

#include "fwd.h"
#include "Point.h"
#include <vector>
#include <map>

namespace testing {
  namespace coverage {
    class SourceFile {
    public:
      SourceFile( const path &filename );

      bool lineHasCode( uint32_t line, Point start, Point end ) const;
    protected:
      std::vector<char> contents;
      std::map<uint32_t, size_t> index;
    };
  }
}

#endif //GTESTCOVERAGE_SOURCEFILE_H
