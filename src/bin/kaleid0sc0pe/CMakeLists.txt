add_library(kaleid0sc0pe SHARED api.cpp frei0r.h frei0r.hpp frei0r_1_2.def)

target_link_libraries(kaleid0sc0pe PUBLIC kaleidoscope)

set_target_properties(kaleidoscope PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
)

set_target_properties (kaleid0sc0pe PROPERTIES PREFIX "")

install(TARGETS kaleid0sc0pe DESTINATION lib/frei0r-1)
