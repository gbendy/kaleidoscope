add_executable(kdiff kdiff.cpp)

target_link_libraries(kdiff PUBLIC kio)

install(TARGETS kdiff DESTINATION bin)
