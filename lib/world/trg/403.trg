#40300
White Rabbit Frets~
0 b 25
~
switch %random.3%
  case 1
    say Oh dear! Oh dear! I shall be too late!
    break
  case 2
    emote checks his enormous pocket watch and gasps in alarm.
    break
  case 3
    say No time to say hello, goodbye! I'm late, I'm late!
    break
done
~
#40301
Evil Queen Greets~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  say Mirror, mirror... and who might YOU be, my pretty?
  emote offers you a gleaming red apple with a cold, thin smile.
end
~
$~
