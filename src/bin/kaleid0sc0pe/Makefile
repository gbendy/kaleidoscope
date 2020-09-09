.PHONY: all clean

CXXFLAGS+=-fPIC
LDFLAGS+=-fPIC -shared

MY_OUT_DIR=$(OUT_PATH)/kaleid0sc0pe
include $(ROOT_PATH)/lib/kaleidoscope/kaleidoscope.mk
include $(ROOT_PATH)/lib/io/io.mk

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/kaleid0sc0pe.so

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/kaleid0sc0pe.so: $(MY_OUT_DIR)/api.o $(LINKDEPS)
	c++ $(LDFLAGS) -o $(MY_OUT_DIR)/kaleid0sc0pe.so $(MY_OUT_DIR)/api.o $(LDPOSTFLAGS) -lc

$(MY_OUT_DIR)/api.o: api.cpp $(SRCDEPS) frei0r.h frei0r.hpp
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/api.o api.cpp

clean:
	rm -f $(MY_OUT_DIR)/api.o
