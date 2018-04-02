#include "CLangCoverageException.h"

using namespace testing::coverage::clang;

CLangCoverageException::CLangCoverageException( const char *msg ) : runtime_error( msg ) {
}

CLangCoverageException::CLangCoverageException( const std::string &msg ) : runtime_error( msg ) {
}
