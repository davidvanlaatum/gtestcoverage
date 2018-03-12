add_library ( json INTERFACE )
target_sources ( json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp ${CMAKE_CURRENT_SOURCE_DIR}/stdserializers.h )
target_include_directories ( json SYSTEM INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
