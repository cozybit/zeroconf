from zc_base import test_base

class test_mdns_query(test_base):

	def __init__(self):
		pass

	def run(self, challenger, subject, conf):
		#challenger.set_wifi(ssid=conf.MANAGED_SSID, mode="managed")
		ip = conf.MANAGED_CHALLENGER_IP
		nm = conf.MANAGED_NETMASK
		gw = conf.MANAGED_GW
		#challenger.set_ip(ip, nm, gw)
		return challenger.send_mdns_query( "http-8388V" )
