#pragma once
#ifndef GTESTCOVERAGE_FILESYSTEMSERIALIZERS_H
#define GTESTCOVERAGE_FILESYSTEMSERIALIZERS_H

#include <boost/filesystem/path.hpp>
#include <json.hpp>

namespace nlohmann {
  template<> struct adl_serializer<boost::filesystem::path> {
    static void to_json( json &j, const boost::filesystem::path &opt ) {
      j = opt.string();
    }

    static void from_json( const json &j, boost::filesystem::path &data ) {
      data = j.get<std::string>();
    }
  };
}

#endif //GTESTCOVERAGE_FILESYSTEMSERIALIZERS_H
