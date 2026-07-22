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
#40905
Bounty Hunter Fights~
0 k 12
~
switch %random.2%
  case 1
    say You're worth more to me dead, it seems.
    break
  case 2
    emote fires a bolt from a hidden wrist blaster!
    break
done
~
#40906
Bounty Hunter Death~
0 f 100
~
say Should have... named a higher price...
~
#40907
Dok-Ondar Fights~
0 k 12
~
switch %random.2%
  case 1
    say You will not plunder MY collection, thief!
    break
  case 2
    emote levels an ancient relic blaster and squeezes off a searing shot!
    break
done
~
#40908
Dok-Ondar Death~
0 f 100
~
say My treasures... scattered... to the winds...
emote curls his long neck downward and goes still.
~
#40909
Oga Garra Fights~
0 k 15
~
switch %random.3%
  case 1
    say You picked the WRONG cantina, spice-brain.
    break
  case 2
    emote snarls and signals her enforcers with a jerk of her head.
    break
  case 3
    say Nobody crosses Oga Garra and walks out!
    break
done
~
#40910
Oga Garra Death~
0 f 100
~
say This... isn't over... the Syndicate will hear of this...
emote slumps across the bar with a final, rattling breath.
~
#40911
Vi Moradi Assesses Recruits~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.class% == Knight
    say A Knight! The Resistance needs blades like yours against the First Order.
  elseif %actor.class% == Scoundrel
    say A smuggler's skills, hm? We pay well for discretion.
  elseif %actor.class% == Sorcerer
    say The Force runs strong in you. Careful -- the First Order hunts your kind.
  elseif %actor.race% == Wookiee
    say A Wookiee ally! Chewbacca speaks well of your people.
  else
    say Keep your voice down. The First Order has ears everywhere, friend.
  end
end
~
#40912
Gatherer Senses Potential~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.class% == Sorcerer
    say You already touch the Force. A kyber blade would but focus it.
  elseif %actor.race% == Fae
    say The Fae carry starlight within. A kyber crystal would sing for you.
  else
    say Speak of lightsabers, and I will show you what the Gatherers guard.
  end
end
~
#40913
Sith Lord Fights~
0 k 15
~
switch %random.3%
  case 1
    say Your feeble skills are no match for the power of the dark side!
    break
  case 2
    emote thrusts out a hand and hurls crackling dark lightning!
    break
  case 3
    say Strike me down and your journey to the dark side will be complete!
    break
done
~
#40914
Sith Lord Death~
0 f 100
~
say Impossible... the dark side... cannot... fall...
emote collapses, the crimson blade guttering out as shadow drains away.
~
#40915
Sith Lord Greets~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.class% == Sorcerer
    say I sense the Force in you. Kneel, and I will teach you true power.
  elseif %actor.class% == Knight
    say A hero comes to die. How predictable. Ignite your courage -- it will not save you.
  else
    say You should not have come here. This throne room will be your tomb.
  end
end
~
$~
