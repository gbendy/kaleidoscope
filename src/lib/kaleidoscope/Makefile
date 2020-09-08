.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/libkaleidoscope

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/libkaleidoscope.a

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/libkaleidoscope.a: $(MY_OUT_DIR)/libkaleidoscope.o
	ar r $(MY_OUT_DIR)/libkaleidoscope.a $(MY_OUT_DIR)/libkaleidoscope.o

$(MY_OUT_DIR)/libkaleidoscope.o: libkaleidoscope.cpp libkaleidoscope.h
	c++ -c $(CXXFLAGS) -o $(MY_OUT_DIR)/libkaleidoscope.o libkaleidoscope.cpp

clean:
	rm -f $(MY_OUT_DIR)/libkaleidoscope.o
