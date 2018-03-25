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
          adl_serializer<T>::to_json( j, *opt );
        } else {
          j = nullptr;
        }
      }

      static void from_json( const json &j, std::shared_ptr<T> &data ) {
        static_assert( std::is_default_constructible<T>::value, "Must be default constructable" );
        if ( !data ) {
          data = std::make_shared<T>();
        }
        adl_serializer<T>::from_json( j, *data );
      }
    };

  template<typename Key, typename Value> inline void from_json( const json &j, std::map<Key, Value> &data ) {
    for ( auto item = j.begin(); item != j.end(); ++item ) {
      data.emplace( item.key(), item.value() );
    }
  }
}

#endif //GTESTCOVERAGE_STDSERIALIZERS_H
