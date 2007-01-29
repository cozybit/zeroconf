# simple link local addressing test.  The challenger brings up an ad-hoc
# network.  Next, the subject to joins the network and gets a link-local
# address.  If the challenger can ping the subject, the test passes.

import time
from zc_base import test_base

class test_ll_simple(test_base):

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		challenger.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")
		ip = conf.ADHOC_CHALLENGER_IP
		nm = conf.ADHOC_NETMASK
		gw = conf.ADHOC_GW
		challenger.set_ip(ip, nm, gw)		
		subject.set_wifi(ssid=conf.ADHOC_SSID, mode="ad-hoc")
		subject.start_ipv4ll()
		time.sleep(2) # let the link-local take its course
		[ip, nm, gw] = subject.get_ip()
		if challenger.ping(ip) == False:
			return "Failed to ping " + ip
		else:
			return ""

