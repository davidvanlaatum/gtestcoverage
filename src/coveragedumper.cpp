extern "C" void __gcov_reset();
extern "C" void __gcov_dump();
extern "C" void __gcov_flush();

#ifndef PREFIX
#error No Prefix
#endif

#define NAME2( fun, suffix ) fun ## _ ## suffix
#define NAME1( fun, suffix ) NAME2(fun,suffix)
#define NAME( fun ) NAME1(PREFIX,fun)

extern "C" void NAME( coverage_dump )() {
  __gcov_dump();
  __gcov_flush();
}

extern "C" void NAME( coverage_reset )() {
  __gcov_reset();
}
