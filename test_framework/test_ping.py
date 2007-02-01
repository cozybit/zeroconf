# simple ping test.  All this test does is call upon the challenger and subject
# to associate with the MANAGED network, and have the challenger ping the
# subject.  This is really just to test the framework.

import time
from zc_base import test_base

class test_ping(test_base):

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		challenger.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")
		ip = conf.ADHOC_CHALLENGER_IP
		nm = conf.ADHOC_NETMASK
		gw = conf.ADHOC_GW
		challenger.set_ip(ip, nm, gw)		
		subject.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")
		ip = conf.ADHOC_SUBJECT_IP
		subject.set_ip(ip, nm, gw)		
		if challenger.ping(conf.ADHOC_SUBJECT_IP) == False:
			return "Failed to ping " + conf.ADHOC_SUBJECT_IP
		else:
			return ""

