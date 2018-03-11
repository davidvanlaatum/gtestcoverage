if ( NOT TARGET googleinclude )
  find_package ( Threads REQUIRED )
  find_path ( GTEST_INCLUDE_DIR gtest/gtest.h HINTS $ENV{GTEST_ROOT}/include ${GTEST_ROOT}/include )
  mark_as_advanced ( GTEST_INCLUDE_DIR )
  find_library ( GTEST_LIBRARY NAMES gtest HINTS ENV GTEST_ROOT ${GTEST_ROOT} )
  mark_as_advanced ( GTEST_LIBRARY )

  find_path ( GMOCK_INCLUDE_DIR gmock/gmock.h HINTS $ENV{GMOCK_ROOT}/include ${GMOCK_ROOT}/include )
  mark_as_advanced ( GMOCK_INCLUDE_DIR )
  find_library ( GMOCK_LIBRARY NAMES gmock HINTS ENV GMOCK_ROOT ${GMOCK_ROOT} )
  mark_as_advanced ( GMOCK_LIBRARY )
  include ( FindPackageHandleStandardArgs )

  if ( GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GMOCK_INCLUDE_DIR AND GMOCK_LIBRARY )
    message ( STATUS "Using system gtest" )
    add_library ( GTest::GTest UNKNOWN IMPORTED GLOBAL )
    set_target_properties ( GTest::GTest PROPERTIES
                            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
                            INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
                            IMPORTED_LOCATION ${GTEST_LIBRARY}
                            )
    add_library ( GMock::GMock UNKNOWN IMPORTED GLOBAL )
    set_target_properties ( GMock::GMock PROPERTIES
                            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${GMOCK_INCLUDE_DIR}
                            INTERFACE_INCLUDE_DIRECTORIES ${GMOCK_INCLUDE_DIR}
                            IMPORTED_LOCATION ${GMOCK_LIBRARY}
                            INTERFACE_COMPILE_DEFINITIONS GTEST_LINKED_AS_SHARED_LIBRARY
                            )

    find_library ( GMOCK_MAIN NAMES gmock_main HINTS ENV GMOCK_ROOT ${GMOCK_ROOT} )
    add_library ( GMock::Main UNKNOWN IMPORTED GLOBAL )
    set_target_properties ( GMock::Main PROPERTIES
                            IMPORTED_LOCATION ${GMOCK_MAIN}
                            )

    find_package_handle_standard_args ( GTest REQUIRED_VARS GTEST_LIBRARY GTEST_INCLUDE_DIR GMOCK_LIBRARY GMOCK_INCLUDE_DIR )
  else ()
    message ( STATUS "Using downloaded gtest" )
    set ( gtest_force_shared_crt ON CACHE BOOL "" FORCE )
    set ( BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE )
    mark_as_advanced ( gtest_force_shared_crt )
    mark_as_advanced ( gmock_build_tests )
    mark_as_advanced ( gtest_build_samples )
    mark_as_advanced ( gtest_build_tests )
    mark_as_advanced ( gtest_disable_pthreads )
    mark_as_advanced ( gtest_hide_internal_symbols )
    if ( IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage )
      include ( ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/DownloadProject.cmake )
    else ()
      include ( DownloadProject )
    endif ()
    cmake_policy ( SET CMP0048 NEW )
    cmake_policy ( SET CMP0042 NEW )
    set ( CMAKE_MACOSX_RPATH TRUE )
    download_project ( PROJ googletest
                       URL http://github.com/google/googletest/archive/release-1.8.0.zip
                       URL_HASH SHA1=667f873ab7a4d246062565fad32fb6d8e203ee73
                       TIMEOUT 600e
                       DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/
                       )
    add_subdirectory ( ${googletest_SOURCE_DIR}/googlemock ${googletest_BINARY_DIR} )
    set_directory_properties ( TARGETS gtest gtest_main gmock_main PROPERTY EXCLUDE_FROM_ALL TRUE )

    get_target_property ( GTEST_INCLUDE_DIR gtest INTERFACE_INCLUDE_DIRECTORIES )
    get_target_property ( GMOCK_INCLUDE_DIR gmock INTERFACE_INCLUDE_DIRECTORIES )

    add_library ( GTest::GTest INTERFACE IMPORTED GLOBAL )
    #    target_link_libraries ( GTest::GTest INTERFACE gtest )
    add_library ( GMock::GMock INTERFACE IMPORTED GLOBAL )
    target_link_libraries ( GMock::GMock INTERFACE gmock )
    set_property ( TARGET GMock::GMock APPEND PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR} )
    set_property ( TARGET GMock::GMock APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR} )
    find_package_handle_standard_args ( GTest REQUIRED_VARS GTEST_INCLUDE_DIR GMOCK_INCLUDE_DIR )
  endif ()
  add_library ( googleinclude INTERFACE )
  target_include_directories ( googleinclude SYSTEM INTERFACE ${GTEST_INCLUDE_DIR} )
  target_include_directories ( googleinclude SYSTEM INTERFACE ${GMOCK_INCLUDE_DIR} )

  #  include ( printproperties )
  #  print_properties ( TARGETS GTest::GTest GMock::GMock )
endif ()
