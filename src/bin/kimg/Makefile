.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/kimg
include $(ROOT_PATH)/lib/kaleidoscope/kaleidoscope.mk
include $(ROOT_PATH)/lib/io/io.mk

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/kimg

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/kimg: $(MY_OUT_DIR)/kimg.o $(LINKDEPS)
	c++ -o $(MY_OUT_DIR)/kimg $(MY_OUT_DIR)/kimg.o $(LDPOSTFLAGS)

$(MY_OUT_DIR)/kimg.o: kimg.cpp $(SRCDEPS)
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/kimg.o kimg.cpp

clean:
	rm -f $(MY_OUT_DIR)/kimg.o
