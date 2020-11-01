.PHONY: all
all: sipeed-longan-nano \
	 sipeed-longan-nano-mutex \
	 seeedstudio-gd32
	echo done

sipeed-longan-nano-mutex:
	make -f bsp/sipeed-longan-nano-mutex/Makefile

sipeed-longan-nano:
	make -f bsp/sipeed-longan-nano/Makefile

seeedstudio-gd32:
	make -f bsp/seeedstudio-gd32/Makefile

clean:
	make -f bsp/sipeed-longan-nano-mutex/Makefile clean
	make -f bsp/sipeed-longan-nano/Makefile clean
	make -f bsp/seeedstudio-gd32/Makefile clean

distclean:
	make -f bsp/sipeed-longan-nano-mutex/Makefile cleanall
	make -f bsp/sipeed-longan-nano/Makefile cleanall
	make -f bsp/seeedstudio-gd32/Makefile cleanall
