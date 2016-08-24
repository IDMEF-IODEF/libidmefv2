#!/usr/bin/env ruby 

require("LibIdmefv2")

LibIdmefv2::LibIdmefv2Log::setCallback(lambda{|level,str|print "log: " + str})
idmefv2 = LibIdmefv2::IDMEFV2.new()

print "*** IDMEFV2->Set() ***\n"
idmefv2.set("alert.classification.text", "My Message")
idmefv2.set("alert.source(0).node.address(0).address", "s0a1")
idmefv2.set("alert.source(0).node.address(1).address", "s0a2")
idmefv2.set("alert.source(1).node.address(0).address", "s1a1")
idmefv2.set("alert.source(1).node.address(1).address", "s1a2")
idmefv2.set("alert.source(1).node.address(2).address", nil)
idmefv2.set("alert.source(1).node.address(3).address", "s1a3")
print idmefv2

print "\n*** Value IDMEFV2->Get() ***\n"
print idmefv2.get("alert.classification.text")

print "\n\n*** Listed Value IDMEFV2->Get() ***\n"
print idmefv2.get("alert.source(*).node.address(*).address")

print "\n\n*** Object IDMEFV2->Get() ***\n"
print idmefv2.get("alert.source(0).node.address(0)")

print "\n\n*** Listed Object IDMEFV2->Get() ***\n"
print idmefv2.get("alert.source(*).node.address(*)")
print "\n\n"

fd = File.new("foo.bin", "w")
idmefv2 >> fd
#idmefv2.Write(fd)
fd.close()

print "\n*** IDMEFV2->Read() ***\n"
fd2 = File.new("foo.bin", "r")
idmefv22 = LibIdmefv2::IDMEFV2.new()
while true do
	begin
		idmefv22 << fd2
		print idmefv22
	rescue EOFError
                print "Got EOF\n"
		break
	end
end
fd2.close()

fd2 = File.new("foo.bin", "r")
idmefv22 = LibIdmefv2::IDMEFV2.new()
while idmefv22.read(fd2) > 0 do
	print idmefv22
end
fd2.close()

print "\n*** Client ***\n"
c = LibIdmefv2::ClientEasy.new("libidmefv2-lml")
c.start()

c << idmefv2
