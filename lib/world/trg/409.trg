#40900
DJ R-3X Random Banter~
0 b 20
~
switch %random.4%
  case 1
    mecho @cDJ R-3X@n cranks up the volume and crows, 'Welcome to Oga's, spacers!'
    break
  case 2
    mecho @cDJ R-3X@n bleeps, 'Keep those feet moving - this is a Batuu beat!'
    break
  case 3
    mecho @cDJ R-3X@n spins a thumping track from the Outer Rim.
    break
  case 4
    mecho @cDJ R-3X@n whirrs happily, 'R-3X in the cantina! Yeah!'
    break
done
~
#40901
Stormtrooper Patrol Chatter~
0 b 15
~
switch %random.3%
  case 1
    say Move along. This area is under First Order control.
    break
  case 2
    emote scans the crowd for any sign of the Resistance.
    break
  case 3
    say Report all suspicious activity to the nearest officer.
    break
done
~
#40902
Oga Garra Greets Newcomers~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  say So. Another face looking for trouble in my cantina.
  emote narrows her eyes at %actor.name%.
end
~
#40903
Gatherer Speaks of Lightsabers~
0 d 1
lightsaber kyber saber crystal jedi~
say The kyber calls to those who listen.
say Bring me proof of your skill, and I will help you build a blade of your own.
emote gestures toward a workbench strewn with crystals and hilt parts.
~
#40904
Cantina Cheers Command~
2 c 100
cheers~
%send% %actor% You raise a mug and shout a toast to the Outer Rim!
%echoaround% %actor% %actor.name% raises a mug and shouts a toast to the Outer Rim!
~
$~
