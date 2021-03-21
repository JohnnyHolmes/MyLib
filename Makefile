CONFIG_MODULE_SIG=n
ifneq ($(KERNELRELEASE),)
	#MODULE_NAME=hello
	#MODULE_NAME=audit_test_ko
	#$(MODULE_NAME)-objs:=hello.o 
	#$(MODULE_NAME)-objs:=audit_test_ko.o
	#obj-m:=audit_ko_test.o
	obj-m:=lsm_audit.o
	#obj-m:=hello.o
else
	KERNEL_DIR = /lib/modules/`uname -r`/build
	MODULEDIR := $(shell pwd)
 
.PHONY: modules
default: modules
 
modules:
	make -C $(KERNEL_DIR) M=$(MODULEDIR) modules
 
clean distclean:
	rm -f *.o *.mod.c .*.*.cmd *.ko
	rm -rf .tmp_versions
endif
