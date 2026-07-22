#41000
Chewbacca Growls~
0 b 25
~
switch %random.2%
  case 1
    emote throws back his head and lets loose a mighty Wookiee roar.
    break
  case 2
    emote rumbles low and gestures impatiently at the dejarik board.
    break
done
~
#41001
Han Solo Greets~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.class% == Scoundrel
    say A smuggler, huh? Finally, someone who gets it. Hands off my ship, though.
  elseif %actor.class% == Knight
    say A hero type. Great. Try not to get us all killed, your worship.
  elseif %actor.race% == Wookiee
    say Another Wookiee? Chewie'll be thrilled. Just don't crowd the cockpit.
  else
    say Hey, kid. Don't get any ideas about my ship.
  end
  emote flashes a lopsided grin and checks his blaster.
end
~
#41002
Han Solo Fights~
0 k 12
~
switch %random.2%
  case 1
    say Look, I'm a nice guy -- but you're really pushing it.
    break
  case 2
    emote fires his DL-44 from the hip, and shoots first.
    break
done
~
#41003
Chewbacca Fights~
0 k 12
~
switch %random.2%
  case 1
    emote throws back his head with a deafening roar and swings a massive fist!
    break
  case 2
    emote fires his bowcaster with a furious growl.
    break
done
~
#41004
Chewbacca Greets Kin~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.race% == Wookiee
    emote throws back his head, roars with joy, and pulls you into a crushing hug!
  elseif %actor.class% == Scoundrel
    emote rumbles approvingly at a fellow scoundrel.
  else
    emote rumbles a low, cautious greeting.
  end
end
~
$~
