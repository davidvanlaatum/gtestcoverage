#pragma once
#ifndef GTESTCOVERAGE_MAPDIFF_H
#define GTESTCOVERAGE_MAPDIFF_H

#include <map>
#include <functional>

namespace testing {
  namespace coverage {
    template<typename K, typename V, typename P=typename std::add_const<typename std::add_lvalue_reference<V>::type>::type,
             typename Callback=const std::function<bool( P a, P b )> &>
    bool mapDiff( const std::map<K, V> &a, const std::map<K, V> &b, Callback callback, V empty = V() ) {
      bool rt = false;
      auto it1 = a.begin();
      auto it2 = b.begin();
      while ( it1 != a.end() && it2 != b.end() ) {
        if ( it1->first == it2->first ) {
          if ( callback( it1->second, it2->second ) ) {
            rt = true;
          }
          ++it1;
          ++it2;
        } else if ( it1->first < it2->first ) {
          callback( it1->second, empty );
          rt = true;
          ++it1;
        } else {
          callback( empty, it2->second );
          rt = true;
          ++it2;
        }
      }
      while ( it1 != a.end() ) {
        callback( it1->second, empty );
        rt = true;
        ++it1;
      }
      while ( it2 != b.end() ) {
        callback( empty, it2->second );
        rt = true;
        ++it2;
      }
      return rt;
    }
  }
}

#endif //GTESTCOVERAGE_MAPDIFF_H
