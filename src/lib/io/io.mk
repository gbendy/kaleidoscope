CXXFLAGS+=-I$(ROOT_PATH)/lib/io
LDPOSTFLAGS+=-L $(OUT_PATH)/libio -lio
LINKDEPS+=$(OUT_PATH)/libio/libio.a
SRCDEPS+=$(ROOT_PATH)/lib/io/libio.h
