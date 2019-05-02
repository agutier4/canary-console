import serial
import struct
import lcm
from xyzLdr import xyzLidar_t
from cmnd import command_t

class SerialHandler:
	def __init__(self, port):
		self.port = port
		self.conn = serial.Serial(port=self.port, baudrate=56700)
		self.received = bytearray(64)
		self.lcm = lcm.LCM()
		self.lcm.subscribe("COMMAND_STREAM",self.lcmHandler)

	def update(self):
		self.lcm.handle_timeout(.1)
		if(self.conn.in_waiting>=64):
			#has full packet
			leadByte = ord(self.conn.read(1))
			if(leadByte == 0xF0):
				self.received[0] = leadByte
				for i in range(63):
					self.received[i+1] =ord(self.conn.read(1))
				if(self.isValid(self.received)):
					self.unpack(self.received)
				else:
					print("invalid")
	def isValid(self,packet):
		_sum = 0x00
		for i in range(64):
			_sum += packet[i]
		return (_sum%256 == 0x00)
	
	def unpack(self,packet):
		print("startPacket")
		numPoints = packet[1] 
		for i in range(numPoints):
			try:
				msg = xyzLidar_t()
				msg.x = struct.unpack(">f",chr(packet[5+i*12])+chr(packet[4+i*12])+chr(packet[3+i*12])+chr(packet[2+i*12]))[0]
				msg.y = struct.unpack(">f",chr(packet[9+i*12])+chr(packet[8+i*12])+chr(packet[7+i*12])+chr(packet[6+i*12]))[0]
				msg.z = struct.unpack(">f",chr(packet[13+i*12])+chr(packet[12+i*12])+chr(packet[11+i*12])+chr(packet[10+i*12]))[0]
				print("valid")
				self.lcm.publish("LIDAR_STREAM",msg.encode())
			except:
				print("Byte Range Error")
	def getChecksum(self,packet):
    		sum = 0
    		for i in range(7):
			sum += packet[i]
    		return (256-(sum % 256))
	
	def lcmHandler(self,channel,data):
		msg = command_t.decode(data)
		packet = bytearray(8)
		packet[0] = 0xF0
		#check command
		packet[1] = msg.state
		if(msg.motor):
			packet[2] = packet[2] | 0x04
			speed = struct.pack('<f',msg.speed)
			packet[3] = ord(speed[0])
			packet[4] = ord(speed[1])
			packet[5] = ord(speed[2])
			packet[6] = ord(speed[3])
		if(msg.scan):
			packet[2] = packet[2] | 0x10
		if(msg.stop):
			packet[2] = packet[2] | 0x40

		packet[7] = self.getChecksum(packet)
		self.conn.write(packet)

if __name__ == "__main__":
	serialHandler = SerialHandler("/dev/cu.SLAB_USBtoUART")
	while(True):
		serialHandler.update()
