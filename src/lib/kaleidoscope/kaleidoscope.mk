CXXFLAGS+=-I../../lib/kaleidoscope
LDPOSTFLAGS+=-L $(OUT_PATH)/libkaleidoscope -lkaleidoscope
LINKDEPS+=$(OUT_PATH)/libkaleidoscope/libkaleidoscope.a
SRCDEPS+=../../lib/kaleidoscope/libkaleidoscope.h
