#!/usr/bin/python


# subject_8388V.py -- 8388V serial console interface
#
# This module provides a 'vir8388' object, which is an interface to the 8388V
# serial debug console.  It implements the subject_base class.  The serial port
# parameters come from the values PORT_8388V and RATE_8388V in the conf object
# passed to the constructor.

import sys
import serial
import time
import re
import config
from zc_base import subject_base, zc_test_exception
import thread

#extract SSID from scan result line
re_ssid = re.compile( r'\s+\d+\.\s+BSSID\s+\w+\:\w+\:\w+\:\w+\:\w+\:\w+\s+RSSI\s+\d+\s+SSID\s+(\S+)\t' )

mutex = thread.allocate_lock()

class subject_8388V(subject_base):
	def __init__( self, conf ):
		self.conf = conf
		port = conf.PORT_8388V
		rate = conf.RATE_8388V
		try:
			self.ser = serial.Serial( port, rate,\
				xonxoff=0,\
				rtscts=0,\
				bytesize=serial.EIGHTBITS,\
				parity=serial.PARITY_NONE,\
				stopbits=serial.STOPBITS_ONE,\
				timeout=1 )
			# clear the buffer
			c = ' '
			while c != '':
				c = self.ser.read(1)


		except serial.SerialException:
			print "error: unable to open serial port",port
			sys.exit(1)
	
	def __del__( self ):
		self.ser.close()

	######################################################
	# Helper Functions

	def clear_buffer( self ):
		c = ' '
		while c != '':
			c = self.ser.read(1)

	# send a command over the serial port
	def send( self, cmd ):
		
		mutex.acquire()
		self.ser.write( cmd + "\r")
		# throw away the stuff echoed by the 8388V
		output = ""
		resp = self.ser.readline()
		while resp != "> ":
			output += resp
			resp = self.ser.readline()
		output = output.rstrip("\r\n")
		mutex.release()
		return output
	
	# runs 'iwlist scan', returns a list of SSIDs found
	def scan( self ):
		results = []
		response = self.send( "iwlist scan" )
		responses = response.split("\r\n")
		for l in responses:
			m = re_ssid.match( l ) 
			if m != None:
				ssid = m.groups()
				results.append( ssid[0] )
		return results

	def set_ssid( self, ssid ):
		self.send("iwconfig essid " + ssid)

	def set_mode( self, mode='managed' ):
		if mode != 'managed' and mode != 'ad-hoc':
			return
		self.send("iwconfig mode " + mode)

	######################################################
	# subject_base interface

	def set_wifi( self, ssid="ANY", mode="managed", channel=6 ):
		self.set_mode(mode)
		self.set_ssid(ssid)

	def set_ip( self, ip="", netmask="", gateway="" ):
		if ip == "":
			ip = self.conf.MANAGED_SUBJECT_IP
		if netmask == "":
			netmask = self.conf.MANAGED_NETMASK
		if gateway == "":
			gateway = self.conf.MANAGED_GW
			
		self.send( "econfig %s %s %s" % (ip, netmask, gateway))

		# For some reason, we have to wait for config to take affect.
		time.sleep(2)
	def start_ipv4ll( self ):
		response = self.send("linklocal start")
		if response != "":
			raise zc_test_exception, response
		
	def stop_ipv4ll( self ):
		self.send("linklocal stop")
		
	def get_ip(self):
		return self.send("printip").split(",")
		
	def get_mac(self):
		return self.send("printmac").rstrip("\r\n")

	# add the MAC address for 224.0.0.251 to multicast filter
	def set_mcast(self):
		response = self.send("mcast set")
		if response != "":
			raise zc_test_exception, response

	# read the multicast filter
	def get_mcast(self):
		return self.send("mcast get").rstrip("\r\n")

	# start mDNS responder
	def start_mdns(self):
		response = self.send("mdns start")
		if response != "":
			raise zc_test_exception, response

	# stop mDNS responder
	def stop_mdns(self):
		response = self.send("mdns stop")
		if response != "":
			raise zc_test_exception, response
	
def run_cmd(cmd):
	tokens = cmd.split(" ")
	if len(tokens) == 0:
		return
	
	if tokens[0] == "scan":
		r = v.scan()
		if len( r ) == 0:
			print "No SSIDs found"
		for id in r:
			print id

	elif tokens[0] == "set_wifi":
		v.set_wifi(tokens[1], tokens[2])

	elif tokens[0] == "set_ssid":
		v.set_ssid(tokens[1])

	elif tokens[0] == "set_mode":
		v.set_mode(tokens[1])

	elif tokens[0] == "set_ip":
		if len(tokens) < 4:
			print "Usage: set_ip ip netmask gateway"
			return
		v.set_ip(tokens[1], tokens[2], tokens[3])

	elif tokens[0] == "ping":
		if len(tokens) < 2:
			print "Usage: ping ip"
			return
		v.ping(tokens[1])

	elif tokens[0] == "help":
		print "scan"
		print "set_ssid"
		print "set_mode"

	else:
		print "Unknown command"
				

if __name__ == '__main__':
	v = subject_8388V(config)

	print "Welcome to the 8388V console"
	print "Type \"raw\" for a raw console or enter for the python interface."
	cmd = raw_input("> ")

	try:
		if(cmd == "raw"):
			print "Welcome to the raw console"
			while(1):
				cmd = raw_input("> ")
				if cmd != '':
					print v.send(cmd)
		else:
			print "Welcome to the python console"
			while(1):
				cmd = raw_input("> ")
				run_cmd(cmd)

	except KeyboardInterrupt:
		sys.exit(0)

	sys.exit(0)
