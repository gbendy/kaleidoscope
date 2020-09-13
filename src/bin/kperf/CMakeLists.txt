add_executable(kperf kperf.cpp)

target_link_libraries(kperf PUBLIC kaleidoscope)
target_link_libraries(kperf PUBLIC kio)

install(TARGETS kperf DESTINATION bin)
