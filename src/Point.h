#pragma once
#ifndef GTESTCOVERAGE_POINT_H
#define GTESTCOVERAGE_POINT_H

#include "fwd.h"
#include <json.hpp>
#include <ostream>              // for operator<<, ostream, basic_ostream, basic_ostream<>::__ostream_type
#include <cstdint>

namespace testing {
  namespace coverage {
    struct Point {
      inline Point();
      inline Point( uint32_t nLine, uint32_t nColumn );
      inline bool operator==( const Point &rhs ) const;
      inline bool operator!=( const Point &rhs ) const;
      inline bool operator<( const Point &rhs ) const;
      inline bool operator>( const Point &rhs ) const;
      inline bool operator<=( const Point &rhs ) const;
      inline bool operator>=( const Point &rhs ) const;
      uint32_t line = 0;
      uint32_t column = 0;
    };

    Point::Point() = default;

    Point::Point( uint32_t nLine, uint32_t nColumn ) : line( nLine ), column( nColumn ) {}

    inline std::ostream &operator<<( std::ostream &os, const Point &point ) {
      os << point.line << ":" << point.column;
      return os;
    }

    bool Point::operator==( const Point &rhs ) const {
      return line == rhs.line && column == rhs.column;
    }

    bool Point::operator!=( const Point &rhs ) const {
      return !( rhs == *this );
    }

    bool Point::operator<( const Point &rhs ) const {
      if ( line < rhs.line ) {
        return true;
      }
      if ( rhs.line < line ) {
        return false;
      }
      return column < rhs.column;
    }

    bool Point::operator>( const Point &rhs ) const {
      return rhs < *this;
    }

    bool Point::operator<=( const Point &rhs ) const {
      return !( rhs < *this );
    }

    bool Point::operator>=( const Point &rhs ) const {
      return !( *this < rhs );
    }

    inline void to_json( nlohmann::json &json, const Point &point ) {
      json["line"] = point.line;
      json["column"] = point.column;
    }

    inline void from_json( const nlohmann::json &json, Point &point ) {
      point.line = json["line"];
      point.column = json["column"];
    }
  }
}

#endif //GTESTCOVERAGE_POINT_H
