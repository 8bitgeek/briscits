
.PHONY: all
all:
	make -f bsp/sipeed-longan-nano/Makefile
	make -f bsp/seeedstudio-gd32/Makefile

clean:
	make -f bsp/sipeed-longan-nano/Makefile clean
	make -f bsp/seeedstudio-gd32/Makefile clean
