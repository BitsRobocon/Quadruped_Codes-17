import smbus
import time

bus=smbus.SMBus(1)

address=0x18

def write(value):
	bus.write_byte(address,int(value))

def read():
	number=bus.read_byte(address)
	return number

while True:
	var=input("enter")
	if  not var:
		break
	write(var)
	print ("RPI:HI arduino,I sent you",var)
	time.sleep(1)
	number=read()
	print("Recieved",number)



