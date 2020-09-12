.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/kperf
include $(ROOT_PATH)/lib/kaleidoscope/kaleidoscope.mk
include $(ROOT_PATH)/lib/io/io.mk

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/kperf

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/kperf: $(MY_OUT_DIR)/kperf.o $(LINKDEPS)
	c++ -o $(MY_OUT_DIR)/kperf $(MY_OUT_DIR)/kperf.o $(LDPOSTFLAGS)

$(MY_OUT_DIR)/kperf.o: kperf.cpp $(SRCDEPS)
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/kperf.o kperf.cpp

clean:
	rm -f $(MY_OUT_DIR)/kperf.o
