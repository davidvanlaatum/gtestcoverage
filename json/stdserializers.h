#pragma once
#ifndef GTESTCOVERAGE_STDSERIALIZERS_H
#define GTESTCOVERAGE_STDSERIALIZERS_H

#include "json.hpp"
#include <memory>

namespace nlohmann {
  template<typename T>
    struct adl_serializer<std::shared_ptr<T>> {
      static void to_json( json &j, const std::shared_ptr<T> &opt ) {
        if ( opt ) {
          to_json( j, *opt );
        } else {
          j = nullptr;
        }
      }

      template<typename X=T> static void from_json( const json &j, std::shared_ptr<X> &data ) {
        if ( !data ) {
          data = std::make_shared<T>();
        }
        adl_serializer<X>::from_json( j, *data );
      }
    };
}

#endif //GTESTCOVERAGE_STDSERIALIZERS_H
