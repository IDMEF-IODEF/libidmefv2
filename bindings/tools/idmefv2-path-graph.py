#!/usr/bin/python
#
# Graph IDMEFV2 Messages
#


import time
import sys
sys.path.append('.')
sys.path.append('./.libs')

import gvgen

try:
       import LibIdmefv2Easy
except Exception,e:
       print "Import failed: ",e
       print "Try 'cd ./.libs && ln -s libidmefv2_python.so _LibIdmefv2Easy.so'"
       sys.exit(1)


client = LibIdmefv2Easy.Client("PoolingTest")
client.Init()

client.PoolInit("192.168.33.215", 1)

def handle_alert(idmefv2):
       graph = gvgen.GvGen()
       alert_g = graph.newItem("IDMEFV2 Alert")

       value = idmefv2.Get("alert.classification.text")
       if value:
              act = graph.newItem("alert.classification.text", None, distinct=1)
              actc = graph.newItem(value, act, distinct=1)
              graph.newLink(alert_g, actc) 

       value = idmefv2.Get("alert.assessment.impact.description")
       if value:
              aaid = graph.newItem("alert.assessment.impact.description", None, distinct=1)
              aaidc = graph.newItem(value, aaid, distinct=1)
              graph.newLink(alert_g, aaidc)

       value = idmefv2.Get("alert.assessment.impact.completion")
       if value:
              aaic = graph.newItem("alert.assessment.impact.completion", None, distinct=1)
              aaicc = graph.newItem(value, aaic, distinct=1)
              graph.newLink(alert_g, aaicc)

       graph.dot()


while 1:
       idmefv2 = client.ReadIDMEFV2(1)
       if idmefv2:
              handle_alert(idmefv2)

       time.sleep(2)

