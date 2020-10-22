
.PHONY: all
all: sipeed-longan-nano seeedstudio-gd32
	echo done

sipeed-longan-nano:
	make -f bsp/sipeed-longan-nano/Makefile
	make -f bsp/sipeed-longan-nano/Makefile clean

seeedstudio-gd32:
	make -f bsp/seeedstudio-gd32/Makefile
	make -f bsp/seeedstudio-gd32/Makefile clean


clean:
	make -f bsp/sipeed-longan-nano/Makefile clean
	make -f bsp/seeedstudio-gd32/Makefile clean

distclean:
	make -f bsp/sipeed-longan-nano/Makefile cleanall
	make -f bsp/seeedstudio-gd32/Makefile cleanall

