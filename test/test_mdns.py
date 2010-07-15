import unittest, sys, os, ConfigParser, time
import dns.query, dns.message
import mdns_subject
import mdns_tool

# Parse config file
configfile = "test.conf"
if not os.path.exists(configfile):
	print "config file " + configfile + " does not exist."
	assert 0
conf = ConfigParser.ConfigParser()
conf.read(configfile)
ipaddr = conf.get("target", "ipaddr")

# create the mdns unit under test
uut = mdns_subject.mdns_subject(conf)

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
		assert r.answer[0].rdclass == dns.rdataclass.FLUSH
		assert r.answer[0].rdtype == dns.rdatatype.A
		assert len(r.answer[0]) == 1
		self.expectEqual(ipaddr, r.answer[0][0].__str__())

	def queryAndVerify(self, q):
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=3)
			self.compareQandA(q, r)
		except dns.exception.Timeout:
			self.failIf(1, "Timed out waiting for mdns response.")

	def setUp(self):
		uut.stop()

	def test_StartStop(self):
		ret = uut.start_and_wait("-b " + ipaddr)
		self.failIf(ret != 0, "Failed to launch mdns")
		uut.stop()
		ret = uut.start_and_wait("-b " + ipaddr)
		self.failIf(ret != 0, "Failed to launch mdns")

	def test_StartStart(self):
		ret = uut.start_and_wait("-b " + ipaddr)
		self.failIf(ret != 0, "Failed to launch mdns")
		ret = uut.start_and_wait("-b " + ipaddr)
		self.failIf(ret == 0, "Started mdns twice!")

	def test_SimpleNameQuery(self):
		# launch mdns
		ret = uut.start_and_wait("-b " + ipaddr + " -n http-andrey")
		self.failIf(ret != 0, "Failed to launch mdns")

		q = dns.message.make_query("http-andrey.local", dns.rdatatype.A,
								   dns.rdataclass.IN)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
			self.compareQandA(q, r)

		except dns.exception.Timeout:
			assert 0

	def test_DifferentDomain(self):
		ret = uut.start_and_wait("-b " + ipaddr + " -n testme -d foobar")
		self.failIf(ret != 0, "Failed to launch mdns")

		q = dns.message.make_query("testme.foobar", dns.rdatatype.A,
								   dns.rdataclass.IN)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
			self.compareQandA(q, r)

		except dns.exception.Timeout:
			assert 0

	def test_InvalidLaunch(self):
		ret = uut.start_and_wait("-b " + ipaddr + " -d foo.")
		self.failIf(ret != 1, "Failed to detect invalid input for -d")
		ret = uut.start_and_wait("-b " + ipaddr + " -n foo.")
		self.failIf(ret != 1, "Failed to detect invalid input for -n")

	def test_WrongNameQuery(self):
		# launch mdns
		ret = uut.start_and_wait("-b " + ipaddr + " -n http-andrey")
		self.failIf(ret != 0, "Failed to launch mdns")

		q = dns.message.make_query("foobar-doesnt-exist.local", dns.rdatatype.A,
								   dns.rdataclass.IN)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=2)
			self.failIf(1, "Got a response for a non-existent hostname")

		except dns.exception.Timeout:
			pass

	def test_Inject(self):
		# Simple test to test that the injector functions
		# launch mdns
		ret = uut.start_and_wait("-b " + ipaddr + " -n inject")
		self.failIf(ret != 0, "Failed to launch mdns")

		q = dns.message.make_query("inject.local", dns.rdatatype.A,
								   dns.rdataclass.IN)
		mdns_tool.inject(q, '224.0.0.251')

	def test_AnswerOneProbe(self):
		ret = uut.start("-b " + ipaddr + " -n foo")
		self.failIf(ret != 0, "Failed to launch mdns")
		q = dns.message.make_query("foo.local", dns.rdatatype.A,
								   dns.rdataclass.IN)
		r = dns.message.make_response(q)
		r.question = []
		r.find_rrset(r.answer, dns.name.Name(["foo", "local", ""]),
					 dns.rdataclass.FLUSH, dns.rdatatype.A,
					 create=True, force_unique=True)
		time.sleep(0.050) # wait for first probe to go out
		mdns_tool.inject(r, '224.0.0.251')
		time.sleep(2) # device should rename itself foo-2
		q = dns.message.make_query("foo-2.local", dns.rdatatype.A,
								   dns.rdataclass.FLUSH)
		self.queryAndVerify(q)
