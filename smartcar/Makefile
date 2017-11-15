all:
	mips-openwrt-linux-gcc ARMserver.c car.c -o server -static
	mips-openwrt-linux-gcc serial_main.c serial_init.c -o serial

scp:
	scp server serial 192.168.1.1:/root/smartcar
remove:
	rm server serial