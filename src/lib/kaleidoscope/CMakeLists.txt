add_library(kaleidoscope libkaleidoscope.cpp libkaleidoscope.h)

target_include_directories(kaleidoscope
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

option(USE_REFLECTION "Calculate reflection by actual reflection rather than rotation" OFF)

if (USE_REFLECTION)
    target_compile_definitions(kaleidoscope INTERFACE "USE_REFLECTION")
    target_compile_definitions(kaleidoscope PRIVATE "USE_REFLECTION")
endif()

install(TARGETS kaleidoscope DESTINATION lib)
install(FILES libkaleidoscope.h DESTINATION include)
