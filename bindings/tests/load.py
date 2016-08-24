#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIdmefv2Easy
except Exception,e:
	print "Import failed: ",e
	print "Try 'cd ./.libs && ln -s libidmefv2_python.so _LibIdmefv2Easy.so'"
	sys.exit(1)

idmefv2 = LibIdmefv2Easy.IDMEFV2()
idmefv2.ReadFromFile("foo.bin")
idmefv2.PrintToStdout()
