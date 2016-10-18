#
# Makefile for the mbroe lock device driver
#

#obj-$(CONFIG_SPINLOCK_DEVICE) += lock_module.o
obj-m += rtds-bug.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
