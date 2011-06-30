# Copyright © 2010-2011, cozybit Inc.
# All rights reserved.
#
# Licensed under the Evaluation License (EVALUATION.LICENSE.txt) that is
# included in this source tree.

import socket
from threading import Thread
import time
import pcap
import struct
import string
import Queue
from impacket import ImpactPacket
import dns.message
import dbus, avahi

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

# dumphex prints out data bytes in hex
def dumphex(s):
    bytes = map(lambda x: '%.2x' % x, map(ord, s))
    for i in xrange(0,len(bytes)/16):
        print '        %s' % string.join(bytes[i*16:(i+1)*16],' ')
    print '        %s' % string.join(bytes[(i+1)*16:],' ')

# Exception returns
class ReadTimeout(Exception):
    "Exception raised by mdns_sniffer.read(): no packet in queue."
    pass

class NotUDP(Exception):
    "Exception raised by mdns_sniffer.read(): mdns packet is not UDP."
    pass

# Packet Queue
# This needs to be global here because capture_packet must be file global due
# to calling restrictions with pylibpcap.
class FlushQueue(Queue.Queue):
	def flush(self):
		self.mutex.acquire()
		self.queue.clear()
		self.mutex.release()

packetQueue = FlushQueue()

def clearPacketQueue():
	packetQueue.flush()

def readPacketQueue(timeout):
	return packetQueue.get(True, timeout)

def writePacketQueue(data):
	try:
		packetQueue.put(data, False)
	except:
		pass

# capture_packet
# This is the callback for pylibpcap's dispatch function.  It gets called for
# every packet captured.  It places the packets into the packetQueue
def capture_packet(pktlen, data, timestamp):
	if not data:
		return

	# print 'got packet len: ' + str(pktlen)
	if data[12:14]=='\x08\x00': # We're only interested in IP packets
		writePacketQueue(data[14:]) # strip the header

# sniffer_thread
# This is an internal class that actually handles the sniffing.
class sniffer_thread(Thread):
	def __init__(self,dev,filter):
		Thread.__init__(self)
		self.done = False;
		self.p = pcap.pcapObject()
		self.net, self.mask = pcap.lookupnet(dev)
		# open_live creates and opens the pcapture interface
		#   arguments are (device, snaplen, promisc, read_timeout_ms)
		#   note:    to_ms does nothing on linux
		#   promisc is 0, we don't open in promiscious mode
		self.p.open_live(dev, 1600, 0, 10)
		self.p.setfilter(filter, 0, 0)
		self.p.setnonblock(1)

	def run(self):
		clearPacketQueue()
		while not self.done:
			self.p.dispatch(1, capture_packet)

	def stop(self):
		self.done = True;

	def read(self, timeout):
		return readPacketQueue(timeout)

	def stats(self):
		print self.p.stats()

# sniffer
# This class is the actual interface to the sniffer.  Basic usage:
# 1. create a sniffer object
# 2. call start()
# 3. call read() to get mdns packets
# 4. call stop()
class sniffer:
	def __init__(self, srcIP, dev='eth0', filter_extra=' and port mdns'):
		self.filter = 'src host ' + srcIP + filter_extra
		self.dev = dev
		self.mySniffer = sniffer_thread(self.dev,self.filter)
		self.mySniffer.start()
		time.sleep(1) # this gives libpcap a moment to get started.
		              # it can miss the first packet if mdns is started
		              # immedately

	def __del__(self):
		self.stop()

	def start(self):
		clearPacketQueue()

	def stop(self):
		self.mySniffer.stop()
		self.mySniffer.join()

	def read_raw(self, timeout):
		try:
			data = self.mySniffer.read(timeout)
		except:
			raise ReadTimeout
		return data

	def read(self, timeout):
		try:
			data = self.mySniffer.read(timeout)
		except:
			raise ReadTimeout
		# so we have data, now process it
		ip_pkt = ImpactPacket.IP(data)
		offset = ip_pkt.get_header_size()
		if not ip_pkt.get_ip_p() == ImpactPacket.UDP.protocol:
			raise NotUDP

		udp_pkt = ImpactPacket.UDP(data[offset:])
		offset += udp_pkt.get_header_size()
		dnsmsg = dns.message.from_wire(data[offset:])
		return dnsmsg

allServices = []

class service:
	"""
	Create an mdns service with the given name, domain, type, and port.  You
	may optionally specify a hostname.  In this case, the hostname.domain will
	be associated with the service.  If you also provide an ipaddr, avahi will
	resolve hostname.domain to ipaddr.  Note that ipaddr must not be a real
	ipaddr on your system, or you'll get a 'Local name collision'.  However, it
	need not be a real live IP address.
	"""
	def __init__(self, name, domain, type, port, hostname="", txt = "",
				 ipaddr = None):
		self.name = name
		self.domain = domain
		self.type = type
		self.port = port
		self.txt = txt
		self.hostname = hostname
		self.ipaddr = ipaddr
		self.fqdn = ""
		if hostname != "":
			self.fqdn = self.hostname + "." + self.domain
		bus = dbus.SystemBus()
		server = dbus.Interface(bus.get_object(avahi.DBUS_NAME,
											   avahi.DBUS_PATH_SERVER),
								avahi.DBUS_INTERFACE_SERVER)
		self.a = dbus.Interface(bus.get_object(avahi.DBUS_NAME,
											   server.EntryGroupNew()),
								avahi.DBUS_INTERFACE_ENTRY_GROUP)

	def publish(self):
		self.a.AddService(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC, dbus.UInt32(0),
						  self.name, self.type, self.domain, self.fqdn,
						  dbus.UInt16(self.port), self.txt)

		if self.ipaddr != None and self.hostname != "":
			self.a.AddAddress(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC,
							  dbus.UInt32(0), self.fqdn, self.ipaddr)

		allServices.append(self)
		self.a.Commit()

	def unpublish(self):
		allServices.remove(self)
		self.a.Reset()

	def __del__(self):
		self.a.Reset()

def unpublishAll():
	for s in allServices:
		s.unpublish()
