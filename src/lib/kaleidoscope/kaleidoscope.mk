CXXFLAGS+=-I$(ROOT_PATH)/lib/kaleidoscope
LDPOSTFLAGS+=-L $(OUT_PATH)/libkaleidoscope -lkaleidoscope
LINKDEPS+=$(OUT_PATH)/libkaleidoscope/libkaleidoscope.a
SRCDEPS+=$(ROOT_PATH)/lib/kaleidoscope/libkaleidoscope.h
