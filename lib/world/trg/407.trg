#40700
Ghost Host Welcome~
0 g 100
~
if %actor.is_pc%
  wait 1 s
  say Welcome, foolish mortal, to the Haunted Mansion.
  emote sweeps an arm toward the grinning ghosts waiting within.
end
~
#40701
Street Jazz~
0 b 25
~
switch %random.2%
  case 1
    emote closes his eyes and blows a slow, smoky trumpet solo.
    break
  case 2
    say Laissez les bons temps rouler, cher!
    break
done
~
#40702
Pirate Captain Fights~
0 k 12
~
switch %random.3%
  case 1
    say Ye'll not take me alive, ye bilge rat!
    break
  case 2
    emote swings from a rigging rope and slashes wildly!
    break
  case 3
    say Dead men tell no tales -- and neither shall you!
    break
done
~
#40703
Pirate Captain Death~
0 f 100
~
say Blast ye... the sea... claims another...
emote topples over with a final, rum-soaked groan.
~
$~
