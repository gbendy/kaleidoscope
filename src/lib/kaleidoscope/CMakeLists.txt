add_library(kaleidoscope libkaleidoscope.cpp)

target_include_directories(kaleidoscope
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS kaleidoscope DESTINATION lib)
install(FILES libkaleidoscope.h DESTINATION include)