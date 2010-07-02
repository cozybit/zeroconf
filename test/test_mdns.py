import unittest
import dns.query, dns.message
import mdns_subject

mdns = mdns_subject.mdns()

def test_SimpleNameQuery():
	# launch mdns
	mdns.start("")

	q = dns.message.make_query("http-andrey.local", 1, 1)
	try:
		r = dns.query.udp(q, '224.0.0.251', port=5353, timeout=5)
		assert r.id == q.id
		assert r.id == q.id
		assert dns.opcode.from_flags(r.flags) == dns.opcode.QUERY
		assert dns.rcode.from_flags(r.flags, r.ednsflags) == dns.rcode.NOERROR

		# expect the response to contain a single A record with the IP address
		assert len(r.answer) == 1
		assert r.answer[0].name.__str__() == "http-andrey.local."
		assert r.answer[0].rdclass == dns.rdataclass.IN
		assert r.answer[0].rdtype == dns.rdatatype.A
		assert len(r.answer[0]) == 1
		assert r.answer[0][0].__str__() == "192.168.1.80"

	except dns.exception.Timeout:
		assert 0
