# link local addressing conflict test.  The challenger brings up an ad-hoc
# network.  Next, the subject to joins the network and tries to get a link-local
# address.  But when the challenger sees the probe from the subject, it
# responds.  The subject should choose another link-local address.

import time
import signal
import os
from zc_base import test_base
from threading import Timer
import socket
import struct

import dpkt
from dpkt import arp

class test_ll_conflict(test_base):

	def __init__(self):
		return

	# eth_ntoa and eth_aton borrowed from:
	# http://www.larsen-b.com/Article/206.html
	def eth_ntoa(self, buffer):
		# Convert binary data into a string.
		macaddr = ''
		for intval in struct.unpack('BBBBBB', buffer):
			if intval > 15:
				replacestr = '0x'
			else:
				replacestr = 'x'
			macaddr = ''.join([macaddr, hex(intval).replace(replacestr, '')])
		return macaddr

	def eth_aton(self, buffer):
		addr =''
		temp = string.split(buffer,':')
		buffer = string.join(temp,'')
		# Split up the hex values and pack.
		for i in range(0, len(buffer), 2):
			addr = ''.join([addr,struct.pack('B', int(buffer[i: i + 2], 16))],)
		return addr

	def is_probe(self, pkt):
		# Is this packet the expected probe?
		mac = self.subject.get_mac().split(":")
		mac = "".join(["%s" % (i) for i in mac])
		op = pkt.op
		sender_mac = self.eth_ntoa(pkt.sha)
		sender_ip = socket.inet_ntoa(pkt.spa)
		target_mac = self.eth_ntoa(pkt.tha)
		target_ip = socket.inet_ntoa(pkt.tpa)
		str = ""
		if op != arp.ARP_OP_REQUEST:
			str = "Expected arp probe with op=1.  Got op=%d." % (op)
		if sender_mac != mac:
			str = "Expected sender mac to be %s but it was %s." % (sender_mac, mac)
		if target_mac != "000000000000":
			str = "Expected target mac to be 0 but it was %s." % (target_mac)
		if sender_ip != "0.0.0.0":
			str = "Expected sender ip to be 0 but it was %s." % (sender_ip)

		return str
	
	def run(self, challenger, subject, conf):
		self.conf = conf
		self.subject = subject
		
		# set up network
		challenger.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")
		ip = conf.ADHOC_CHALLENGER_IP
		nm = conf.ADHOC_NETMASK
		gw = conf.ADHOC_GW
		challenger.set_ip(ip, nm, gw)		
		subject.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")

		# start challenger listening for arp packets, then launch link-local on
		# subject.  To avoid missing the first arp probe from the subject, these
		# two things must be done in this order.  But because we want to block
		# waiting for arp packets, this is a bit tricky.  The solution is to use
		# a timer to launch the subjects link local.

		t = Timer(1, subject.start_ipv4ll)
		t.start()
		pkt = challenger.recv_arp(3)

		if pkt == "":
			subject.stop_ipv4ll()
			return "Challenger saw no arp probes from subject."
		
		else:
			str = self.is_probe(pkt)
			if str != "":
				return str

			
			subject.stop_ipv4ll()
			return str

