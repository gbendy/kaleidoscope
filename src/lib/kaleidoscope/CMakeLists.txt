add_library(kaleidoscope libkaleidoscope.cpp libkaleidoscope.h ikaleidoscope.h)

target_include_directories(kaleidoscope
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

if (NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(kaleidoscope PRIVATE Threads::Threads)
endif()

install(TARGETS kaleidoscope DESTINATION lib)
install(FILES ikaleidoscope.h DESTINATION include)
