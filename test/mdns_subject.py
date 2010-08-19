import pxssh
import time

# This is an abstraction layer for launching the mdns on a remote target under
# test.  At this time it is just hard coded to launch mdns on a linux host over
# ssh.  In the future, we should query the config variable targettype from the
# test config.
class NoSuchTargetType(Exception):
    "Unsupported test target type"
    pass

class mdns_subject:

	# Set DEBUG to True if you want to see the commands and stdout between the
	# subject and the host.
	DEBUG = False

	def __init__(self, conf):
		targettype = conf.get("target", "targettype")
		if targettype != "linuxssh":
			raise NoSuchTargetType
		try:
			self.mdnspath = conf.get("target", "mdnspath")
		except:
			self.mdnspath = 'mdns'
		try:
			self.mdnslog = conf.get("target", "mdnslog")
		except:
			self.mdnslog = '/root/mdns.log'

		self.session = pxssh.pxssh()
		self.session.login(conf.get("target", "ipaddr"), "root")
		self.conf = conf
		self.stop()

	def start(self, args=""):
		command = self.mdnspath + " " + args + " -l " + self.mdnslog + " launch"
		# TODO: Okay.  Here's a mystery.  If the command is 69 chars long,
		# pxssh chokes on whatever it sees over ssh and all subsequent tests
		# fail.  Amazing!  If it's longer, or shorter, everything works fine.
		# But the magic number 69 breaks the command flow.  Why?  Could it be
		# that the prompt "[PEXPECT]# " is 11 chars, and 69 + 11 is 80, and
		# there's a line discipline problem somewhere?  If you figure it out
		# you'll be my hero.
		if len(command) == 69:
			print "Warning: command length is 69, inflating command."
			command = "  " + command

		if self.DEBUG:
			print "Running Command " + command
		self.session.sendline(command)
		self.session.prompt()
		if self.DEBUG:
			print self.session.before
		self.session.sendline("echo $?")
		self.session.prompt()
		if self.DEBUG:
			print self.session.before
		ret = int(self.session.before.split("\n")[-2])
		if ret != 0:
			return ret
		return 0

	def ready(self, timeout=None):
		# After we start mdns, there's still a period of time before it
		# successfully claims its name.  Generally, this is less than 2
		# seconds.  We don't have a way to get the status of the mdns thread,
		# so we just wait.
		time.sleep(2)
		return 0

	def start_and_wait(self, args="", timeout=None):
		ret = self.start(args)
		if ret != 0:
			return ret
		return self.ready(timeout)

	def stop(self, args=""):
		self.session.sendline("mdns halt")
		self.session.prompt()
