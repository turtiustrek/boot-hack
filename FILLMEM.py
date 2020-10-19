#horrible script to 'upload' the program into the memory
#Note: Rewrite this asap to use the fm.l feature and upload 4 bytes at a time
import binascii
import serial #pyserial
import time
start_addr = 0x4000DEE0 #start location where the binary gets written to.
ser = serial.Serial("/dev/ttyUSB0", 115200)
ser.write(b'\n')
time.sleep(0.5)
with open('boot.bin','rb') as raw: 
     content = raw.read()
num=list(content) #store the bin file as a list.
for i in range(len(num)):
  strs = "fm.b %s 1 %s \n" %(hex((start_addr + i)).rstrip("L").lstrip("0x") or "0",hex(num[i]).rstrip("L").lstrip("0x") or "0")
  ser.write(strs.encode())
  time.sleep(0.01)
  #check = ser.read_until().decode("utf-8")
  print(strs)  
  



    
