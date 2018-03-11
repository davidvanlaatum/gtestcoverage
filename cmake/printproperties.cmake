set ( CONFIG_LIST
      DEBUG
      RELEASE
      RELWITHDEBINFO
      MINSIZEREL
      )
set ( LANG_LIST C CXX )

if ( CMAKE_BUILD_TYPE )
  string ( TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE )
  list ( APPEND CONFIG_LIST ${CMAKE_BUILD_TYPE} )
endif ()
if ( CMAKE_CONFIGURATION_TYPES )
  list ( APPEND CONFIG_LIST ${CMAKE_CONFIGURATION_TYPES} )
endif ()
list ( REMOVE_DUPLICATES CONFIG_LIST )

set ( TARGET_PROPERTIES_LIST
      <CONFIG>_OUTPUT_NAME
      <CONFIG>_POSTFIX
      <LANG>_CLANG_TIDY
      <LANG>_COMPILER_LAUNCHER
      <LANG>_CPPCHECK
      <LANG>_CPPLINT
      <LANG>_INCLUDE_WHAT_YOU_USE
      <LANG>_VISIBILITY_PRESET
      ALIASED_TARGET
      ANDROID_ANT_ADDITIONAL_OPTIONS
      ANDROID_API
      ANDROID_API_MIN
      ANDROID_ARCH
      ANDROID_ASSETS_DIRECTORIES
      ANDROID_GUI
      ANDROID_JAR_DEPENDENCIES
      ANDROID_JAR_DIRECTORIES
      ANDROID_JAVA_SOURCE_DIR
      ANDROID_NATIVE_LIB_DEPENDENCIES
      ANDROID_NATIVE_LIB_DIRECTORIES
      ANDROID_PROCESS_MAX
      ANDROID_PROGUARD
      ANDROID_PROGUARD_CONFIG_PATH
      ANDROID_SECURE_PROPS_PATH
      ANDROID_SKIP_ANT_STEP
      ANDROID_STL_TYPE
      ARCHIVE_OUTPUT_DIRECTORY
      ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>
      ARCHIVE_OUTPUT_NAME
      ARCHIVE_OUTPUT_NAME_<CONFIG>
      AUTOGEN_BUILD_DIR
      AUTOGEN_PARALLEL
      AUTOGEN_TARGET_DEPENDS
      AUTOMOC
      AUTOMOC_COMPILER_PREDEFINES
      AUTOMOC_DEPEND_FILTERS
      AUTOMOC_MACRO_NAMES
      AUTOMOC_MOC_OPTIONS
      AUTORCC
      AUTORCC_OPTIONS
      AUTOUIC
      AUTOUIC_OPTIONS
      AUTOUIC_SEARCH_PATHS
      BINARY_DIR
      BUILD_RPATH
      BUILD_WITH_INSTALL_NAME_DIR
      BUILD_WITH_INSTALL_RPATH
      BUNDLE
      BUNDLE_EXTENSION
      C_EXTENSIONS
      C_STANDARD
      C_STANDARD_REQUIRED
      COMPATIBLE_INTERFACE_BOOL
      COMPATIBLE_INTERFACE_NUMBER_MAX
      COMPATIBLE_INTERFACE_NUMBER_MIN
      COMPATIBLE_INTERFACE_STRING
      COMPILE_DEFINITIONS
      COMPILE_FEATURES
      COMPILE_FLAGS
      COMPILE_OPTIONS
      COMPILE_PDB_NAME
      COMPILE_PDB_NAME_<CONFIG>
      COMPILE_PDB_OUTPUT_DIRECTORY
      COMPILE_PDB_OUTPUT_DIRECTORY_<CONFIG>
      CROSSCOMPILING_EMULATOR
      CUDA_EXTENSIONS
      CUDA_PTX_COMPILATION
      CUDA_RESOLVE_DEVICE_SYMBOLS
      CUDA_SEPARABLE_COMPILATION
      CUDA_STANDARD
      CUDA_STANDARD_REQUIRED
      CXX_EXTENSIONS
      CXX_STANDARD
      CXX_STANDARD_REQUIRED
      DEBUG_POSTFIX
      DEFINE_SYMBOL
      DEPLOYMENT_REMOTE_DIRECTORY
      EchoString
      ENABLE_EXPORTS
      EXCLUDE_FROM_ALL
      EXCLUDE_FROM_DEFAULT_BUILD
      EXCLUDE_FROM_DEFAULT_BUILD_<CONFIG>
      EXPORT_NAME
      FOLDER
      Fortran_FORMAT
      Fortran_MODULE_DIRECTORY
      FRAMEWORK
      FRAMEWORK_VERSION
      GENERATOR_FILE_NAME
      GNUtoMS
      HAS_CXX
      IMPLICIT_DEPENDS_INCLUDE_TRANSFORM
      IMPORT_PREFIX
      IMPORT_SUFFIX
      IMPORTED
      IMPORTED_CONFIGURATIONS
      IMPORTED_GLOBAL
      IMPORTED_IMPLIB
      IMPORTED_IMPLIB_<CONFIG>
      IMPORTED_LIBNAME
      IMPORTED_LIBNAME_<CONFIG>
      IMPORTED_LINK_DEPENDENT_LIBRARIES
      IMPORTED_LINK_DEPENDENT_LIBRARIES_<CONFIG>
      IMPORTED_LINK_INTERFACE_LANGUAGES
      IMPORTED_LINK_INTERFACE_LANGUAGES_<CONFIG>
      IMPORTED_LINK_INTERFACE_LIBRARIES
      IMPORTED_LINK_INTERFACE_LIBRARIES_<CONFIG>
      IMPORTED_LINK_INTERFACE_MULTIPLICITY
      IMPORTED_LINK_INTERFACE_MULTIPLICITY_<CONFIG>
      IMPORTED_LOCATION
      IMPORTED_LOCATION_<CONFIG>
      IMPORTED_NO_SONAME
      IMPORTED_NO_SONAME_<CONFIG>
      IMPORTED_OBJECTS
      IMPORTED_OBJECTS_<CONFIG>
      IMPORTED_SONAME
      IMPORTED_SONAME_<CONFIG>
      INCLUDE_DIRECTORIES
      INSTALL_NAME_DIR
      INSTALL_RPATH
      INSTALL_RPATH_USE_LINK_PATH
      INTERFACE_AUTOUIC_OPTIONS
      INTERFACE_COMPILE_DEFINITIONS
      INTERFACE_COMPILE_FEATURES
      INTERFACE_COMPILE_OPTIONS
      INTERFACE_INCLUDE_DIRECTORIES
      INTERFACE_LINK_LIBRARIES
      INTERFACE_POSITION_INDEPENDENT_CODE
      INTERFACE_SOURCES
      INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
      INTERPROCEDURAL_OPTIMIZATION
      INTERPROCEDURAL_OPTIMIZATION_<CONFIG>
      IOS_INSTALL_COMBINED
      JOB_POOL_COMPILE
      JOB_POOL_LINK
      LABELS
      LIBRARY_OUTPUT_DIRECTORY
      LIBRARY_OUTPUT_DIRECTORY_<CONFIG>
      LIBRARY_OUTPUT_NAME
      LIBRARY_OUTPUT_NAME_<CONFIG>
      LINK_DEPENDS
      LINK_DEPENDS_NO_SHARED
      LINK_FLAGS
      LINK_FLAGS_<CONFIG>
      LINK_INTERFACE_LIBRARIES
      LINK_INTERFACE_LIBRARIES_<CONFIG>
      LINK_INTERFACE_MULTIPLICITY
      LINK_INTERFACE_MULTIPLICITY_<CONFIG>
      LINK_LIBRARIES
      LINK_SEARCH_END_STATIC
      LINK_SEARCH_START_STATIC
      LINK_WHAT_YOU_USE
      LINKER_LANGUAGE
      LOCATION
      LOCATION_<CONFIG>
      MACOSX_BUNDLE
      MACOSX_BUNDLE_INFO_PLIST
      MACOSX_FRAMEWORK_INFO_PLIST
      MACOSX_RPATH
      MANUALLY_ADDED_DEPENDENCIES
      MAP_IMPORTED_CONFIG_<CONFIG>
      NAME
      NO_SONAME
      NO_SYSTEM_FROM_IMPORTED
      OSX_ARCHITECTURES
      OSX_ARCHITECTURES_<CONFIG>
      OUTPUT_NAME
      OUTPUT_NAME_<CONFIG>
      PDB_NAME
      PDB_NAME_<CONFIG>
      PDB_OUTPUT_DIRECTORY
      PDB_OUTPUT_DIRECTORY_<CONFIG>
      POSITION_INDEPENDENT_CODE
      PREFIX
      PRIVATE_HEADER
      PROJECT_LABEL
      PUBLIC_HEADER
      RESOURCE
      RULE_LAUNCH_COMPILE
      RULE_LAUNCH_CUSTOM
      RULE_LAUNCH_LINK
      RUNTIME_OUTPUT_DIRECTORY
      RUNTIME_OUTPUT_DIRECTORY_<CONFIG>
      RUNTIME_OUTPUT_NAME
      RUNTIME_OUTPUT_NAME_<CONFIG>
      SKIP_BUILD_RPATH
      SOURCE_DIR
      SOURCES
      SOVERSION
      STATIC_LIBRARY_FLAGS
      STATIC_LIBRARY_FLAGS_<CONFIG>
      SUFFIX
      TYPE
      VERSION
      VISIBILITY_INLINES_HIDDEN
      VS_CONFIGURATION_TYPE
      VS_DEBUGGER_WORKING_DIRECTORY
      VS_DESKTOP_EXTENSIONS_VERSION
      VS_DOTNET_REFERENCE_<refname>
      VS_DOTNET_REFERENCEPROP_<refname>_TAG_<tagname>
      VS_DOTNET_REFERENCES
      VS_DOTNET_REFERENCES_COPY_LOCAL
      VS_DOTNET_TARGET_FRAMEWORK_VERSION
      VS_GLOBAL_<variable>
      VS_GLOBAL_KEYWORD
      VS_GLOBAL_PROJECT_TYPES
      VS_GLOBAL_ROOTNAMESPACE
      VS_IOT_EXTENSIONS_VERSION
      VS_IOT_STARTUP_TASK
      VS_KEYWORD
      VS_MOBILE_EXTENSIONS_VERSION
      VS_SCC_AUXPATH
      VS_SCC_LOCALPATH
      VS_SCC_PROJECTNAME
      VS_SCC_PROVIDER
      VS_SDK_REFERENCES
      VS_USER_PROPS
      VS_WINDOWS_TARGET_PLATFORM_MIN_VERSION
      VS_WINRT_COMPONENT
      VS_WINRT_EXTENSIONS
      VS_WINRT_REFERENCES
      WIN32_EXECUTABLE
      WINDOWS_EXPORT_ALL_SYMBOLS
      XCODE_ATTRIBUTE_<an-attribute>
      XCODE_EXPLICIT_FILE_TYPE
      XCODE_PRODUCT_TYPE
      XCTEST
      )

set ( INTERFACE_LIBRARY_WHITELIST ${TARGET_PROPERTIES_LIST} )
list ( FILTER INTERFACE_LIBRARY_WHITELIST INCLUDE REGEX "(^COMPATIBLE_INTERFACE|^IMPORTED|^INTERFACE)" )
list ( APPEND INTERFACE_LIBRARY_WHITELIST
       COMPATIBLE_INTERFACE_BOOL
       COMPATIBLE_INTERFACE_NUMBER_MAX
       COMPATIBLE_INTERFACE_NUMBER_MIN
       COMPATIBLE_INTERFACE_STRING
       EXPORT_NAME
       IMPORTED
       IMPORTED_CONFIGURATIONS
       IMPORTED_LIBNAME
       IMPORTED_LIBNAME_<CONFIG>
       INTERFACE_AUTOUIC_OPTIONS
       INTERFACE_COMPILE_DEFINITIONS
       INTERFACE_COMPILE_FEATURES
       INTERFACE_COMPILE_OPTIONS
       INTERFACE_INCLUDE_DIRECTORIES
       INTERFACE_LINK_LIBRARIES
       INTERFACE_POSITION_INDEPENDENT_CODE
       INTERFACE_SOURCES
       INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
       MAP_IMPORTED_CONFIG_<CONFIG>
       NAME
       NO_SYSTEM_FROM_IMPORTED
       TYPE
       )
set ( INTERFACE_LIBRARY_BLACKLIST
      IMPORTED_GLOBAL
      IMPORTED_LINK_INTERFACE_MULTIPLICITY
      IMPORTED_LINK_INTERFACE_MULTIPLICITY_<CONFIG>
      IMPORTED_LOCATION
      IMPORTED_LOCATION_<CONFIG>
      IMPORTED_NO_SONAME
      IMPORTED_NO_SONAME_<CONFIG>
      IMPORTED_OBJECTS
      IMPORTED_OBJECTS_<CONFIG>
      IMPORTED_SONAME
      IMPORTED_SONAME_<CONFIG>
      IMPORTED_LINK_INTERFACE_LIBRARIES
      IMPORTED_LINK_INTERFACE_LIBRARIES_<CONFIG>
      IMPORTED_LINK_INTERFACE_LANGUAGES
      IMPORTED_LINK_INTERFACE_LANGUAGES_<CONFIG>
      IMPORTED_LINK_DEPENDENT_LIBRARIES
      IMPORTED_LINK_DEPENDENT_LIBRARIES_<CONFIG>
      IMPORTED_IMPLIB
      IMPORTED_IMPLIB_<CONFIG>
      )

set ( SHARED_LIBRARY_BLACKLIST
      LOCATION
      LOCATION_<CONFIG>
      )

set ( STATIC_LIBRARY_BLACKLIST
      LOCATION
      LOCATION_<CONFIG>
      )

set ( MODULE_LIBRARY_BLACKLIST
      LOCATION
      LOCATION_<CONFIG>
      )

set ( EXECUTABLE_BLACKLIST
      LOCATION
      LOCATION_<CONFIG>
      )

function ( print_target_property TARGET PROPERTY RETURN )
  get_property ( SET TARGET ${TARGET} PROPERTY ${PROPERTY} SET )
  if ( SET )
    get_target_property ( VALUE ${TARGET} ${PROPERTY} )
    if ( RETURN )
      set ( OUT "  ${PROPERTY} = ${VALUE}" PARENT_SCOPE )
    else ()
      message ( STATUS "  ${PROPERTY} = ${VALUE}" )
    endif ()
  elseif ( RETURN )
    set ( OUT "" PARENT_SCOPE )
  endif ()
endfunction ()

function ( walk_target_property TARGET PROP RETURN )
  set ( VALUE )
  if ( PROP MATCHES "<CONFIG>$" )
    foreach ( CONFIG IN LISTS CONFIG_LIST )
      string ( REPLACE "<CONFIG>" ${CONFIG} CONFIG_PROP ${PROP} )
      walk_target_property ( ${TARGET} ${CONFIG_PROP} ${RETURN} )
      if ( RETURN AND OUT )
        string ( APPEND VALUE "${OUT}" )
      endif ()
    endforeach ()
  elseif ( PROP MATCHES "<LANG>" )
    foreach ( CONFIG IN LISTS LANG_LIST )
      string ( REPLACE "<LANG>" ${CONFIG} CONFIG_PROP ${PROP} )
      walk_target_property ( ${TARGET} ${CONFIG_PROP} ${RETURN} )
      if ( RETURN AND OUT )
        string ( APPEND VALUE "${OUT}" )
      endif ()
    endforeach ()
  else ()
    print_target_property ( ${TARGET} ${PROP} ${RETURN} )
    if ( RETURN AND OUT )
      string ( APPEND VALUE "${OUT}\n" )
    endif ()
  endif ()
  set ( OUT ${VALUE} PARENT_SCOPE )
endfunction ()

function ( print_properties )
  set ( OPTIONS )
  set ( VALUES VAR )
  set ( MVALUES TARGETS )
  cmake_parse_arguments ( MY "${OPTIONS}" "${VALUES}" "${MVALUES}" ${ARGN} )
  set ( RETURN FALSE )

  if ( DEFINED MY_VAR )
    set ( RETURN TRUE )
    set ( VALUE )
  endif ()

  foreach ( T IN LISTS MY_TARGETS )
    if ( NOT TARGET ${T} )
      message ( WARNING "No such target ${T}" )
      continue ()
    endif ()
    get_target_property ( TYPE ${T} TYPE )
    if ( RETURN )
      string ( APPEND VALUE "Target ${T} ${TYPE}\n" )
    else ()
      message ( STATUS "Target ${T} ${TYPE}" )
    endif ()
    foreach ( PROP IN LISTS TARGET_PROPERTIES_LIST )
      if ( ${TYPE}_BLACKLIST AND ${PROP} IN_LIST ${TYPE}_BLACKLIST )
        continue ()
      endif ()
      if ( ${TYPE}_WHITELIST AND NOT ${PROP} IN_LIST ${TYPE}_WHITELIST )
        continue ()
      endif ()
      walk_target_property ( ${T} ${PROP} ${RETURN} )
      if ( RETURN AND OUT )
        string ( APPEND VALUE "${OUT}" )
      endif ()
    endforeach ()
  endforeach ()
  if ( RETURN )
    set ( ${MY_VAR} "${VALUE}" PARENT_SCOPE )
  endif ()
endfunction ()