# Helper class for eth

import socket
import struct
import dpkt
import string

ETH_ADDR_UNSPEC = '\x00\x00\x00\x00\x00\x00'
ETH_ADDR_BROADCAST = '\xff\xff\xff\xff\xff\xff'

# eth_ntoa and eth_aton borrowed from:
# http://www.larsen-b.com/Article/206.html
def eth_ntoa(buffer):
	# Convert binary data into a string.
	macaddr = ''
	for intval in struct.unpack('BBBBBB', buffer):
		if intval > 15:
			replacestr = '0x'
		else:
			replacestr = 'x'
		macaddr = ''.join([macaddr, hex(intval).replace(replacestr, '')])
	return macaddr

def eth_aton(buffer):
	buffer = buffer.rstrip("\n")
	addr =''
	temp = string.split(buffer,':')
	buffer = string.join(temp,'')
	# Split up the hex values and pack.
	for i in range(0, len(buffer), 2):
		addr = ''.join([addr,struct.pack('B', int(buffer[i: i + 2], 16))],)
	return addr

