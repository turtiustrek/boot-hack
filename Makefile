CC = arm-none-eabi
ARGS = -march=armv5t -mbig-endian -nostdlib  -ffunction-sections -T linker.lds  -fpic
ELF = boot.elf
TARGET = boot.bin
all:boot.c platform/365W/stubs.S DisplayLibrary/display.c
	$(CC)-gcc $(ARGS)  -o $(ELF)   platform/365W/stubs.S DisplayLibrary/display.c boot.c
	 $(CC)-objcopy -O binary $(ELF) $(TARGET)
clean:
	rm boot.bin
	rm boot.elf
