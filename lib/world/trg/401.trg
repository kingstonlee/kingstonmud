#40100
Room Healing~
2 b 100
~
* By Rumble of The Builder Academy    tbamud.com 9091
* This is required because a random trig does not have an actor.
set actor %random.char%
* only continue if an actor is defined.
if %actor.is_pc%
  * check if they are hurt.
  if %actor.hitp% < %actor.maxhitp% 
    * heal them their level in hitpoints.
    %damage% %actor% -%actor.level%
  end
end
~
$~
