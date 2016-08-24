#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import idmefv2

idmefv2 = idmefv2.IDMEFV2()

print("*** IDMEFV2->Set() ***")
idmefv2.set("alert.classification.text", "My Message")
idmefv2.set("alert.analyzer(0).node.interface(0).address(0).address", "s0a0")
idmefv2.set("alert.analyzer(0).node.interface(0).address(1).address", "s0a1")
idmefv2.set("alert.analyzer(1).node.interface(0).address(0).address", "s1a0")
idmefv2.set("alert.analyzer(1).node.interface(0).address(1).address", "s1a1")
idmefv2.set("alert.analyzer(1).node.interface(0).address(2).address", None)
idmefv2.set("alert.analyzer(1).node.interface(0).address(3).address", "s1a3")

print(idmefv2)

print("\n*** Value IDMEFV2->Get() ***")
print(idmefv2.get("alert.classification.text"))
print ("\n*** Listed Value IDMEFV2->Get() ***")
print (idmefv2.get("alert.analyzer(*).node.interface(0).address(*).address"))

print ("\n*** Object IDMEFV2->Get() ***")
print (idmefv2.get("alert.analyzer(0).node.interface(0).address(0)"))

print ("\n*** Listed Object IDMEFV2->Get() ***")
print (idmefv2.get("alert.analyzer(*).node.interface(0).address(*)"))

