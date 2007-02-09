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
import myarp
import myeth
import dpkt
from dpkt import arp

class test_ll_conflict(test_base):

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

		mac = challenger.get_mac()
		arp_p = arp.ARP()
		arp_p.sha = myeth.eth_aton(mac)          # sender hardware addr
		arp_p.tha = myeth.ETH_ADDR_UNSPEC  # dest hardware addr 
		arp_p.op = arp.ARP_OP_REPLY
		
		try:
			probe = challenger.recv_arp(3)
			ip = probe.target_ip
			arp_p.tpa = socket.inet_aton(ip) # ip addr of request
			arp_p.spa = socket.inet_aton(ip) # sender ip addr
		
			# send response to arp probe.
			challenger.send_arp(arp_p, myeth.ETH_ADDR_BROADCAST)

			# now we should see a probe with a new address.
			probe = challenger.recv_arp(3)
			new_ip = probe.target_ip
			subject.stop_ipv4ll()

			if new_ip == ip:
				return "Subject failed to choose new IP after probe response."
			
			return ""

		except socket.timeout:
			subject.stop_ipv4ll()
			return "Challenger failed to collect all arp traffic."

