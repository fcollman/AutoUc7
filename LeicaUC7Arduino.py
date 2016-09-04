import serial
import struct
import time

class LeicaUC7Arduino(object):
    def __init__(self,port = 'COM8',baudrate = '9600'):
        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = baudrate
        self.ser.STOPBITS = 1
        self.ser.timeout =.5
        self.ser.open()
        time.sleep(1)

    def sendMessage(self,message,debug=False):
        cmd = bytearray(message)
        answer = self.sendDirect(message,len(message))
        if debug:
            values = struct.unpack('B'*len(message),answer)
            print message,values
        return cmd == answer

    def sendDirect(self,cmd,bytesback=1):
        self.ser.write(cmd)
        return self.ser.read(bytesback)

    def sendCmd(self,cmd):
        self.ser.write(cmd)
        answer = self.ser.readline()
        #print 'answer:',answer[:-2]
        return answer[:-2]
    
    def setPreset(self,preset):
        self.sendMessage([1,preset])

    def startCutting(self):
        self.sendMessage([2])

    def stopCutting(self):
        self.sendMessage([3])

    def shutdown(self):
        self.ser.close()