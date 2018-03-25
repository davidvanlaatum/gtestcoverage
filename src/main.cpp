#include "CLI.h"

int main( int argc, char **argv ) {
  testing::coverage::cli::CLI cli;
  return cli.run( argc, argv );
}

extern "C" void main_coverage_dump() {
}

extern "C" void main_coverage_reset() {
}

extern "C" void unit_coverage_dump() {
}

extern "C" void unit_coverage_reset() {
}

extern "C" const char *main_driver_name() {
  return "null";
}

extern "C" const char *unit_driver_name() {
  return "null";
}
