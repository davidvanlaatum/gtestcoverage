#pragma once
#ifndef GTESTCOVERAGE_GTESTCOVERAGECOVERS_H
#define GTESTCOVERAGE_GTESTCOVERAGECOVERS_H

#include <list>
#include <sstream>
#include <boost/core/typeinfo.hpp>

namespace testing {
  namespace coverage {
    inline std::string toString( const std::list<std::string> &list ) {
      std::ostringstream out;
      bool first = true;
      for ( const auto &item : list ) {
        if ( first ) {
          first = false;
        } else {
          out << std::endl;
        }
        out << item;
      }
      return out.str();
    }

    template<typename T> struct returnExtract { typedef void type; };
    template<typename R, typename...A> struct returnExtract<R( A... )> { typedef R type; };
    template<typename...> struct argsExtract { static std::string toString() { return std::string(); } };
    template<typename T, typename Y, typename... A> struct argsExtract<T, Y, A...> {
      static std::string toString() {
        return boost::core::demangled_name( typeid( T ) ) + ", " + argsExtract<Y, A...>::toString();
      }
    };
    template<typename T> struct argsExtract<T> { static std::string toString() { return boost::core::demangled_name( typeid( T ) ); } };
    template<typename T, typename... A> struct argsExtract<T( A... )> { static std::string toString() { return argsExtract<A...>::toString(); } };
    template<typename T> struct argsExtract<T()> { static std::string toString() { return std::string(); } };

    std::string functionComponentsToString( const std::type_info &returnType, const std::type_info &classType, const char *functionName,
                                            const std::string &argumentTypes, const std::string &templateArguments = std::string() ) {
      std::ostringstream rt;
      if ( not templateArguments.empty() ) {
        rt << boost::core::demangled_name( returnType ) << " ";
      }
      rt << boost::core::demangled_name( classType ) << "::" << functionName;
      if ( not templateArguments.empty() ) {
        rt << "<" << templateArguments << ">";
      }
      rt << "(" << argumentTypes << ")";
      return rt.str();
    }

    template<typename... X> struct functionCheck {};
    template<typename A, typename B, typename... C> struct functionCheck<A, B( C... )> { inline static void func( B (A::*)( C... ) ) {} };
  }
}

#define COVERS_FILES_PROPERTY "COVERS_FILES"
#define COVERS_FUNCTIONS_PROPERTY "COVERS_FUNCTIONS"
#define COVERS_NOTHING_PROPERTY "COVERS_NOTHING"

#define COVERS_FILES( files ) RecordProperty( COVERS_FILES_PROPERTY, files )
#define COVERS_BEGIN_FUNCTIONS { \
  std::list<std::string> functions;
#define COVERS_CLASS_FUNCTION( c, n, s ) \
  functions.emplace_back( testing::coverage::functionComponentsToString( typeid( testing::coverage::returnExtract<s>::type ), typeid( c ), #n, testing::coverage::argsExtract<s>::toString() ) ); \
  testing::coverage::functionCheck<c,s>::func( &c::n );
#define COVERS_CLASS_FUNCTION_T( c, n, s, t... ) \
  functions.emplace_back( testing::coverage::functionComponentsToString( typeid( testing::coverage::returnExtract<s>::type ), typeid( c ), #n, testing::coverage::argsExtract<s>::toString(), testing::coverage::argsExtract<t>::toString() ) ); \
  testing::coverage::functionCheck<c,s>::func( &c::n<t> );
#define COVERS_END_FUNCTIONS RecordProperty(COVERS_FUNCTIONS_PROPERTY, testing::coverage::toString(functions) ); \
}
#define COVERS_NOTHING RecordProperty(COVERS_NOTHING_PROPERTY,"");

#endif //GTESTCOVERAGE_GTESTCOVERAGECOVERS_H
