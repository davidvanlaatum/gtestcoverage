extern "C" void __gcov_reset();
extern "C" void __gcov_dump();
extern "C" void __gcov_flush();

extern "C" int __llvm_profile_runtime() { return 0; }

extern "C" void __llvm_profile_initialize_file( void );
extern "C" int __llvm_profile_write_file( void );

#ifndef PREFIX
#error No Prefix
#endif

#define NAME2( fun, suffix ) fun ## _ ## suffix
#define NAME1( fun, suffix ) NAME2(fun,suffix)
#define NAME( fun ) NAME1(PREFIX,fun)

extern "C" void NAME( coverage_dump )() {
#ifdef STYLE_gcc
  __gcov_dump();
  __gcov_flush();
#endif
#ifdef STYLE_clang
  __llvm_profile_write_file();
#endif
}

extern "C" void NAME( coverage_reset )() {
#ifdef STYLE_gcc
  __gcov_reset();
#endif
#ifdef STYLE_clang
  __llvm_profile_initialize_file();
#endif
}

extern "C" const char *NAME( driver_name )() {
  return STYLE;
}
