export OUT_PATH=$(CURDIR)/out

TREE=lib bin/ktest

.PHONY: all clean

all:
	$(foreach tree, $(TREE), $(MAKE) -C $(tree) all &&) /bin/true

clean:
	$(foreach tree, $(TREE), $(MAKE) -C $(tree) clean &&) /bin/true
