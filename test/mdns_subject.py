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
	# subject and the host.  Set MANUAL to true if you want to manually start
	# and stop the mdns daemon (e.g., in a debugger).
	def __init__(self, conf, DEBUG=False, MANUAL=False):
		self.DEBUG = DEBUG
		self.MANUAL = MANUAL
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

	def do_command(self, command):
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

	def start(self, args=""):
		# in debug mode, user starts and stops the uut.
		if self.MANUAL == True:
			return 0
		command = self.mdnspath + " " + args + " -l " + self.mdnslog + " launch"
		return self.do_command(command)

	def monitor(self, fqst=""):
		command = self.mdnspath + " monitor " + fqst
		return self.do_command(command)

	def unmonitor(self, fqst=""):
		command = self.mdnspath + " unmonitor " + fqst
		return self.do_command(command)

	def get_results(self):
		# results (if any) are in a tmp file of the form
		# /tmp/servtype.proto.results.
		self.session.sendline("cat /tmp/*.results")
		self.session.prompt()
		return self.session.before.replace("\r", "").split("\n")[1:-1]

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
		# in debug mode, user starts and stops the uut.
		if self.MANUAL == True:
			return
		command = self.mdnspath + " halt"
		self.session.sendline(command)
		self.session.prompt()
		self.session.sendline("rm -f /tmp/*.results")
		self.session.prompt()
