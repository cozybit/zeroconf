import socket

def inject(q, where, port=5353, source=None, source_port=0):
	pkt = q.to_wire()
	destination = (where, port)
	if source is not None:
		source = (source, source_port)
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
	try:
		s.sendto(pkt, destination)
	finally:
		s.close()

