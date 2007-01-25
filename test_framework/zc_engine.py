#!/usr/bin/python
#
# zc_engine.py: This is the zeroconf test engine.  Based on config.py, the test
# engine sets up a challenge system and a subject system, and runs various
# tests.

import os
import sys
import re

#instantiate a test object from test file 
def import_test(name):
	from zc_base import test_base
	mod = __import__(name, globals(), locals(), [])
	t = eval("mod." + name + "()")
	if t.__class__.__bases__[0] != test_base:
		raise ImportError, "Failed to create test " + name
	return t

# Check to see of the config file is available
def load_config():
	try:
		import config
	except ImportError:
		print "No config.py.  Please create it from config.sample.py."
		sys.exit(1)
	return config

# Instantite the subject and challenge systems
def create_challenger(name):
	from zc_base import challenger_base
	mod = __import__(name, globals(), locals(), [])
	challenger = eval("mod." + name + "(conf)")
	if challenger.__class__.__bases__[0] != challenger_base:
		raise ImportError, "Can't create challenger " + name
	return challenger

# This is the actual test engine.  It runs the tests specified on the command
# line.  If no test is specified, it attempts to load and run all files in the
# current directory that are called test_*

test_names = []
if len(sys.argv) > 1:
	# Load all tests
	for arg in sys.argv[1:]:
		if os.path.exists(arg + ".py"):
			test_names.append(arg)
		else:
			print arg + ".py is not a proper file."
			sys.exit(1)
else:
	all_names = os.listdir(".")
	for name in all_names:
		match = re.findall(r'^test_.*\.py$', name)
		if len(match) == 1:
			test_names.append(name.split(".")[0])

conf = load_config()
challenger = create_challenger(conf.CHALLENGER_NAME)
subject = 0 # For now, the subject is not used

for test_name in test_names:
	test = import_test(test_name)
	error = test.run(challenger, subject, conf)
	if(error == ""):
		print "PASS: " + test_name
	else:
		print "FAIL: " + test_name + ": " + error
