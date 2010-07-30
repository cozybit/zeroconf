import unittest, sys, os, ConfigParser, time, socket, struct
import dns.query, dns.message
import mdns_subject
import mdns_tool
import time

# Parse config file
configfile = "test.conf"
if not os.path.exists(configfile):
	print "config file " + configfile + " does not exist."
	assert 0
conf = ConfigParser.ConfigParser()
conf.read(configfile)
ipaddr = conf.get("target", "ipaddr")
sniffdev = conf.get("sniffer","dev")

# create the mdns unit under test
uut = mdns_subject.mdns_subject(conf)

class mdnsTest(unittest.TestCase):

	#################### helper functions ####################

	def expectEqual(self, a, b):
		self.failIf(a != b, "Expected " + str(a) + " but got " + str(b))

	# expect a single A record in the response
	def compareARecord(self, q, r):
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

	def sendQuery(self, q):
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=3)
		except dns.exception.Timeout:
			self.failIf(1, "Timed out waiting for mdns response.")
		return r

	def queryAndVerifyARecord(self, q):
		r = self.sendQuery(q)
		self.compareARecord(q, r)

	# ensure that the response r contains a PTR record for the fully qualified
	# service type fqst that points to the SRV with the supplied fully
	# qualified service name fqsn on port.  Also ensure that the SRV record
	# points to the fully qualified domain name fqdn.  If txt is not None, it's
	# an array of expected values in the TXT record.
	def verifySRVPTRRecord(self, r, fqst, fqsn, port, fqdn, txt=None):
		foundPTR = False
		foundSRV = False
		foundTXT = False
		for a in r.answer:
			if a == dns.rrset.from_text(fqst, 255, dns.rdataclass.FLUSH,
										dns.rdatatype.PTR, fqsn):
				foundPTR = True

			rr = dns.rrset.from_text(fqsn, 255, dns.rdataclass.FLUSH,
									 dns.rdatatype.SRV, "0 0 " + str(port) +
									 " " + fqdn)
			if a == rr:
				foundSRV = True

			if txt != None:
				if txt == "":
					rr = dns.rrset.from_text(fqsn, 255, dns.rdataclass.FLUSH,
											 dns.rdatatype.TXT, "\"\"")
				else:
					rr = dns.rrset.from_text(fqsn, 255, dns.rdataclass.FLUSH,
											 dns.rdatatype.TXT, txt)					
				if a == rr:
					foundTXT = True

		self.failIf(foundPTR == False, "Failed to find PTR record for " + fqst)
		self.failIf(foundSRV == False, "Failed to find SRV record for " + fqsn)
		self.failIf(txt != None and foundTXT == False,
					"Failed to find expected TXT record")

	# ensure that an A record is present with ipaddr and fqdn
	def verifyARecord(self, r, ipaddr, fqdn):
		foundA = False
		for a in r.answer:
			rr = dns.rrset.from_text(fqdn, 255, dns.rdataclass.FLUSH,
									 dns.rdatatype.A, str(ipaddr))
			if a == rr:
				foundA = True

		self.failIf(foundA == False, "Failed to find A record for " + fqdn)

	# launch mdns with "args" and wait to see the first probe
	def waitForFirstProbe(self, args):
		# fire up the sniffer
		s = mdns_tool.sniffer()
		s.start(ipaddr, sniffdev)

		# launch mdns
		ret = uut.start(args)
		self.failIf(ret != 0, "Failed to launch mdns")

		# Wait for first probe
		p = None
		try:
			p = s.read(0.250)
		except:
			pass
		self.failIf(p == None, "Failed to find first probe")
		s.stop()

	def getNextPacket(self, s):
		try:
			Pmsg = s.read(0.250)
			return Pmsg
		except:
			self.failIf(1, "Failed to get packet from sniffer")

	def verifyIsQuery(self, Pmsg):
		self.failIf( Pmsg.opcode() != dns.opcode.QUERY,
					 "P was not a QUERY")
		self.failIf( Pmsg.rcode() != dns.rcode.NOERROR,
					 "Expected error code NOERROR")

	def reverseArpaName(self, ip):
		myip = socket.ntohl(struct.unpack('I', socket.inet_aton(ip))[0])
		myip = struct.pack('I', myip)
		myip = socket.inet_ntoa(myip)
		return myip + ".in-addr.arpa."

	def makeBareProbe(self, fqdn):
		bareProbe = dns.message.make_query(fqdn, dns.rdatatype.ANY,
										   dns.rdataclass.IN)
		bareProbe.id = 0
		bareProbe.flags = 0
		arpaQ = dns.rrset.from_text(self.reverseArpaName(ipaddr), 255,
									dns.rdataclass.IN, dns.rdatatype.ANY)
		bareProbe.question.append(arpaQ)
		expectedA = dns.rrset.from_text(fqdn, 255, dns.rdataclass.FLUSH,
										dns.rdatatype.A, ipaddr)
		bareProbe.authority.append(expectedA)
		arpaA = dns.rrset.from_text(self.reverseArpaName(ipaddr), 255,
									dns.rdataclass.FLUSH, dns.rdatatype.PTR,
									fqdn)
		bareProbe.authority.append(arpaA)
		return bareProbe

	#################### unittest functions ####################
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
			self.verifyARecord(r, ipaddr, "http-andrey.local.")

		except dns.exception.Timeout:
			assert 0

	def test_DifferentDomain(self):
		ret = uut.start_and_wait("-b " + ipaddr + " -n testme -d foobar")
		self.failIf(ret != 0, "Failed to launch mdns")

		q = dns.message.make_query("testme.foobar", dns.rdatatype.A,
								   dns.rdataclass.IN)
		try:
			r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
			self.verifyARecord(r, ipaddr, "testme.foobar.")

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

	def test_mdnsProbe(self):
		# Simple test to see that we get the expected probe packets
		# first start sniffer
		name = "testprobe"
		s = mdns_tool.sniffer()
		s.start(ipaddr, sniffdev)

		# launch mdns
		self.failIf(uut.start_and_wait("-b " + ipaddr + " -n " + name) != 0,
			"Failed to launch mdns")

		# Check packets
		expectedProbe = self.makeBareProbe(name + ".local.")

		# P0
		P0msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P0msg)

		# P1
		P1msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P1msg)

		# P2
		P2msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P2msg)

		# Announce
		Amsg = self.getNextPacket(s)
		expectedProbe.question = []
		expectedProbe.answer = expectedProbe.authority
		expectedProbe.authority = []
		expectedProbe.flags = dns.flags.QR | dns.flags.AA
		self.expectEqual(expectedProbe, Amsg)

		# cleanup
		s.stop()

	def test_Goodbye(self):
		# Simple test to see that we get the expected probe packets
		# first start sniffer
		name = "testgoodbye"

		# launch mdns
		self.failIf(uut.start_and_wait("-b " + ipaddr + " -n " + name) != 0,
			"Failed to launch mdns")

		time.sleep(1)

		# Check packets
		s = mdns_tool.sniffer()
		s.start(ipaddr, sniffdev)

		# Stop the mdns service
		uut.stop()

		# Check for goodbye
		Amsg = None
		try:
			Amsg = s.read(0.250)
		except:
			pass

		self.failIf( Amsg == None, "Failed to get A" )
		self.failIf( Amsg.opcode() != dns.opcode.QUERY, "A was not a QUERY")
		self.failIf( Amsg.rcode() != dns.rcode.NOERROR, "A rcode was error")
		self.failIf( Amsg.flags & dns.flags.QR != dns.flags.QR, "QR bit not set")
		self.failIf( Amsg.flags & dns.flags.AA != dns.flags.AA, "AA bit not set")
		self.failIf( str(Amsg.answer).find(name) == -1,
		             "A didn't have correct name")
		# Most importaint test is that ttl is 0:
		self.failIf( Amsg.answer[0].ttl != 0, "TTL of goodbye is not 0")

		# cleanup
		s.stop()

	def test_AnswerOneProbe(self):
		# create a response to a probe
		q = dns.message.make_query("foo.local", dns.rdatatype.A,
								   dns.rdataclass.IN)
		r = dns.message.make_response(q)
		r.question = []
		r.find_rrset(r.answer, dns.name.Name(["foo", "local", ""]),
					 dns.rdataclass.FLUSH, dns.rdatatype.A,
					 create=True, force_unique=True)

		self.waitForFirstProbe("-b " + ipaddr + " -n foo")

		# respond to probe
		mdns_tool.inject(r, '224.0.0.251')
		time.sleep(2) # device should rename itself foo-2
		q = dns.message.make_query("foo-2.local", dns.rdatatype.A,
								   dns.rdataclass.FLUSH)
		self.queryAndVerifyARecord(q)

	def test_SimultaneousGreaterProbe(self):
		p = dns.message.make_query("foo.local", dns.rdatatype.ANY,
								   dns.rdataclass.IN)
		myip = socket.ntohl(struct.unpack('I', socket.inet_aton(ipaddr))[0])
		myip = struct.pack('I', socket.htonl(myip + 1))
		myip = socket.inet_ntoa(myip)
		rr = dns.rrset.from_text("foo.local.", 255, dns.rdataclass.IN,
								 dns.rdatatype.A, myip)
		p.authority = [rr]

		self.waitForFirstProbe("-b " + ipaddr + " -n foo")
		mdns_tool.inject(p, '224.0.0.251')
		time.sleep(2) # device should rename itself foo-2
		q = dns.message.make_query("foo-2.local", dns.rdatatype.A,
								   dns.rdataclass.FLUSH)
		self.queryAndVerifyARecord(q)

	def test_SimultaneousLesserProbe(self):
		p = dns.message.make_query("foo.local", dns.rdatatype.ANY,
								   dns.rdataclass.IN)
		myip = socket.ntohl(struct.unpack('I', socket.inet_aton(ipaddr))[0])
		myip = struct.pack('I', socket.htonl(myip - 1))
		myip = socket.inet_ntoa(myip)
		rr = dns.rrset.from_text("foo.local.", 255, dns.rdataclass.IN,
								 dns.rdatatype.A, myip)
		p.authority = [rr]
		self.waitForFirstProbe("-b " + ipaddr + " -n foo")
		mdns_tool.inject(p, '224.0.0.251')
		time.sleep(2) # device should not rename itself
		q = dns.message.make_query("foo.local", dns.rdatatype.A,
								   dns.rdataclass.FLUSH)
		self.queryAndVerifyARecord(q)

	def test_ServiceParser(self):
		# service args are "name:type:port:proto[:key1=val1:key2=val2]"
		ret = uut.start("-b " + ipaddr + ' -s "my website":http:80:tcp')
		self.expectEqual(0, ret)
		uut.stop()
		ret = uut.start("-b " + ipaddr + ' -s "my website":http:80:tcp -s printer:printer:555:tcp')
		self.expectEqual(0, ret)
		uut.stop()
		ret = uut.start("-b " + ipaddr + ' -s ::::')
		self.expectEqual(-1 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website')
		self.expectEqual(-2 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website:http')
		self.expectEqual(-3 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website:http:999980')
		self.expectEqual(-4 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website:http:80')
		self.expectEqual(-5 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website:http:80:foo')
		self.expectEqual(-6 & 0xff, ret)
		ret = uut.start("-b " + ipaddr + ' -s website:http:80:tcp:u=uname:p=passwd')
		self.expectEqual(0, ret)

	def test_SimpleSRV(self):
		ret = uut.start_and_wait("-b " + ipaddr + " -s mydev:fakeserv:80:tcp")
		self.expectEqual(0, ret)
		q = dns.message.make_query("_fakeserv._tcp.local", dns.rdatatype.PTR,
								   dns.rdataclass.IN)
		r = self.sendQuery(q)
		self.verifySRVPTRRecord(r, "_fakeserv._tcp.local.",
								"mydev._fakeserv._tcp.local.", 80,
								"node.local.")

	def test_MultiSRV(self):
		ret = uut.start_and_wait("-b " + ipaddr + ' -s "my foo":foo:80:tcp -s mybar:bar:555:udp')
		self.expectEqual(0, ret)

		q = dns.message.make_query("_foo._tcp.local", dns.rdatatype.PTR,
								   dns.rdataclass.IN)
		q2 = dns.rrset.from_text("_bar._udp.local.", 0, dns.rdataclass.IN,
								 dns.rdatatype.PTR)
		q.question.append(q2)
		r = self.sendQuery(q)
		self.verifySRVPTRRecord(r, "_foo._tcp.local.",
								"my\ foo._foo._tcp.local.", 80,
								"node.local.")
		self.verifySRVPTRRecord(r, "_bar._udp.local.",
								"mybar._bar._udp.local.", 555,
								"node.local.")

	def test_AnySRV(self):
		ret = uut.start_and_wait("-b " + ipaddr + " -n foo -s myfoo:fooserv:1234:tcp")
		self.expectEqual(0, ret)
		q = dns.message.make_query("_fooserv._tcp.local", dns.rdatatype.ANY,
								   dns.rdataclass.IN)
		r = self.sendQuery(q)
		self.verifySRVPTRRecord(r, "_fooserv._tcp.local.",
								"myfoo._fooserv._tcp.local.", 1234, "foo.local.")

	def test_SRVWithTXT(self):
		ret = uut.start_and_wait("-b " + ipaddr +
			' -s mybar:bar:555:udp:a=b:somevar=someval')
		self.expectEqual(0, ret)

		q = dns.message.make_query("_bar._tcp.local", dns.rdatatype.PTR,
								   dns.rdataclass.IN)
		r = self.sendQuery(q)
		self.verifySRVPTRRecord(r, "_bar._udp.local.",
								"mybar._bar._udp.local.", 555,
								"node.local.", "a=b somevar=someval")

	def test_SRVWithEmptyTXT(self):
		ret = uut.start_and_wait("-b " + ipaddr + ' -s mybaz:baz:555:udp:')
		self.expectEqual(0, ret)

		q = dns.message.make_query("_baz._tcp.local", dns.rdatatype.PTR,
								   dns.rdataclass.IN)
		r = self.sendQuery(q)
		self.verifySRVPTRRecord(r, "_baz._udp.local.",
								"mybaz._baz._udp.local.", 555,
								"node.local.", "")

	def test_ProbeHasSRV(self):
		s = mdns_tool.sniffer()
		s.start(ipaddr, sniffdev)

		# launch mdns
		ret = uut.start_and_wait("-b " + ipaddr + ' -s mybaz:baz:555:udp:tag=val')
		self.expectEqual(0, ret)

		fqsn = "mybaz._baz._udp.local."
		fqdn = "node.local."
		ptrname = "_baz._udp.local."
		expectedProbe = self.makeBareProbe(fqdn)
		srvQ = dns.rrset.from_text(fqsn, 0, dns.rdataclass.IN,
								   dns.rdatatype.SRV)
		srvQ.rdtype = dns.rdatatype.ANY
		expectedProbe.question.append(srvQ)
		expectedSRV = dns.rrset.from_text(fqsn, 255, dns.rdataclass.FLUSH,
										  dns.rdatatype.SRV,
										  "0 0 %d %s" % (555, fqdn))
		expectedProbe.authority.append(expectedSRV)

		# P0
		P0msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P0msg)

		# P1
		P1msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P1msg)

		# P2
		P2msg = self.getNextPacket(s)
		self.expectEqual(expectedProbe, P2msg)

		# To create the announcement, we just move the authority RRs to the
		# answer section and set some flags.  We also need to add some txt
		# records if they exist
		Amsg = self.getNextPacket(s)
		expectedProbe.question = []
		expectedProbe.answer = expectedProbe.authority
		expectedProbe.authority = []
		expectedProbe.flags = dns.flags.QR | dns.flags.AA
		rr = dns.rrset.from_text(ptrname, 255, dns.rdataclass.FLUSH,
								 dns.rdatatype.PTR, fqsn)
		expectedProbe.answer.append(rr)
		rr = dns.rrset.from_text(fqsn, 255, dns.rdataclass.FLUSH,
								 dns.rdatatype.TXT, "tag=val")
		expectedProbe.answer.append(rr)
		self.expectEqual(expectedProbe, Amsg)

		# cleanup
		s.stop()

	def test_PacketSize(self):
		ret = uut.start("-b " + ipaddr + ' -s "my website":http:80:tcp -s printer:printer:555:tcp launch')
		self.expectEqual(0, ret)
		uut.stop()
		crazyname = '0bcdefghijklmnopqrstuvwxyz1bcdefghijklmnopqrstuvwxyz'
		ret = uut.start("-b " + ipaddr +
		                ' -s ' + crazyname + '0:http:80:tcp' +
		                ' -s ' + crazyname + '1:http:8080:tcp' +
		                ' -s ' + crazyname + '2:ssh:22:tcp' +
		                ' -s ' + crazyname + '3:https:443:tcp' +
		                ' -s ' + crazyname + '0:http:81:tcp' +
		                ' -s ' + crazyname + '1:http:8081:tcp' +
		                ' -s ' + crazyname + '2:ssh:221:tcp' +
		                ' -s ' + crazyname + '3:https:4431:tcp' +
		                ' launch')
		self.expectEqual(3, ret)
		uut.stop()
