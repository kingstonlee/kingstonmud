#40030
Pin Trader Trades~
0 j 100
~
if %object.vnum% >= 40030 && %object.vnum% <= 40039
  wait 1 s
  %purge% %object%
  eval roll %random.10%
  eval pinnum 40029 + %roll%
  %load% obj %pinnum%
  say A wonderful trade! Here's one for your collection.
  give pin %actor.name%
  emote pins your old one to the display board behind the counter.
else
  say I only trade Disney pins, friend -- here, you keep this.
  give %object.name% %actor.name%
end
~
#40031
Pin Trader Greets~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  say Welcome to Disney's Pin Traders!
  say Hand me any pin and I'll trade you one from the collection -- that's
  say the rule! New here? Grab a lanyard and buy a starter pin from the cart.
end
~
$~
