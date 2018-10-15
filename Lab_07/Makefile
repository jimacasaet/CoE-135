ifneq ($(KERNELRELEASE),)
   obj-m := proc_read_3_10.o
else

KERNELDIR ?= /lib/modules/4.15.0-36-generic/build
KERNELDIR ?= /home/john/Documents/CoE\ 135/Lab\ 07/

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
