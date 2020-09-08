export OUT_PATH=$(CURDIR)/out
export ROOT_PATH=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

TREE=lib/kaleidoscope lib/io bin/ktest bin/kimg

.PHONY: all clean

all:
	$(foreach tree, $(TREE), $(MAKE) -C $(ROOT_PATH)/$(tree) all &&) /bin/true

clean:
	$(foreach tree, $(TREE), $(MAKE) -C $(ROOT_PATH)/$(tree) clean &&) /bin/true
