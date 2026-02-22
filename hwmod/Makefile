# Простой Makefile для NixOS + kernel modules
obj-m += hwmod.o

# Берём KERNEL_DIR из окружения nix-shell
PWD  = $(shell pwd)

all:
	@echo "Building with KDIR=$(KDIR)"
	$(MAKE) -C $(KERNEL_DIR)/lib/modules/$(shell uname --kernel-release)/build M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR)/lib/modules/$(shell uname --kernel-release)/build M=$(PWD) clean
