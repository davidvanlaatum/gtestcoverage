if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(ORIGIN @loader_path)
else ()
    set(ORIGIN "\$ORIGIN")
endif ()