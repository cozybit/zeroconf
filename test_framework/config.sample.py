# This is a sample configuration file for the zeroconf test engine.  Copy it to
# config.py and set the variables below to configure it your system.

# Turn debug messages on or off
DEBUG = 0

# This is the name of the python interface to the challenge system.  The
# challenge system will be used to challenge the subject system.  It must
# inherit from zc_base.challenger_base to work.
CHALLENGER_NAME = "challenger_linux"

# This is the name of the python interface to the subject system.  The subject
# system is the one that we are testing.
SUBJECT_NAME = "subject_8388V"

# subject_8388V-specific variables.  Other subjects probably don't use these.
# serial port and rate of the 8388V subject.
PORT_8388V = "/dev/ttyS0"
RATE_8388V = 57600

# These are the network interfaces on the subject and challenger systems.
# These interfaces should be capable of joining the same network (duh).
SUBJECT_INTERFACE = ""
CHALLENGER_INTERFACE = "eth1"

# These are static network parameters that can be assigned to the subject and
# challenger on the MANAGED_SSID.  They may use a DHCP server instead if one is
# available.
MANAGED_SSID = "cozybit"
MANAGED_SUBJECT_IP = "192.168.1.40"
MANAGED_CHALLENGER_IP = "192.168.1.41"
MANAGED_NETMASK = "255.255.255.0"
MANAGED_GW = "192.168.1.1"

# This is the SSID of an ad-hoc network that the challenger or subject can
# create and/or join.
ADHOC_SSID = "brian-test"

