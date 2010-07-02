import pxssh
import time

class mdns:

	def __init__(self):
		self.session = pxssh.pxssh()
		self.session.login("192.168.1.80", "root")

	def start(self, args=""):
		self.session.sendline("killall /root/mdns")
		self.session.prompt()
		self.session.sendline("/root/mdns " + args + " > /dev/null 2>&1 &")
		self.session.prompt()
		# we have to wait for mdns to start.  This is because it sends several
		# probes for its name before claiming it.
		time.sleep(2)
