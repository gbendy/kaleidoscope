.PHONY: all clean

MY_OUT_DIR=$(OUT_PATH)/libio

all: $(MY_OUT_DIR) $(MY_OUT_DIR)/libio.a

$(MY_OUT_DIR):
	mkdir -p $(MY_OUT_DIR)

$(MY_OUT_DIR)/libio.a: $(MY_OUT_DIR)/libio.o
	ar r $(MY_OUT_DIR)/libio.a $(MY_OUT_DIR)/libio.o

$(MY_OUT_DIR)/libio.o: libio.cpp
	c++ -c -o $(MY_OUT_DIR)/libio.o libio.cpp

clean:
	rm -f $(MY_OUT_DIR)/libio.o
