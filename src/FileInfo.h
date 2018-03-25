#pragma once
#ifndef GTESTCOVERAGE_FILEINFO_H
#define GTESTCOVERAGE_FILEINFO_H

#include "Block.h"   // for Block
#include "fwd.h"     // for BlockInfoPtr, LineInfoPtr, TestInfoWeakPtr, path, TestInfoPtr
#include <cstdint>   // for uint32_t
#include <json.hpp>  // for json
#include <map>       // for map
#include <memory>    // for enable_shared_from_this
#include <string>    // for string
#include <ostream>

namespace testing {
  namespace coverage {
    class LineNumber {
    public:
      typedef uint32_t innerType;
      LineNumber() = default;
      inline LineNumber( innerType nnum );
      inline LineNumber( const std::string &lineNum );
      inline bool operator==( const LineNumber &rhs ) const;
      inline bool operator!=( const LineNumber &rhs ) const;
      inline bool operator<( const LineNumber &rhs ) const;
      inline bool operator>( const LineNumber &rhs ) const;
      inline bool operator<=( const LineNumber &rhs ) const;
      inline bool operator>=( const LineNumber &rhs ) const;
      inline operator std::string() const;
      inline operator innerType() const;
      friend std::ostream &operator<<( std::ostream &os, const LineNumber &number );
    protected:
      innerType num{ 0 };
    };

    class FileInfo : public std::enable_shared_from_this<FileInfo> {
    public:
      FileInfo() = default;
      explicit FileInfo( const path &nName );
      void addCoveringTest( const TestInfoPtr &test );
      const BlockInfoPtr &getBlock( const Block &block );
      const LineInfoPtr &getLine( uint32_t line );
      const path &getName() const;
      void setName( const path &nName );
      size_t getBlockCount() const;
      size_t getCoveredBlocks() const;
      size_t getLineCount() const;
      size_t getCoveredLines() const;
      void readResolve( const CoverageDataPtr &data );
      bool hasCoverage() const;
      const std::map<LineNumber, LineInfoPtr> &getLines() const;
    protected:
      path name;
      std::map<std::string, TestInfoWeakPtr> tests;
      std::map<LineNumber, LineInfoPtr> lines;
      std::map<Block, BlockInfoPtr> blocks;

      friend void from_json( const nlohmann::json &j, FileInfo &data );
      friend void to_json( nlohmann::json &j, const FileInfo &data );
    };
    void from_json( const nlohmann::json &j, FileInfo &data );
    void to_json( nlohmann::json &j, const FileInfo &data );

    LineNumber::LineNumber( LineNumber::innerType nnum ) : num( nnum ) {}

    LineNumber::LineNumber( const std::string &lineNum ) {
      std::istringstream str( lineNum );
      str >> num;
    }

    bool LineNumber::operator==( const LineNumber &rhs ) const {
      return num == rhs.num;
    }

    bool LineNumber::operator!=( const LineNumber &rhs ) const {
      return !( rhs == *this );
    }

    bool LineNumber::operator<( const LineNumber &rhs ) const {
      return num < rhs.num;
    }

    bool LineNumber::operator>( const LineNumber &rhs ) const {
      return rhs < *this;
    }

    bool LineNumber::operator<=( const LineNumber &rhs ) const {
      return !( rhs < *this );
    }

    bool LineNumber::operator>=( const LineNumber &rhs ) const {
      return !( *this < rhs );
    }

    inline std::ostream &operator<<( std::ostream &os, const LineNumber &number ) {
      os << number.num;
      return os;
    }

    LineNumber::operator std::string() const {
      return std::to_string( num );
    }

    LineNumber::operator innerType() const {
      return num;
    }
  }
}

#endif //GTESTCOVERAGE_FILEINFO_H
