# link local normal operation test.  This test watches for the proper probes and
# announcements without interfering.  In the unlikely event that the subject
# chooses conf.ADHOC_CHALLENGER_IP as its IP address, this test will fail.

import time
import signal
import os
from zc_base import test_base
from threading import Timer
import socket

import dpkt
from dpkt import arp
import myarp
from myarp import myarp
from myarp import format

class test_ll_normal(test_base):

	def __init__(self):
		return
	
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

		try:
			probe1 = challenger.recv_arp(3)
			probe2 = challenger.recv_arp(3)
			probe3 = challenger.recv_arp(3)
			announce1 = challenger.recv_arp(3)
			announce2 = challenger.recv_arp(3)
			
			mac = self.subject.get_mac()
			probe1.is_probe(mac)
			probe2.is_probe(mac)
			probe3.is_probe(mac)
			announce1.is_announce(mac)
			announce2.is_announce(mac)
			
			subject.stop_ipv4ll()
			return ""

		except socket.timeout:
			subject.stop_ipv4ll()
			return "Challenger failed to collect all arp traffic."

		except format, e:
			subject.stop_ipv4ll()
			return e.value
