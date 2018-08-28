obj-m=device_manager.o
all:
	make -C /lib/modules/4.18.5new/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/4.18.5new/build/ M=$(PWD) clean