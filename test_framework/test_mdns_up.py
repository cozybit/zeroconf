# this brings up the mDNS responder

from zc_base import test_base

class test_mdns_up(test_base):

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		# add 224.0.0.251 to multicast filter in the MAC
		subject.set_mcast()
		# associate
		subject.set_wifi(ssid=conf.MANAGED_SSID, mode="managed")
		# configure and bring up TCP/IP
		subject.set_ip( conf.MANAGED_SUBJECT_IP, conf.MANAGED_NETMASK,\
			conf.MANAGED_GW )
		# bring up mDNS responder
		subject.start_mdns()
		return ""

