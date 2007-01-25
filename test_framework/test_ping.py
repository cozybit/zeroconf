# simple ping test.  All this test does is call upon the challenger to associate
# with the MANAGED network and ping the gateway.  This is really just to test
# the framework.  It will probably change in functionality.

from zc_base import test_base

class test_ping(test_base):

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		challenger.set_ip()
		if challenger.ping(conf.MANAGED_GW) == False:
			return "Failed to ping " + conf.MANAGED_GW
		else:
			return ""

