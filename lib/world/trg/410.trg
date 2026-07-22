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
$~
