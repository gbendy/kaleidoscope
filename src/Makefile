export OUT_PATH=$(CURDIR)/out
export ROOT_PATH=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

export CXXFLAGS=-fPIC

ifeq ($(DEBUG), 1)
CXXFLAGS+=-g
else
CXXFLAGS+=-O2
endif

TREE=lib/kaleidoscope lib/io bin/ktest bin/kimg bin/kaleid0sc0pe

.PHONY: all clean

all:
	$(foreach tree, $(TREE), $(MAKE) -C $(ROOT_PATH)/$(tree) all &&) /bin/true

clean:
	$(foreach tree, $(TREE), $(MAKE) -C $(ROOT_PATH)/$(tree) clean &&) /bin/true
