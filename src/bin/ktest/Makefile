.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/ktest

CXXFLAGS=-I../../lib

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/ktest

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/ktest: $(MY_OUT_DIR)/ktest.o
	c++ -o $(MY_OUT_DIR)/ktest $(MY_OUT_DIR)/ktest.o -L $(OUT_PATH)/libkaleidoscope -lkaleidoscope

$(MY_OUT_DIR)/ktest.o: ktest.cpp
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/ktest.o ktest.cpp

clean:
	rm -f $(MY_OUT_DIR)/ktest.o
