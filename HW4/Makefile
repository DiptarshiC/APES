#Makefile for process tree
obj-m +=   Kthread2.o Process_tree.o Sample.o Sample1.o


all:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
