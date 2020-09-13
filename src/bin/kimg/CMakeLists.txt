add_executable(kimg kimg.cpp)

target_link_libraries(kimg PUBLIC kaleidoscope)
target_link_libraries(kimg PUBLIC kio)

install(TARGETS kimg DESTINATION bin)
