# Простой Makefile для NixOS + kernel modules
obj-m += hwmod.o

# Берём KERNEL_DIR из окружения nix-shell
KDIR = $(KERNEL_DIR)
PWD  = $(shell pwd)

all:
	@echo "Building with KDIR=$(KDIR)"
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

