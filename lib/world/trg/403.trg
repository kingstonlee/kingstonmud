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
#40302
Evil Queen Enraged~
0 k 12
~
switch %random.3%
  case 1
    say You DARE raise a hand to me? I am the fairest of them all!
    break
  case 2
    emote hurls a cursed apple that bursts in a cloud of green smoke!
    break
  case 3
    say Mirror, mirror -- strike this insolent fool down!
    break
done
~
#40303
Evil Queen Death~
0 f 100
~
say No... it cannot be... I was the FAIREST!
emote crumbles into a heap of black robes and bitter dust.
~
#40304
King Arthur Hails Newcomers~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.class% == Knight
    say Well met, fellow Knight! Rise, and be welcome at my court.
  elseif %actor.class% == Sorcerer
    say A wielder of the arcane... Merlin himself would welcome you.
  elseif %actor.race% == Fae
    say The fair folk grace my hall. You honor Camelot.
  else
    say Welcome, traveller, to the court of the once and future king.
  end
end
~
#40305
Tinker Bell Sizes You Up~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  if %actor.race% == Fae
    emote jingles with delight and flutters a happy circle around a fellow fae!
  elseif %actor.race% == Na'vi
    emote hovers close, entranced by a kindred child of the wild.
  else
    emote folds her arms, jingles once, and dusts you with a wary little sparkle.
  end
end
~
$~
