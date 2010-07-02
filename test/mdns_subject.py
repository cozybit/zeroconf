import pxssh
import time

# This is an abstraction layer for launching the mdns on a remote target under
# test.  At this time it is just hard coded to launch mdns on a linux host over
# ssh.  In the future, we should query the config variable targettype from the
# test config.
class mdns:

	def __init__(self, conf):
		self.session = pxssh.pxssh()
		self.session.login(conf.get("target", "ipaddr"), "root")
		self.conf = conf

	def start(self, args=""):
		command = "mdns " + args + " launch"
		self.session.sendline(command)
		self.session.prompt()
		self.session.sendline("echo $?")
		self.session.prompt()
		ret = int(self.session.before.split("\n")[-2])
		if ret != 0:
			return ret

		# we have to wait for mdns to start.  This is because it sends several
		# probes for its name before claiming it.
		time.sleep(2)
		return 0

	def stop(self, args=""):
		self.session.sendline("mdns halt")
		self.session.prompt()
