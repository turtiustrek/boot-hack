# boot-hack
read the [docs](https://turtiustrek.github.io/boot-hack/) for context 

This program enables a common samsung laser printer to use an OLED display.

This program does not contain any code from the samsung firmware itself.
I dont recommend running this at this stage because it has not been tested for other printers.

##### To compile:
```shell
    make
```
This will produce the file named boot.bin which has no elf parts and simply raw instructions. 
##### Uploading & Running
This needs a serial device connected on to the DEBUG port on the printer and then booted into the bootloader(pROBE+>).
Executing FILLMEM.py would simply put the code in memory (defualt is 0x4000DEE0).
```shell
python3 FILLMEM.py
```
After uploading the code can be ran by the following command in the pROBE+> shell. 
```shell
go 4000DEE0
```
Currently this is not persistent and it will not run again after a reboot(currently working on a signed version). 
This should not be run on anything else unless the firmware version is V3.00.01.14F.

![](docs/further-analysis-of-a-samsung-c410w-365w-laser-printer/images/IMG_3830-1024x683.jpg)
