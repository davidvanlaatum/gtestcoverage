include ( copy_target_properties )

foreach ( MAJOR RANGE 10 4 -1 )
  foreach ( MINOR RANGE 10 0 -1 )
    list ( APPEND CLANG_TIDY_ALIASES clang-tidy-${MAJOR}.${MINOR} clang-tidy-mp-${MAJOR}.${MINOR} )
  endforeach ()
endforeach ()

find_program ( CLANG_TIDY_PATH NAMES ${CLANG_TIDY_ALIASES} clang-tidy )
option ( CLANG_TIDY "Enable clang-tidy" ON )
if ( CLANG_TIDY AND CLANG_TIDY_PATH )
  add_custom_target ( clang_tidy )
  add_dependencies ( checks clang_tidy )
  configure_file ( scripts/clang_tidy.sh clang_tidy.sh @ONLY )
  configure_file ( scripts/clang_tidy_combine.sh clang_tidy_combine.sh @ONLY )
endif ()

find_package_handle_standard_args ( Clang-Tidy
                                    FOUND_VAR Clang-Tidy_FOUND
                                    REQUIRED_VARS CLANG_TIDY_PATH CLANG_TIDY
                                    )

function ( clang_tidy_target TARGET )
  if ( CLANG_TIDY AND CLANG_TIDY_PATH )
    set ( INTERFACE_INCLUDE_DIRECTORIES "" )
    get_target_property ( SOURCES ${TARGET} SOURCES )
    list ( FILTER SOURCES EXCLUDE REGEX "_export.h$" )
    add_library ( ${TARGET}_clang_tidy SHARED ${SOURCES} )
    set_target_properties ( ${TARGET}_clang_tidy PROPERTIES
                            CXX_COMPILER_LAUNCHER ${CMAKE_BINARY_DIR}/clang_tidy.sh
                            C_COMPILER_LAUNCHER ${CMAKE_BINARY_DIR}/clang_tidy.sh
                            EXCLUDE_FROM_ALL TRUE
                            OUTPUT_NAME ${TARGET}.clang_tidy.log
                            SUFFIX ""
                            PREFIX ""
                            LINK_DEPENDS ${CMAKE_BINARY_DIR}/clang_tidy_combine.sh
                            RULE_LAUNCH_LINK ${CMAKE_BINARY_DIR}/clang_tidy_combine.sh )
    set_source_files_properties ( ${SOURCES} OBJECT_DEPENDS ${CMAKE_SOURCE_DIR}/.clang-tidy )
    copy_target_properties ( ${TARGET} _clang_tidy )
    add_for_headers ( ${TARGET}_clang_tidy )
    target_include_directories ( ${TARGET}_clang_tidy SYSTEM PUBLIC ${CXX_DEFAULT_INCLUDES} )
    add_dependencies ( clang_tidy ${TARGET}_clang_tidy )
    if ( DISABLE_COMPILE )
      set_target_properties ( ${TARGET} PROPERTIES
                              C_COMPILER_LAUNCHER ${CMAKE_SOURCE_DIR}/scripts/nulllinker.sh
                              CXX_COMPILER_LAUNCHER ${CMAKE_SOURCE_DIR}/scripts/nulllinker.sh
                              RULE_LAUNCH_LINK ${CMAKE_SOURCE_DIR}/scripts/nulllinker.sh )
    endif ()
  endif ()
endfunction ()
