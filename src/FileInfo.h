#pragma once
#ifndef GTESTCOVERAGE_FILEINFO_H
#define GTESTCOVERAGE_FILEINFO_H

#include "fwd.h"
#include <memory>
#include <json.hpp>
#include <map>
#include <boost/filesystem/path.hpp>
#include "Block.h"

namespace testing {
  namespace coverage {
    class FileInfo : public std::enable_shared_from_this<FileInfo> {
    public:
      explicit FileInfo( const path &nName );
      void addCoveringTest( const TestInfoPtr &test );
      const BlockInfoPtr &getBlock( const Block &block );
    protected:
      path name;
      std::map<std::string, TestInfoWeakPtr> tests;
      std::map<uint32_t, LineInfoPtr> lines;
      std::map<Block, BlockInfoPtr> blocks;

      friend void from_json( const nlohmann::json &j, FileInfo &data );
      friend void to_json( nlohmann::json &j, const FileInfo &data );
    };
    void from_json( const nlohmann::json &j, FileInfo &data );
    void to_json( nlohmann::json &j, const FileInfo &data );
  }
}

#endif //GTESTCOVERAGE_FILEINFO_H
