obj-m += hwmod.o

KDIR ?= $(KERNEL_DIR)/lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	@echo "Building with KDIR=$(KDIR)"
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
