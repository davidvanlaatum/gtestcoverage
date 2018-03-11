include ( FindPkgConfig )
pkg_check_modules ( tinyxml QUIET IMPORTED_TARGET tinyxml2 )
mark_as_advanced ( pkgcfg_lib_tinyxml_tinyxml2 )

if ( TARGET PkgConfig::tinyxml )
  set ( TINY_XML_FOUND TRUE )
  add_library ( tinyxml INTERFACE )
  set_target_properties ( tinyxml PROPERTIES INTERFACE_LINK_LIBRARIES PkgConfig::tinyxml )
else ()
    include ( GNUInstallDirs )
    include ( ExternalProject )
    set ( PREFIX ${CMAKE_BINARY_DIR}/external/tinyxml/install/ )
    externalproject_add ( tinyxmlExternal PREFIX external/tinyxml
                          URL https://github.com/leethomason/tinyxml2/archive/4.0.1.tar.gz
                          URL_HASH SHA1=08570d385788f6b02f50f5fd9df32a9d4f8482cc
                          TIMEOUT 600e
                          DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/
                          CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON
                          -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX}
                          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
                          BUILD_BYPRODUCTS "${PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2${CMAKE_STATIC_LIBRARY_SUFFIX}" )
    add_library ( tinyxml INTERFACE )
    set_target_properties ( tinyxml PROPERTIES
                            INTERFACE_LINK_LIBRARIES "${PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2${CMAKE_STATIC_LIBRARY_SUFFIX}"
                            INTERFACE_INCLUDE_DIRECTORIES "${PREFIX}/include"
                            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${PREFIX}/include" )
    add_dependencies ( tinyxml tinyxmlExternal )
  set ( TINY_XML_FOUND true )
endif ()
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args ( TinyXML REQUIRED_VARS TINY_XML_FOUND VERSION_VAR tinyxml_VERSION )
