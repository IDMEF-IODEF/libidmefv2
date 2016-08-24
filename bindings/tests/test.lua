#!/usr/bin/env lua

require("libidmefv2")

function my_cb(level, log)
	io.write("log: " .. log)
end
libidmefv2.LibIdmefv2Log_setCallback(my_cb)


function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif not v then
      print(formatting .. 'NUL')
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))
    else
      print(formatting .. tostring(v))
    end
  end
end


idmefv2 = libidmefv2.IDMEFV2()

print("*** IDMEFV2->Set() ***")
idmefv2:set("alert.classification.text", "My Message")
idmefv2:set("alert.source(0).node.address(0).address", "s0a0")
idmefv2:set("alert.source(0).node.address(1).address", "s0a1")
idmefv2:set("alert.source(1).node.address(0).address", "s1a0")
idmefv2:set("alert.source(1).node.address(1).address", "s1a1")
idmefv2:set("alert.source(1).node.address(2).address", nil)
idmefv2:set("alert.source(1).node.address(3).address", "s1a3")
print(idmefv2)

print("\n*** Value IDMEFV2->Get() ***")
print(idmefv2:get("alert.classification.text"))

print("\n*** Listed Value IDMEFV2->Get() ***")
tprint(idmefv2:get("alert.source(*).node.address(*).address"))

print("\n*** Object IDMEFV2->Get() ***")
print(idmefv2:get("alert.source(0).node.address(0)"))

print("\n*** Listed Object IDMEFV2->Get() ***")
tprint(idmefv2:get("alert.source(*).node.address(*)"))


fd = io.open("foo.bin","w")
idmefv2:write(fd)
fd:close()

print("\n*** IDMEFV2->Read() ***")
fd2 = io.open("foo.bin","r")
idmefv22 = libidmefv2.IDMEFV2()
while idmefv22:read(fd2) do
	print(idmefv22)
end
fd2:close()


print("\n*** Client ***")
c = libidmefv2.ClientEasy("libidmefv2-lml")
c:start()

c:sendIDMEFV2(idmefv2)
