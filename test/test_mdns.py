import unittest, sys, os, ConfigParser
import dns.query, dns.message
import mdns_subject

# Parse config file
configfile = "test.conf"
if not os.path.exists(configfile):
	print "config file " + configfile + " does not exist."
	assert 0
conf = ConfigParser.ConfigParser()
conf.read(configfile)
ipaddr = conf.get("target", "ipaddr")

# create the mdns device to test
mdns = mdns_subject.mdns(conf)

class mdnsTest(unittest.TestCase):

	def expectEqual(self, a, b):
		self.failIf(a != b, "Expected " + a + " but got " + b)

	def compareQandA(self, q, r):
		assert r.id == q.id
		assert r.id == q.id
		assert dns.opcode.from_flags(r.flags) == dns.opcode.QUERY
		assert dns.rcode.from_flags(r.flags, r.ednsflags) == dns.rcode.NOERROR

		# expect the response to contain a single A record with the IP address
		assert len(r.answer) == 1
		self.expectEqual(q.question[0].name.__str__(), r.answer[0].name.__str__())
		assert r.answer[0].rdclass == dns.rdataclass.IN
		assert r.answer[0].rdtype == dns.rdatatype.A
		assert len(r.answer[0]) == 1
		self.expectEqual(ipaddr, r.answer[0][0].__str__())

	def tearDown(self):
		mdns.stop()

	def test_StartStop(self):
		self.failIf(mdns.start("-b " + ipaddr) != 0, "Failed to launch mdns")
		mdns.stop()
		self.failIf(mdns.start("-b " + ipaddr) != 0, "Failed to launch mdns")

	def test_StartStart(self):
		self.failIf(mdns.start("-b " + ipaddr) != 0, "Failed to launch mdns")
		self.failIf(mdns.start("-b " + ipaddr) == 0, "Started mdns twice!")

	def test_SimpleNameQuery(self):
		# launch mdns
		self.failIf(mdns.start("-b " + ipaddr + " -n http-andrey") != 0,
					"Failed to launch mdns")

		q = dns.message.make_query("http-andrey.local", 1, 1)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
			self.compareQandA(q, r)

		except dns.exception.Timeout:
			assert 0

	def test_DifferentDomain(self):
		self.failIf(mdns.start("-b " + ipaddr + " -n testme -d foobar") != 0,
					"Failed to launch mdns")

		q = dns.message.make_query("testme.foobar", 1, 1)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
			self.compareQandA(q, r)

		except dns.exception.Timeout:
			assert 0

	def test_InvalidLaunch(self):
		self.failIf(mdns.start("-b " + ipaddr + " -d foo.") != 1,
					"Failed to detect invalid input for -d")
		self.failIf(mdns.start("-b " + ipaddr + " -n foo.") != 1,
					"Failed to detect invalid input for -n")

	def test_WrongNameQuery(self):
		# launch mdns
		self.failIf(mdns.start("-b " + ipaddr + " -n http-andrey") != 0,
					"Failed to launch mdns")

		q = dns.message.make_query("foobar-doesnt-exist.local", 1, 1)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=2)
			self.failIf(1, "Got a response for a non-existent hostname")

		except dns.exception.Timeout:
			pass
