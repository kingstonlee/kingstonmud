#40800
Use Pixie Dust to Get to Neverland~
2 c 100
s~
if %cmd% == sprinkle && (%arg% /= pixie || %arg% /= dust)
  if %actor.inventory(40800)%
    %send% %actor% You reach into your pocket and sprinkle some pixie dust onto your head.
    %echoaround% %actor% %actor.name% sprinkles some pixie dust onto %actor.hisher% head.
    wait 5 s
    %send% %actor% Nothing happens.
    %echoaround% %actor% Nothing happens.
    wait 5 s
    %send% %actor% You close your eyes, make a happy thought and feel your feet lift off from the ground.
    %echoaround% %actor% %actor.name% closes eyes and starts to float into the sky.
    %teleport% %actor% 40801
    %send% %actor% You open your eyes, the moon is ahead of you and Disneyland is getting smaller and smaller behind you.
    wait 5 s
    %send% %actor% Blinking once, you watch as the moon gets smaller behind you and an island gets closer.
    wait 5 s
    %teleport% %actor% 40802
  else
    %send% %actor% You pat your pockets and pull out a piece of lint.
    %send% %actor% Nothing happens.
    %echoaround% %actor% You see %actor.name% patting his pockets like he lost his marbles.
  end
end
~
#40801
Peter Pan's Flight Cast Member Dialogue~
0 b 25
~
switch %random.5%
  case 1
    emote says, "Next guest!" 
    break
  case 2
    emote says, "Please step all the way in." 
    break
  case 3
    emote says, "Watch for the bar coming down on your lap."
    break
  case 4
    emote mumbles, "	Y	_Sprinkle Pixie Dust	n	n"
    break
  case 5
    emote mumbles, "	Y	_Use Pixie Dust	n	n"
    break
~
$~
