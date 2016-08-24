#!/usr/bin/python

import os
import sys
import idmefv2


##################
# this program will save all alerts to the "alerts" directory.
#
# known bu^Wfeatures: directory must exist, and old alerts are overwritten if present
##################

i = 0
dest_dir = "alerts"
if len(sys.argv) > 1:
	dest_dir = sys.argv[1]


if os.path.exists(dest_dir) == 0:
	print "dir ",dest_dir," does not exist"
	sys.exit(1)


client = LibIdmefv2Easy.ClientEasy("PolluxTest", LibIdmefv2Easy.Client.IDMEFV2_READ)
client.SetFlags(client.GetFlags() & ~LibIdmefv2Easy.Client.HEARTBEAT)
#client.Init()
client.Start()

#ret = client.PoolInit("192.168.33.215", 1)
#print "PoolInit =>",ret


def handle_alert(idmefv2):
	global i
	try:
		print idmefv2
		i += 1
		f = open( "%s/%d.idmefv2" % (dest_dir,i), "w" )
		idmefv2 >> f
		f.close()
		print i
	except RuntimeError,_e:
		print "An exception occured: ",_e


import time

while 1:
	sys.stdout.write(".")
	sys.stdout.flush()
	idmefv2 = LibIdmefv2Easy.IDMEFV2()
	client >> idmefv2
	if idmefv2:
		handle_alert(idmefv2)
	time.sleep(2)
