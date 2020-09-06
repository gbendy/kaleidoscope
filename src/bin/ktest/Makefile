.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/ktest
include ../../lib/kaleidoscope/kaleidoscope.mk
include ../../lib/io/io.mk

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/ktest

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/ktest: $(MY_OUT_DIR)/ktest.o
	c++ -o $(MY_OUT_DIR)/ktest $(MY_OUT_DIR)/ktest.o $(LDPOSTFLAGS)

$(MY_OUT_DIR)/ktest.o: ktest.cpp
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/ktest.o ktest.cpp

clean:
	rm -f $(MY_OUT_DIR)/ktest.o
