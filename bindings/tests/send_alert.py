#!/usr/bin/python

import sys
import LibIdmefv2Easy

idmefv2 = LibIdmefv2Easy.IDMEFV2()
idmefv2.Set("alert.classification.text", "Bar")

client = LibIdmefv2Easy.ClientEasy("MyTest")
client << idmefv2

