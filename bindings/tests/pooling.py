#!/usr/bin/python

import LibIdmefv2Easy

client = LibIdmefv2Easy.ClientEasy("PoolingTest", LibIdmefv2Easy.Client.IDMEFV2_READ)
client.Start()

while True:
    idmefv2 = LibIdmefv2Easy.IDMEFV2()

    ret = client.RecvIDMEFV2(idmefv2)
    if ret:
	print idmefv2
