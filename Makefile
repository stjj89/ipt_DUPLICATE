MODULES_DIR := /lib/modules/$(shell uname -r)
KERNEL_DIR := ${MODULES_DIR}/build
obj-m += ipt_DUPLICATE.o

all: ipt_DUPLICATE.h ipt_DUPLICATE.c
	cp ./ipt_DUPLICATE.h /usr/src/linux-headers-`uname -r`/include/linux/netfilter_ipv4/
	make -C ${KERNEL_DIR} M=$$PWD;
	make libipt_DUPLICATE.so
install: libipt_DUPLICATE.so ipt_DUPLICATE.ko
	cp ./libipt_DUPLICATE.so /lib/xtables/
	rm -rf /lib/modules/`uname -r`/ipt_DUPLICATE.ko
	ln -s `pwd`/ipt_DUPLICATE.ko /lib/modules/`uname -r`
	depmod -a
	modprobe ipt_DUPLICATE
	rm -rf /lib/modules/`uname -r`/ipt_DUPLICATE.ko
uninstall:
	modprobe -r ipt_DUPLICATE
modules:
	make -C ${KERNEL_DIR} M=$$PWD $@;
modules_install:
	make -C ${KERNEL_DIR} M=$$PWD $@;
libipt_DUPLICATE.so: libipt_DUPLICATE.o
	gcc -shared -fPIC -o $@ $^;
libipt_DUPLICATE.o: libipt_DUPLICATE.c
	gcc -O2 -Wall -D_INIT=lib$*_init -fPIC -c -o $@ $<;
clean:
	make -C ${KERNEL_DIR} M=$$PWD $@;
	rm -rf libipt_DUPLICATE.so libipt_DUPLICATE.o