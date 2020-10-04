include(CheckIncludeFileCXX)
include(CheckCSourceCompiles)

add_library(kaleidoscope libkaleidoscope.cpp libkaleidoscope.h ikaleidoscope.h sse_mathfun_extension.h sse_mathfun.h)
target_include_directories(kaleidoscope
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

if (NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(kaleidoscope PRIVATE Threads::Threads)
    target_compile_options(kaleidoscope PRIVATE -msse2)
endif()

check_include_file_cxx(immintrin.h HAS_INTEL_INTRINSICS)
if (HAS_INTEL_INTRINSICS)
    add_definitions(-DHAS_INTEL_INTRINSICS)
    if (NOT WIN32)
        set(CMAKE_REQUIRED_FLAGS "-msse2")
    endif()
    check_c_source_compiles("
                               
                                #include <emmintrin.h>
                                #include <immintrin.h>
                                int main(){ __m128 a = _mm_set1_ps(0.0f); a = _mm_sin_ps(a); return 0;}" HAS_SIN_INTRINSIC)
    check_c_source_compiles("
                                #include <emmintrin.h>
                                #include <immintrin.h>
                                int main(){ __m128 a = _mm_set1_ps(0.0f); a = _mm_cos_ps(a); return 0;}" HAS_COS_INTRINSIC)
    check_c_source_compiles("
                                #include <emmintrin.h>
                                #include <immintrin.h>
                                int main(){ __m128 a = _mm_set1_ps(0.0f); __m128 b = _mm_set1_ps(0.0f); a = _mm_atan2_ps(a,b); return 0;}" HAS_ATAN2_INTRINSIC)
    if (HAS_SIN_INTRINSIC)
        add_definitions(-DHAS_SIN_INTRINSIC)
    endif()
    if (HAS_COS_INTRINSIC)
        add_definitions(-DHAS_COS_INTRINSIC)
    endif()
    if (HAS_ATAN2_INTRINSIC)
        add_definitions(-DHAS_ATAN2_INTRINSIC)
    endif()
endif()

install(TARGETS kaleidoscope DESTINATION lib)
install(FILES ikaleidoscope.h DESTINATION include)
