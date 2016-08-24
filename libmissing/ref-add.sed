/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ libidmefv2 / libidmefv2 /
  tb
  s/ $/ libidmefv2 /
  :b
  s/^/# Packages using this file:/
}
