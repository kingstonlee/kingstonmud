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
  say Hey, kid. Don't get any ideas about my ship.
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
$~
