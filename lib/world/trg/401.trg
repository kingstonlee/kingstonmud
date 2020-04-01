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
#40101
Train Track~
2 ab 100
~
if %dlrrinmotion%
  %echo% The engineer does a coal check
else
* Announce to Room - All Aboard - Next Stop New Orleans Square
  %echo% TOOT!
  %asound% TOOT!
  %echo% Next Stop New Orleans Square
* Give 5 Seconds
  wait 10 s
  %echo% 	RTOOT! TOOT!	n
  %asound% 	RTOOT! TOOT!	n
  %echo% 	YAll Aboard!	n
  %asound% 	YAll Aboard!	n
  %echo% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
  %asound% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
* Remove Main Street Exit North (40111)
  %door% %self.vnum% n purge
  %door% 40111 s purge
* Set Global Variable to INTRANSIT
  set dlrrinmotion 1
* Announce in 40707 - The Disneyland Train Arriving
  %at% 40707 %echo% 	gThe Disneyland Train is now Arriving	n
* Give 5 Seconds
  wait 10 s
* Announce Now Arriving at New Orleans Square
  %echo% 	GNow arriving at New Orleans Square	n
  %asound% 	GNow arriving at New Orleans Square	n
* Create Exit to 40707
  %door% %self.vnum% e room 40707
  %door% 40707 w room %self.vnum%
* Give 5 Seconds
  wait 10 s
* Announce to Room - All Aboard - Next Stop Toontown
  %echo% TOOT!
  %asound% TOOT!
  %echo% Next Stop Toontown
  %asound% Next Stop Toontown
* Give 5 Seconds
  wait 10 s
  %echo% 	RTOOT! TOOT!	n
  %asound% 	RTOOT! TOOT!	n
  %echo% All Aboard!
  %asound% All Aboard!
  %echo% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
  %asound% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
* Remove New Orleans Square (40707)
  %door% %self.vnum% e purge
  %door% 40707 w purge
* Announce in 40322 - The Disneyland Train Arriving
  %at% 40322 %echo% The Disneyland Train is now Arriving
* Give 5 Seconds
  wait 10 s
* Announce Now Arriving at Toontown
  %echo% Now arriving at Toontown
* Create Exit to 40322
  %door% %self.vnum% s room 40322
  %door% 40322 n room %self.vnum%
* Give 5 Seconds
  wait 10 s
* Announce to Room - All Aboard - Next Stop Tomorrowland
  %asound% TOOT!
  %echo% TOOT!
  %asound% Next Stop Tomorrowland
  %echo% Next Stop Tomorrowland
* Give 5 Seconds
  wait 10 s
  %echo% 	RTOOT! TOOT!	n
  %echo% All Aboard!
  %asound% 	RTOOT! TOOT!	n
  %asound% All Aboard!
  %echo% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
  %asound% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
* Remove Toontown (40322)
  %door% %self.vnum% s purge
  %door% 40322 n purge
* Announce in 40403 - The Disneyland Train Arriving
  %at% 40403 %echo% The Disneyland Train is now Arriving
* Give 5 Seconds
  wait 10 s
* Announce Now Arriving at Tomorrowland
  %echo% Now arriving at Tomorrowland
* Create Exit to 40403
  %door% %self.vnum% e room 40403
  %door% 40403 w room %self.vnum%
* Give 5 Seconds
  wait 10 s
* Announce to Room - All Aboard - Next Stop Main Street
  %echo% TOOT!
  %asound% TOOT!
  %echo% Next Stop Main Street
  %asound% Next Stop Main Street
* Give 5 Seconds
  wait 10 s
* Remove Tomorrowland (40403)
  %door% %self.vnum% e purge
  %door% 40403 w purge
* Announce in 40111 - The Disneyland Train Arriving
  %at% 40111 %echo% The Disneyland Train is now Arriving
* Give 5 Seconds
  wait 10 s
* Announce Now Arriving at Main Street
  %echo% Now arriving at Main Street
* Set Global Variable to MAINSTREET
  set dlrrinmotion 0
* Create Exit to 40111
  %door% %self.vnum% n room 40111
  %door% 40111 s room %self.vnum%
~
#40102
Test~
2 b 100
~
%echo% testing!
while %i% < 4
  switch %i%
    case 0
      set dlrrstationname Main Street
      set dlrrstationvnum 40111
      set dlrrstationnext New Orleans Square
      set dlrrstationnextvnum 40707
      break
    case 1
      set dlrrstationname New Orleans Square
      set dlrrstationvnum 40707
      set dlrrstationnext Toontown
      set dlrrstationnextvnum 40322
      break
    case 2
      set dlrrstationname Toontown
      set dlrrstationvnum 40322
      set dlrrstationnext Tomorrowland
      set dlrrstationnextvnum 40403
      break
    case 3
      set dlrrstationname Tomorrowland
      set dlrrstationvnum 40403
      set dlrrstationnext Main Street
      set dlrrstationnextvnum 40111
      break
    done
  eval i %i% + 1
  * Announce to Room - All Aboard - Next Stop 
  %echo% TOOT!
  %asound% TOOT!
  %echo% Next Stop %dlrrstationnext%
* Give 5 Seconds
  wait 5 s
  %echo% 	RTOOT! TOOT!	n
  %asound% 	RTOOT! TOOT!	n
  %echo% 	YAll Aboard!	n
  %asound% 	YAll Aboard!	n
  %echo% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
  %asound% 	RTOOOOT! TOOT! TOOOOT! TOOOOT!	n
* Remove Exits
  %door% %self.vnum% n purge
  %door% %dlrrstationvnum% s purge
* Announce - The Disneyland Train Arriving
  %at% %dlrrstationnextvnum% %echo% 	gThe Disneyland Train is now Arriving	n
* Give 5 Seconds
  wait 5 s
* Announce Now Arriving at Next Station
  %echo% Now arriving at %dlrrstationnext%
  %asound% Now arriving at %dlrrstationnext%
* Create Exit
  %door% %self.vnum% e room %dlrrstationnextvnum%
  %door% %dlrrstationnextvnum% w room %self.vnum%
* Give 5 Seconds
  wait 5 s
done
~
$~
