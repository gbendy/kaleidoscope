add_executable(ktest ktest.cpp)

target_link_libraries(ktest PUBLIC kaleidoscope)
target_link_libraries(ktest PUBLIC kio)

install(TARGETS ktest DESTINATION bin)
