#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIdmefv2Easy
except:
	print "Import failed"
	print "Try 'cd ./.libs && ln -s libidmefv2_python.so _LibIdmefv2Easy.so'"
	sys.exit(1)

def foo(id):
        print "callback: id = " + str(id)
	idmefv2 = LibIdmefv2Easy._get_IDMEFV2(id)
        idmefv2.PrintToStdout()
        #print bar.Get("alert.classification.text") # XXX not yet implemented
        return 0

LibIdmefv2Easy.set_pymethod(foo)

LibIdmefv2Easy.test_fct()
