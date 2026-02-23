MODULE_DIR = test-module
USERSPACE_DIR = test-userspace

KDIR ?= $(KERNEL_DIR)/lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := $(MODULE_DIR)/test_module.o

LOADER = loader
LOADER_SRC = $(USERSPACE_DIR)/loader.c

.PHONY: all clean load unload loader kernel full install help

all: kernel loader

kernel: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	@echo "test_module.ko built"

loader: $(LOADER)

$(LOADER): $(LOADER_SRC)
	gcc -Wall -Wextra -std=c99 -O2 -o $(LOADER) $(LOADER_SRC)
	chmod +x $(LOADER)
	@echo "$(LOADER) compiled successfully"
	@echo "Usage: ./$(LOADER) test.log 5"

load:
	sudo insmod $(MODULE_DIR)/test_module.ko log_file_path=test.log period=5

unload:
	sudo rmmod test_module

full:
	sudo mkdir -p /var/tmp/test_module 2>/dev/null || true
	$(MAKE) all
	sudo $(MAKE) load
	@echo "Full setup complete!"
	@echo "Check: tail -f /var/tmp/test_module/test.log"

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean 2>/dev/null || true
	rm -f $(LOADER) *.o *.ko *.mod.c *.mod.o *.symvers *.order
	cd $(MODULE_DIR) && rm -f *.o *.ko *.mod.c *.mod.o *.symvers *.order 2>/dev/null || true
	@echo "Everything cleaned"

install: all

help:
	@echo "Available targets:"
	@echo "  all      — build kernel module + loader"
	@echo "  kernel   — build only test_module.ko"
	@echo "  loader   — build only loader"
	@echo "  full     — mkdir + build + load"
	@echo "  load     — sudo insmod test_module.ko"
	@echo "  unload   — sudo rmmod test_module"
	@echo "  clean    — clean everything"
	@echo "  help     — show this help"
