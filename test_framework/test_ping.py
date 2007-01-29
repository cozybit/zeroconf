# simple ping test.  All this test does is call upon the challenger and subject
# to associate with the MANAGED network, and have the challenger ping the
# subject.  This is really just to test the framework.

import time
from zc_base import test_base

class test_ping(test_base):

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		challenger.set_wifi(ssid=conf.MANAGED_SSID, mode="managed")
		challenger.set_ip()
		subject.set_wifi(ssid=conf.MANAGED_SSID, mode="managed")
		subject.set_ip()
		if challenger.ping(conf.MANAGED_SUBJECT_IP) == False:
			return "Failed to ping " + conf.MANAGED_GW
		else:
			return ""

