Hi,

What is this?
  It's a new take on clans, as a patch for tbaMUD 3.68.

How do I use it? 
  Copy the patch file to your tbamud directory.
  patch it in with this command:

	patch --strip 1 < clans_2018-01-13.patch

  Then build as usual.
    
How does it work?
  The clans are stored in lib/etc/clans
  They use linked lists for lookups - since these lists will never be extremely long, 
  this should not matter for performance.


Here's a help file entry you might want to add:
--------------------------------------------------------------
CLAN CLANS
 
If you aren't a member of any clans, you have these commands available:
 
  clan list   
      - shows a list of all current clans, along with the number of members in each.
 
  clan info <clan>
      - shows you info about that particular clan. Notably lists members and ranks.
 
Once you've been admitted to a clan, you get access to these commands:
 
  clan who
      - lists currently online clan members
 
  clan tell <message>
  ct <message>
      - broadcast the message to your online clan members
 
  clan leave
      - leaves the clan. Be careful, you will have to be added by the clan leader to rejoin.
        Also, if you leave as the only leader, someone else will be promoted to fill your place.
 
  clan info
      - as above, but now defaults to your clan
 
If you are so lucky as to get higher ranks in you clan, you will eventually get access to these commands:
 
  clan enlist <player>
      - enlists the player in your guild.
 
  clan dismiss <player>
      - dismisses (kicks out) the player from your guild
 
  clan promote <player>
      - promotes the player to the next higher rank
 
  clan demote <player>
      - demotes the player to the next lower rank
 
Leaders may also do this:
 
  clan rename <new name>
      - renames the clan
 
  clan rank <number> <new rank>
      - renames the clan ranks
 
Gods may create a new clan with clan create and remove it with clan disband.

--------------------------------------------------------------

