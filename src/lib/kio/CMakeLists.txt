add_library(kio libkio.cpp libkio.h)

target_include_directories(kio
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


install(TARGETS kio DESTINATION lib)
install(FILES libkio.h DESTINATION include)
