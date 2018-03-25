#include "CLI.h"

int main( int argc, char **argv ) {
  testing::coverage::cli::CLI cli;
  return cli.run( argc, argv );
}
