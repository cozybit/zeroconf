#!/usr/bin/python

import sys
import serial
import time
import re

#extract SSID from scan result line
re_ssid = re.compile( r'\s+\d+\.\s+BSSID\s+\w+\:\w+\:\w+\:\w+\:\w+\:\w+\s+RSSI\s+\d+\s+SSID\s+(\S+)\t' )

class vir8388:
	def __init__( self, port='/dev/ttyS0', rate=57600 ):
		try:	
			self.ser = serial.Serial( port, rate,\
				xonxoff=0,\
				rtscts=0,\
				bytesize=serial.EIGHTBITS,\
				parity=serial.PARITY_NONE,\
				stopbits=serial.STOPBITS_ONE,\
				timeout=1 )
		except serial.SerialException:
			print "error: unable to open serial port",port
			sys.exit(1)
	
	def __del__( self ):
		self.ser.close()

	def clear_buffer( self ):
		c = ' '
		while c != '':
			c = self.ser.read(1)

	# runs 'wilist scan', returns a list of SSIDs found
	def scan( self ):
		results = []
		self.ser.write( "iwlist scan\r" )
		time.sleep( 5 )
		l = self.ser.readline()
		while l != '':
			l = self.ser.readline()
			m = re_ssid.match( l ) 
			if m != None:
				ssid = m.groups()
				results.append( ssid[0] )
		return results

# unit testing
if __name__ == '__main__':
	print "testing 8388 interface..."
	v = vir8388()
#	v.clear_buffer()
	r = v.scan()
	print r
