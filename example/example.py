import idmefv2

msg = idmefv2.IDMEFV2()

msg.set("alert.classification.text", "Test alert")
msg.set("alert.source(0).node.interface(0).address(0).address", "10.0.0.1")

print(msg)
print(msg.toJSON())
print(msg.toBinary())

msg2 = idmefv2.IDMEFV2(msg.toJSON())
print(msg2)
