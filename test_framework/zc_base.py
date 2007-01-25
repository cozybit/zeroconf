##############################################################################
# This module contains all of the base classes for the zeroconf test framework.
# These are meant to be abstract base classes, so all of the methods return
# failure.  It is expected that classes that extend these base classes will
# override all methods in these classes.  Because we return failures in all
# methods of these base classes, if a sub class fails to implement one of the
# methods, tests that call that method will fail.

import sys
import config

##############################################################################
# zc_test_exception: Base exception for all Zeroconf testing exceptions
class zc_test_exception(Exception):
	def __init__(self, value):
		self.value = value

	def __str__(self):
		return repr(self.value)


##############################################################################
# challenger_base: Base class for systems that act as challengers.
#
# set_ip(ip, netmask, gateway) -- sets up system's TCP/IP stack.  If any
#                                 arguments are empty strings, function should
#                                 use the MANAGED_* values from config.py
#
# get_ip() -- returns a list of decimal-dotted ip addresses representing the ip
#             address, netmask, and gateway of the system.  If tcp/ip is not
#             properly setup, return empty list.
#
# ping(ip) -- Briefly ping ip.  ip is in decimal-dotted notation.  Return True
#             if a response was heard, False otherwise.

class challenger_base:
	def __init__( self, conf ):
		return

	def set_ip( self, ip="", netmask="", gateway="" ):
		raise zc_test_exception, "set_ip function unimplemented."

	def get_ip( self ):
		raise zc_test_exception, "get_ip function unimplemented."

	def ping( self, ip ):
		return False
		
	def __del__( self ):
		return

##############################################################################
# subject_base: Base class for all Zeroconf systems that are to be tested.
#
# start_ipv4ll() -- starts system's ipv4 link-local address process
#
# stop_ipv4ll() -- stops system's ipv4 link-local address process
#
# set_ip(ip, netmask, gateway) -- sets up system's TCP/IP stack.  If any
#                                 arguments are empty strings, function should
#                                 use the MANAGED_* values from config.py
#
# get_ip() -- returns a list of decimal-dotted ip addresses representing the ip
#             address, netmask, and gateway of the system.  If tcp/ip is not
#             properly setup, return empty list.
#
# ping(ip) -- Briefly ping ip.  ip is in decimal-dotted notation.  Return True
#             if a response was heard, False otherwise.

class subject_base:
	def __init__( self ):
		return

	def __del__( self ):
		return

	def start_ipv4ll( self ):
		raise zc_test_exception, "start_ipv4ll function unimplemented."

	def stop_ipv4ll( self ):
		raise zc_test_exception, "stop_ipv4ll function unimplemented."

	def set_ip( self, ip="", netmask="", gateway="" ):
		raise zc_test_exception, "set_ip function unimplemented."

	def get_ip( self ):
		raise zc_test_exception, "get_ip function unimplemented."

	def ping( self, ip):
		return False

##############################################################################
# test_base: Base class for tests
#
# run(challenger, subject, conf): Run the test on subject, using challenger,
# based on conf.  Return an empty string for success, or an error string for
# failure.  Implementations may assume that challenger and subject have been
# initialized with the same conf object.

class test_base:

	def __init__(self):
		return

	def run(self, challenger, subject, conf):
		return "Test Not Implemented"
