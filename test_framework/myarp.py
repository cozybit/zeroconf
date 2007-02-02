# Helper class for arp

import socket
import struct
import dpkt
from dpkt import arp
import string
import myeth

# myarp format exception
class format(Exception):
	def __init__(self, value):
		self.value = value

	def __str__(self):
		return repr(self.value)

class myarp:

	# Constructor takes an arp packet
	def __init__(self, pkt):
		self.pkt = pkt
		self.op = self.pkt.op
		self.sender_mac = myeth.eth_ntoa(self.pkt.sha)
		self.sender_ip = socket.inet_ntoa(self.pkt.spa)
		self.target_mac = myeth.eth_ntoa(self.pkt.tha)
		self.target_ip = socket.inet_ntoa(self.pkt.tpa)
		return

	def is_probe(self, mac):
		# Is this packet a probe from mac?
		mac = mac.split(":")
		mac = "".join(["%s" % (i) for i in mac])
		str = ""
		
		if self.op != arp.ARP_OP_REQUEST:
			str = "Expected arp probe with op=1.  Got op=%d." % (op)
		if self.sender_mac != mac:
			str = "Expected sender mac to be %s but it was %s." % (sender_mac, mac)
		if self.target_mac != "000000000000":
			str = "Expected target mac to be 0 but it was %s." % (target_mac)
		if self.sender_ip != "0.0.0.0":
			str = "Expected sender ip to be 0 but it was %s." % (sender_ip)

		if str != "":
			raise format, str

		return True
	

	def is_announce(self, mac):
		# Is this packet an announcement from mac?
		mac = mac.split(":")
		mac = "".join(["%s" % (i) for i in mac])
		str = ""
		
		if self.op != arp.ARP_OP_REQUEST:
			str = "Expected arp probe with op=1.  Got op=%d." % (op)
		if self.sender_mac != mac:
			str = "Expected sender mac to be %s but it was %s." % (sender_mac, mac)

		if str != "":
			raise format, str

		return True
	
