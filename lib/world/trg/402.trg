#40200
Castle Guard protecting the Castle~
0 gk 100
~
if %actor.vnum% == 40200
  wait 1
  say For Prince Phillip!
  wait 1
  mkill goon
~
#40201
Fire Breath Function~
0 k 100
~
set current_hp %actor.hitp%
eval low (%self.hitp% / 9)
eval high %self.hitp% / 5
eval range %high% - %low%
eval dmg %%random.%range%%% + %low%
eval dmgpc (%dmg% * 100) / %current_hp%
set spellname fire breath
if %dmgpc% == 0
  set vp misses
elseif %dmgpc% <= 4
  set vp scratches
elseif %dmgpc% <= 8
  set vp grazes
elseif %dmgpc% <= 12
  set vp hits
elseif %dmgpc% <= 16
  set vp injures
elseif %dmgpc% <= 20
  set vp wounds
elseif %dmgpc% <= 24
  set vp mauls
elseif %dmgpc% <= 28
  set vp decimates
elseif %dmgpc% <= 32
  set vp devastates
elseif %dmgpc% <= 36
  set vp maims
elseif %dmgpc% <= 40
  set vp MUTILATES
elseif %dmgpc% <= 44
  set vp DISEMBOWELS
elseif %dmgpc% <= 48
  set vp EVISCERATES
elseif %dmgpc% <= 52
  set vp MASSACRES
elseif %dmgpc% <= 100
  set vp DEMOLISHES
else
  set vp ANNIHILATES
end
%echoaround% %actor% %self.name% breathes forth a cone of fire.
%send% %actor% %self.name% breathes a cone of hot fire over you!
if %actor.vnum% == 40202
  %echoaround% %actor% The fire hits Prince Phillip's Shield of Virtue
else
  %send% %actor% %self.name%'s %spellname% %vp% you!
  %echoaround% %actor% %self.name%'s %spellname% %vp% %actor.name%!
  %damage% %actor% %dmg%
end
~
#40202
Prince Phillip fights the Dragon~
0 gk 100
~
if %actor.vnum% == 40203
  kill dragon
end
~
#40203
Dragon fights Phillip~
0 gk 100
~
if %actor.vnum% == 40202
  wait 1
  say No, it cannot be!
  wait 1
  say Now, you shall deal with ME, O Prince...
  wait 1
  say and all the powers of HELL.
  wait 1
  mkill phillip
  wait 2
  %echoaround% %actor% Prince Phillip lifts his sword and hurls the Sword of Truth into Maleficent. 
  %asound% Noooooooo! 
  %damage% dragon 500
end
~
#40204
load Prince Phillip~
0 n 100
~
%load% obj 40201
wear shield
%load% obj 40202
wield sword
~
$~
