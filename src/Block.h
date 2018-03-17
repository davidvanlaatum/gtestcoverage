#pragma once
#ifndef GTESTCOVERAGE_BLOCK_H
#define GTESTCOVERAGE_BLOCK_H

#include "Point.h"                    // for Point, operator<<
#include "fwd.h"                      // for path
#include <boost/filesystem/path.hpp>  // for operator<, operator<<, operator==
#include <cstdint>                    // for uint32_t
#include <json.hpp>                   // for json, basic_json<>::value_type
#include <ostream>                    // for ostream, operator<<, basic_ostream, char_traits

namespace testing {
  namespace coverage {
    struct Block {
      inline bool operator==( const Block &rhs ) const;
      inline bool operator!=( const Block &rhs ) const;
      inline bool operator<( const Block &rhs ) const;
      inline bool operator>( const Block &rhs ) const;
      inline bool operator<=( const Block &rhs ) const;
      inline bool operator>=( const Block &rhs ) const;
      inline Block();
      inline Block( const Point &nStart, const Point &nEnd );
      inline bool contains( const Block &other ) const;
      Point start, end;
    };

    struct BlockWithFilename : public Block {
      inline BlockWithFilename();
      inline BlockWithFilename( const Point &nStart, const Point &nEnd );
      path file;
      inline bool operator==( const BlockWithFilename &rhs ) const;
      inline bool operator!=( const BlockWithFilename &rhs ) const;
      inline bool operator<( const BlockWithFilename &rhs ) const;
      inline bool operator>( const BlockWithFilename &rhs ) const;
      inline bool operator<=( const BlockWithFilename &rhs ) const;
      inline bool operator>=( const BlockWithFilename &rhs ) const;
    };

    Block::Block() = default;

    Block::Block( const Point &nStart, const Point &nEnd ) : start( nStart ), end( nEnd ) {}

    bool testing::coverage::Block::operator==( const testing::coverage::Block &rhs ) const {
      return start == rhs.start &&
             end == rhs.end;
    }

    bool testing::coverage::Block::operator!=( const testing::coverage::Block &rhs ) const {
      return !( rhs == *this );
    }

    bool Block::operator<( const Block &rhs ) const {
      if ( start < rhs.start ) {
        return true;
      }
      if ( rhs.start < start ) {
        return false;
      }
      return end < rhs.end;
    }

    bool Block::operator>( const Block &rhs ) const {
      return rhs < *this;
    }

    bool Block::operator<=( const Block &rhs ) const {
      return !( rhs < *this );
    }

    bool Block::operator>=( const Block &rhs ) const {
      return !( *this < rhs );
    }

    inline void to_json( nlohmann::json &json, const Block &block ) {
      json["startLine"] = block.start.line;
      json["startColumn"] = block.start.column;
      json["endLine"] = block.end.line;
      json["endColumn"] = block.end.column;
    }

    bool Block::contains( const Block &other ) const {
      bool rt = false;

      if ( start <= other.start && end >= other.end ) {
        rt = true;
      }
      return rt;
    }

    inline std::ostream &operator<<( std::ostream &os, const Block &block ) {
      os << block.start << "-" << block.end;
      return os;
    }

    BlockWithFilename::BlockWithFilename() = default;

    BlockWithFilename::BlockWithFilename( const Point &nStart, const Point &nEnd ) : Block( nStart, nEnd ) {}

    bool BlockWithFilename::operator==( const BlockWithFilename &rhs ) const {
      return static_cast<const testing::coverage::Block &>(*this) == static_cast<const testing::coverage::Block &>(rhs) &&
             file == rhs.file;
    }

    bool BlockWithFilename::operator!=( const BlockWithFilename &rhs ) const {
      return !( rhs == *this );
    }

    bool BlockWithFilename::operator<( const BlockWithFilename &rhs ) const {
      if ( static_cast<const testing::coverage::Block &>(*this) < static_cast<const testing::coverage::Block &>(rhs) ) {
        return true;
      }
      if ( static_cast<const testing::coverage::Block &>(rhs) < static_cast<const testing::coverage::Block &>(*this) ) {
        return false;
      }
      return file < rhs.file;
    }

    bool BlockWithFilename::operator>( const BlockWithFilename &rhs ) const {
      return rhs < *this;
    }

    bool BlockWithFilename::operator<=( const BlockWithFilename &rhs ) const {
      return !( rhs < *this );
    }

    bool BlockWithFilename::operator>=( const BlockWithFilename &rhs ) const {
      return !( *this < rhs );
    }

    inline std::ostream &operator<<( std::ostream &os, const BlockWithFilename &filename ) {
      os << filename.file << ":" << static_cast<const Block &>(filename);
      return os;
    }
  }
}
#endif //GTESTCOVERAGE_BLOCK_H
