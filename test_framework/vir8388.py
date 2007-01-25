#!/usr/bin/python


# vir8388.py -- 8388V serial console interface
#
# This module provides a 'vir8388' object, which is an interface to the 8388V
# serial debug console.  The constructor takes two optional arguments, 'port' and
# 'rate', whose defaults are /dev/ttyS0 and 57600 respectively.
#
# methods:
# clear_buffer() -- reads and ignores anything in the serial input 
#                   buffer, effectively 'flushing' it
#
# scan() -- performs a wireless network scan and returns a list of
#           AP SSIDs found or an empty list if none were found.
#
# set_ssid( ssid ) -- sets the 8388V's SSID
#
# set_tcp( ip, netmask, gateway ) -- sets up TCP

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

	# runs 'iwlist scan', returns a list of SSIDs found
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

	def set_ssid( self, ssid ):
		self.ser.write( "iwconfig essid "+ssid+"\r" )

	def set_mode( self, mode='managed' ):
		if mode != 'managed' and mode != 'ad-hoc':
			return
		self.ser.write( "iwconfig mode "+mode+"\r" )

	def set_tcp( self, ip, netmask, gateway ):
		self.ser.write( "econfig %s %s %s\r" % ( ip, netmask, gateway ) )

# unit testing
if __name__ == '__main__':
	print "testing 8388 interface..."
	v = vir8388()
#	v.clear_buffer()
	r = v.scan()
	if len( r ) == 0:
		print "no SSIDs found, exiting"
		sys.exit( 0 )

	for id in r:
		print id
	id = raw_input( '> ' )
	v.set_ssid( id )
	ip = raw_input( 'ip? ' )
	netmask = raw_input( 'netmask? ' )
	gateway = raw_input( 'gateway? ' )
	v.set_tcp( ip, netmask, gateway )
