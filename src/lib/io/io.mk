CXXFLAGS+=-I../../lib/io
LDPOSTFLAGS+=-L $(OUT_PATH)/libio -lio
LINKDEPS+=$(OUT_PATH)/libio/libio.a
SRCDEPS+=../../lib/io/libio.h
