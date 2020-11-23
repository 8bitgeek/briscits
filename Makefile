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

cleantargets:
	rm -f seeedstudio-gd32.bin
	rm -f seeedstudio-gd32.elf
	rm -f seeedstudio-gd32.map
	rm -f sipeed-longan-nano.bin
	rm -f sipeed-longan-nano.elf
	rm -f sipeed-longan-nano.map
	rm -f sipeed-longan-nano-mutex.bin
	rm -f sipeed-longan-nano-mutex.elf
	rm -f sipeed-longan-nano-mutex.map

clean: cleantargets
	make -f bsp/sipeed-longan-nano-mutex/Makefile clean
	make -f bsp/sipeed-longan-nano/Makefile clean
	make -f bsp/seeedstudio-gd32/Makefile clean

distclean: cleantargets
	make -f bsp/sipeed-longan-nano-mutex/Makefile cleanall
	make -f bsp/sipeed-longan-nano/Makefile cleanall
	make -f bsp/seeedstudio-gd32/Makefile cleanall
