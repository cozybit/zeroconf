#!/usr/bin/python

import dns
from dns import message
from dns import query

class mdns:

	def __init__( self ):
		pass

	def query( self, name, src_ip ):
		m = message.make_query( name, 1, 1 )
		try:
			query.udp( m, '224.0.0.251', port=5353, timeout=20, 
				source_port=5353, source=src_ip )
		except dns.exception.Timeout:
			return False
		return True
