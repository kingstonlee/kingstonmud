#include "mysql/mysql.h"

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "deities.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "constants.h"
#include "dg_scripts.h"
#include "feats.h"
#include "polls.h"

struct poll_data poll_list[NUM_POLLS];

void add_poll(int pnum, char *question, sbyte active, sbyte revote)
{
  poll_list[pnum].title = strdup(question);
  poll_list[pnum].active = active;
  poll_list[pnum].revote = revote;
  poll_list[pnum].options[0] = strdup("\r\n");
}

void add_poll_option(int pnum, int onum, char *option)
{



  extern MYSQL *conn;

  // Open mysql connection
  conn = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in add poll option.");
  }


  MYSQL_RES *result = NULL;
  MYSQL_ROW row = NULL;
  char query[300];

  if (onum < 0 || onum > 9)
    return;

  poll_list[pnum].options[onum] = strdup(option);

  if (CONFIG_DFLT_PORT == 4000 || CONFIG_DFLT_PORT == 6070) {
    sprintf(query, "SELECT name FROM  `poll_data` WHERE  `poll_num` = '%d' AND  `option` = '%d'", pnum, onum);

    mysql_query(conn, query);
    result = mysql_use_result(conn);

    if (result == NULL) {
      poll_list[pnum].votes[onum] = 0;
      log("NO RESULT FOUND FOR POLL NUMBER %d OPTION %d", pnum, onum);
      log(query);
    }
    else {
      while ((row = mysql_fetch_row(result))) {
          poll_list[pnum].votes[onum]++;
          log("RESULT FOR FOR POLL NUMBER %d OPTION %d is %d", pnum, onum, atoi(row[0]));          
          log(query);
      }
    }
  }

  mysql_close(conn);
}

void build_poll_list(void)
{
  add_poll(0, "Test", FALSE, FALSE);
  add_poll_option(0, 1, "Test Option");

  add_poll(1, "How did you hear about d20MUD: Age of Dragons?", TRUE, FALSE);
  add_poll_option(1, 1, "Through a search engine search.");
  add_poll_option(1, 2, "From a MUD Connector forum post.");
  add_poll_option(1, 3, "From a MUD Connector MUD search.");
  add_poll_option(1, 4, "From a facebook advertisement.");
  add_poll_option(1, 5, "Through a friend / word of mouth.");
  add_poll_option(1, 6, "Another unmentioned method.");
  add_poll_option(1, 7, "From a MUD Connector banner ad.");

  add_poll(2, "Should we open Sanction as a City for Evil Characters?", FALSE, FALSE);
  add_poll_option(2, 1, "No.  There are not enough players to warrant a second starting city yet.");
  add_poll_option(2, 2, "No.  We should fully flesh out Palanthas and the surrounding area with content & quests first.");
  add_poll_option(2, 3, "Yes.  Evil people need a place to do their scheming.");
  add_poll_option(2, 4, "I am undecided or I don't really care either way.");

  add_poll(3, "Do you believe that normal experience gains for role playing should be increased?\r\n"
              "Keep in mind that experience rewards for rp go down on a percent of your next level\r\n"
              "requirement basis as you increase in level.  We don't want people gaining massive\r\n"
              "amounts of exp compared to their required for next level at higher levels either.\r\n"
              "But we do believe that role playing should be a viable alternative for grinding in\r\n"
              "terms of gaining combat class levels.", FALSE, FALSE);
  add_poll_option(3, 1, "Yes.  By at least double.  It is way too low to be a worthwhile incentive.");
  add_poll_option(3, 2, "Yes.  By maybe 50%.  It is low but not too, too low.");
  add_poll_option(3, 3, "No, It is fine how it is.");
  add_poll_option(3, 4, "No, in fact it needs to be decreased a little bit.");

  add_poll(4, "What do you want to see for the Christmas Bonus for players this year?\r\n"
              "I will be giving a 50% exp bonus between Dec 24 and Jan 1.  So this is\r\n"
              "in addition to that.\r\n@lYOU CAN CHANGE YOUR VOTE ON THIS POLL BY VOTING AGAIN.@n\r\n", FALSE, FALSE);
  add_poll_option(4, 1, "Make the exp bonus 100% instead of 50%");
  add_poll_option(4, 2, "A custom item for each regular player (1 per account), including special code functionality.");
  add_poll_option(4, 3, "Guild halls for all of the clans, including restricted access for guild mambers & a board.");
  add_poll_option(4, 4, "Something else (send a mail to gicker with your idea, and we'll post it here.");

  add_poll(5, "What time zone do you normally play from?\r\n", TRUE, TRUE);
  add_poll_option(5, 1, "-12 to -9 GMT");
  add_poll_option(5, 2, "Pacific / -8 GMT");
  add_poll_option(5, 3, "Mountain / -7 GMT");
  add_poll_option(5, 4, "Central / -6 GMT");
  add_poll_option(5, 5, "Eastern / -5 GMT");
  add_poll_option(5, 6, "Atlantic / NFLD / -4 GMT");
  add_poll_option(5, 7, "Europe / Africa / Middle East");
  add_poll_option(5, 8, "Asia / Oceania / Australia");
  add_poll_option(5, 9, "Hawaii");

  add_poll(6, "What kind of death penalty do you prefer?\r\n", TRUE, FALSE);
  add_poll_option(6, 1, "Current System: Exp Loss");
  add_poll_option(6, 2, "Weakness system with reduced damage and hit points for 10 minutes");
  add_poll_option(6, 3, "Weakness system with small penalty to future exp gains for 10 minutes");
  add_poll_option(6, 4, "Other System (please post it on the forums at d20mud.com)");

}
