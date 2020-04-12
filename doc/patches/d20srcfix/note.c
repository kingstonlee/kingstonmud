/* 
 * Copyright 2011 William Rupert Stephen Squires (aka Gicker)
 * You may use this code for any purpose, but you must give credit
 * to it's creator in your credits file, or another easily-accessible
 * location on your game or program.
 */

#include "conf.h"
#include "sysdep.h"

#include "mysql/mysql.h"
#include "structs.h"
#include "comm.h"
#include "interpreter.h"
#include "db.h"
#include "feats.h"
#include "utils.h"
#include "spells.h"
#include "handler.h"
#include "improved-edit.h"
#include "constants.h"

MYSQL *conn2;
extern MYSQL *conn;

char *get_blank_clan_name(int clan);

void note_list_all_cats(struct char_data *ch) {

  int num_cats = 0;
  int unread = 0, total = 0;
  char buf[400];

  // Open mysql connection
  conn2 = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;

  char query[MAX_INPUT_LENGTH];

  send_to_char(ch, "%-20s %-25s\r\n", "Category Keyword", "Category Name");
  send_to_char(ch, "%-20s %-25s\r\n", "--------------------", "-------------------------");

  sprintf(query, "SELECT COUNT(*) FROM player_note_categories WHERE adm_level <= '%d'", GET_ADMLEVEL(ch));
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      num_cats = atoi(row[0]);
    }
  }
  mysql_free_result(res);

  if (!num_cats || num_cats < 0)
    num_cats = 0;

  if (num_cats > 0) {

  char *cat_text[num_cats];
  char *cat_text_mid[num_cats];
  char *cat_text_fin[num_cats];
  int totals[num_cats];


  int i = 0;
  sprintf(query, "SELECT name, display FROM player_note_categories WHERE adm_level <= '%d' ORDER BY display ASC", GET_ADMLEVEL(ch));
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      if (i < num_cats) {
        sprintf(buf, "%-20s %-25s ", row[0], row[1]);
        cat_text[i] = strdup(buf);
        i++;
      }
    }
  }
  mysql_free_result(res);

  for (i = 0; i < num_cats; i++) {
    totals[i] = 0;
    sprintf(buf, "%s   0 total ", cat_text[i]);
    cat_text_mid[i] = strdup(buf);
  }

  sprintf(query, "SELECT cat_name, COUNT(*) as total FROM player_note_messages GROUP BY cat_name ORDER BY cat_name ASC");
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      for (i = 0; i < num_cats; i++) {
        if (strstr(cat_text[i], row[0]))
          break;
      }
      if (i < num_cats) {
        total = atoi(row[1]);
        totals[i] = total;
        sprintf(buf, "%s %3d total ", cat_text[i], total);
        cat_text_mid[i] = strdup(buf);        
      }
    }
  }
  mysql_free_result(res);

  for (i = 0; i < num_cats; i++) {
    sprintf(buf, "%s %3d unread", cat_text_mid[i], totals[i]);
    cat_text_fin[i] = strdup(buf);
  }

  char name_buf[500];

  sprintf(name_buf, " WHERE ");

  if (ch->desc->account) {
    for (i = 0; i < MAX_CHARS_PER_ACCOUNT; i++) {
      if (ch->desc->account->character_names[i] != NULL) {
        sprintf(name_buf, "%s %splayer_name = '%s' ", name_buf, (i > 0) ? " OR " : "", ch->desc->account->character_names[i]);
      }
    }
  }

  sprintf(query, "SELECT cat_name, COUNT(*) as total FROM player_note_read %s GROUP BY cat_name ORDER BY cat_name ASC",
          name_buf);

  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      for (i = 0; i < num_cats; i++) {
        if (strstr(cat_text[i], row[0]))
          break;
      }
      if (i < num_cats) {
        unread = MAX(0, totals[i] - atoi(row[1]));
        sprintf(buf, "%s %3d unread", cat_text_mid[i], unread);
        cat_text_fin[i] = strdup(buf);
      }
    }
  }
  mysql_free_result(res);


  for (i = 0; i < num_cats; i++)
    send_to_char(ch, "%s\r\n", cat_text_fin[i]);

  }  
  mysql_close(conn2);
}

void note_mark_read(struct char_data *ch)
{
  // Open mysql connection
  conn = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  conn2 = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;

  char query[200];

  sprintf(query, "DELETE FROM player_note_read WHERE player_name='%s'", GET_NAME(ch));
  mysql_query(conn, query);

  sprintf(query, "SELECT id_msg, cat_name FROM player_note_messages");
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      sprintf(query, "INSERT INTO player_note_read (id_msg,player_name,cat_name) VALUES('%s','%s','%s')",
                     row[0], GET_NAME(ch), row[1]);
      mysql_query(conn, query);
    }
  }
  mysql_free_result(res);
  

  send_to_char(ch, "All notes have been marked read"); 

  mysql_close(conn);
  mysql_close(conn2);

}

void note_archive(struct char_data *ch, int start, int end, sbyte archive) {

  // Open mysql connection
  conn = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  conn2 = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;

  int i = start;
  sbyte found = FALSE;
  char query[200];

  for (i = start; i <= end; i++) {
    found = FALSE;
    sprintf(query, "SELECT id_msg FROM player_note_archive WHERE id_msg='%d' AND player_name='%s'", i, GET_NAME(ch));
    mysql_query(conn2, query);
    res = mysql_use_result(conn2);
    if (res != NULL) {
      if ((row = mysql_fetch_row(res)) != NULL) {
        found = TRUE;
      }
    }
    mysql_free_result(res);

    if (found) {
      if (archive) {
        continue;
      } else {
        sprintf(query, "DELETE FROM player_note_archive WHERE id_msg='%d' AND player_name='%s'", i, GET_NAME(ch));
        mysql_query(conn, query);
      }
    } else {
      if (archive) {
        sprintf(query, "INSERT INTO player_note_archive (id_msg,player_name) VALUES('%d','%s')", i, GET_NAME(ch));
        mysql_query(conn, query);
      } else {
        continue;
      }
    }
  }

  send_to_char(ch, "The specified notes have been archived."); 

  mysql_close(conn);
  mysql_close(conn2);  

}

void note_display_unread(struct char_data *ch) {
  int num_cats = 0;
  int unread = 0, total = 0;
  char buf[400];

  // Open mysql connection
  conn2 = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;
  int new_msgs = FALSE;

  char query[MAX_INPUT_LENGTH];

  sprintf(query, "SELECT COUNT(*) FROM player_note_categories WHERE adm_level <= '%d'", GET_ADMLEVEL(ch));
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      num_cats = atoi(row[0]);
    }
  }
  mysql_free_result(res);

  if (!num_cats || num_cats < 0)
    num_cats = 0;

  if (num_cats > 0) {

  char *cat_text[num_cats];
  int totals[num_cats];


  int i = 0;
  sprintf(query, "SELECT name, display FROM player_note_categories WHERE adm_level <= '%d' ORDER BY name ASC", GET_ADMLEVEL(ch));
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      if (i < num_cats) {
        sprintf(buf, "%-20s %-25s ", row[0], row[1]);
        cat_text[i] = strdup(buf);
        i++;
      }
    }
  }
  mysql_free_result(res);

  sprintf(query, "SELECT cat_name, COUNT(*) as total FROM player_note_messages GROUP BY cat_name ORDER BY cat_name ASC");
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      for (i = 0; i < num_cats; i++) {
        if (strstr(cat_text[i], row[0]))
          break;
      }
      if (i < num_cats) {
        total = atoi(row[1]);
        totals[i] = total;
      }
    }
  }
  mysql_free_result(res);

  char name_buf[500];

  sprintf(name_buf, " WHERE ");

  if (ch->desc->account) {
    for (i = 0; i < MAX_CHARS_PER_ACCOUNT; i++) {
      if (ch->desc->account->character_names[i] != NULL) {
        sprintf(name_buf, "%s %splayer_name = '%s' ", name_buf, (i > 0) ? " OR " : "", ch->desc->account->character_names[i]);
      }
    }
  }

  sprintf(query, "SELECT cat_name, COUNT(*) as total FROM player_note_read %s GROUP BY cat_name ORDER BY cat_name ASC", 
          name_buf);
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      for (i = 0; i < num_cats; i++) {
        if (strstr(cat_text[i], row[0]))
          break;
      }
      if (i < num_cats) {
        unread = MAX(0, totals[i] - atoi(row[1]));
        if (unread > 0) {
        new_msgs = TRUE;
        send_to_char(ch, "@l@WYou have %d unread notes in category %s! (See @YHELP NOTE@W)@n\r\n", unread, row[0]);        
        }
      }
    }
  }
  mysql_free_result(res);

  }  
  mysql_close(conn2);
}

void note_list_single_cat(struct char_data *ch, char *arg, char *buf2) {

  char arg2[200];

  one_argument(buf2, arg2);

  send_to_char(ch, "Listing note category %s.\r\n\r\n", arg);

  // Open mysql connection
  conn2 = mysql_init(NULL);
  // Open mysql connection
  conn = mysql_init(NULL);

  send_to_char(ch, "Messages in Category '%s'\r\n\r\n"
               "    MSG_ID AUTHOR          TIME                SUBJECT\r\n"
               "    ------ --------------- ------------------- ------------------------------\r\n", arg);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }
  /* Connect to database */
  if (!mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;

  MYSQL_RES *res2 = NULL;
  MYSQL_ROW row2 = NULL;

  char query[MAX_INPUT_LENGTH];

  char clansql[100];

  sprintf(clansql, " AND poster_clan='%s'", get_blank_clan_name(GET_CLAN(ch)));

  sprintf(query, "SELECT adm_level FROM player_note_categories WHERE name='%s'", arg);
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      if (GET_ADMLEVEL(ch) < atoi(row[0])) {
        send_to_char(ch, "You do not have access to that message. (wrong admin level)\r\n");
        return;
      }
    }
  }
  mysql_free_result(res);

  sbyte unread = TRUE, archive = FALSE;

  sprintf(query, "SELECT id_msg, subject, poster_name, time_stamp, cat_name FROM player_note_messages WHERE cat_name = '%s'%s ORDER BY id_msg DESC LIMIT %d", arg, strstr(arg, "clan") ? clansql : "", !*arg2 ? 20 : MAX(20, MIN(100, atoi(arg2))));
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      unread = TRUE; archive = FALSE;
      sprintf(query, "SELECT id_msg FROM player_note_read WHERE id_msg='%s' AND player_name='%s' AND cat_name='%s'", row[0], GET_NAME(ch), row[4]);
      mysql_query(conn, query);
      res2 = mysql_use_result(conn);
      if (res2 != NULL) {
        if ((row2 = mysql_fetch_row(res2)) != NULL) {
          unread = FALSE;
        }
      }
      mysql_free_result(res2);
      
      sprintf(query, "SELECT id_msg FROM player_note_archive WHERE id_msg='%s' AND player_name='%s'", row[0], GET_NAME(ch));
      mysql_query(conn, query);
      res2 = mysql_use_result(conn);
      if (res2 != NULL) {
        if ((row2 = mysql_fetch_row(res2)) != NULL) {
          archive = TRUE;
        }
      }
      mysql_free_result(res2);

      if (archive) continue;
      
      send_to_char(ch, "%-3s %6d %-15s %-19s %-50s\r\n",  unread ? "NEW" : "", atoi(row[0]), row[2], row[3], row[1]);
    }
  }
  mysql_free_result(res);

//  page_string(ch->desc, buf, 0);

  mysql_close(conn2);
  mysql_close(conn);
}

void note_read(struct char_data *ch, char *arg) {
  // Open mysql connection
  conn2 = mysql_init(NULL);

  /* Connect to database */
  if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
    log("Cannot connect to mysql database in note_list_all_cats.");
  }

  MYSQL_RES *res = NULL;
  MYSQL_ROW row = NULL;

  char query[MAX_INPUT_LENGTH];

  int found = TRUE;
  char cat[255];

  sprintf(query, "SELECT a.adm_level, a.name, b.poster_clan FROM player_note_categories a LEFT JOIN player_note_messages b ON a.name=b.cat_name WHERE b.id_msg='%s'", arg);
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    while ((row = mysql_fetch_row(res)) != NULL) {
      if (GET_ADMLEVEL(ch) < atoi(row[0])) {
        send_to_char(ch, "You do not have access to that message. (wrong admin level)\r\n");
        return;
      }
      if (strstr(row[1], "clan") && !strstr(row[2], get_blank_clan_name(GET_CLAN(ch)))) {
        send_to_char(ch, "You do not have access to that message. (wrong clan)\r\n");
        return;
      }
    }
  }
  mysql_free_result(res);

  sprintf(query, "SELECT id_msg, subject, message, cat_name, poster_name, time_stamp FROM player_note_messages WHERE id_msg='%s'", arg);
  mysql_query(conn2, query);
  res = mysql_use_result(conn2);
  if (res != NULL) {
    if ((row = mysql_fetch_row(res)) != NULL) {
      send_to_char(ch, "Messsage ID: %d Author: %s Timestamp: %s Subject: %s\r\n\r\n"
                       "Message:\r\n--------------------------------------------------------------------------------\r\n%s\r\n",
                       atoi(row[0]), row[4], row[5], row[1], row[2]);
      
      sprintf(cat, "%s", row[3]);
    } else found = FALSE;
  } else found = FALSE;
  mysql_free_result(res);

  if (!found) {
    send_to_char(ch, "There is no note message with that message id.\r\n");
    return;
  }

  sprintf(query, "REPLACE INTO player_note_read (id_msg, player_name, cat_name) VALUES('%s','%s','%s')", arg, GET_NAME(ch), cat);
  mysql_query(conn2, query);

  mysql_close(conn2);
}

ACMD(do_note)
{

  char arg[200], arg2[200];
  char buf[MAX_INPUT_LENGTH];
  char buf2[MAX_INPUT_LENGTH];

  half_chop(argument, arg, buf);
  half_chop(buf, arg2, buf2);

  if (!*arg || is_abbrev(arg, "list")) {

    if (!*arg2 || is_abbrev(arg2, "all")) {
      note_list_all_cats(ch);
      return;
    } else {
      note_list_single_cat(ch, arg2, buf2);
      return;
    }
  } else if (is_abbrev(arg, "markread")) {
    note_mark_read(ch);
    return;
  } else if (is_abbrev(arg, "archive")) {
    if (!*arg2) {
      send_to_char(ch, "You must specify a message id to archive.  Optionally you can specify instead a start and end id to archive all in between.\r\n");
      return;
    }
    int start = atoi(arg2);
    if (start <= 0) {
      send_to_char(ch, "You must specify a starting or specific message id above zero.\r\n");
      return;
    }
    int end = 0;
    if (!*buf2) {
      end = start;
    } else {
      end = atoi(buf2);
    }
    if (end <= 0) {
      send_to_char(ch, "You must specify an ending message id above zero.\r\n");
      return;
    }

    note_archive(ch, start, end, TRUE);
    return;
  } else if (is_abbrev(arg, "unarchive")) {
    if (!*arg2) {
      send_to_char(ch, "You must specify a message id to archive.  Optionally you can specify instead a start and end id to unarchive all in between.\r\n");
      return;
    }
    int start = atoi(arg2);
    if (start <= 0) {
      send_to_char(ch, "You must specify a starting or specific message id above zero.\r\n");
      return;
    }
    int end = 0;
    if (!*buf2) {
      end = start;
    } else {
      end = atoi(buf2);
    }
    if (end <= 0) {
      send_to_char(ch, "You must specify an ending message id above zero.\r\n");
      return;
    }

    note_archive(ch, start, end, FALSE);
    return;
  } else if (is_abbrev(arg, "read")) {
    if (!*arg2) {
      send_to_char(ch, "You must specify a message id to read.  To view a list of messages use the @Ynote list@n command.\r\n");
      return;
    }
    note_read(ch, arg2);
    return;
  } else if (is_abbrev(arg, "write")) {
      extern int circle_copyover;
      if (circle_copyover) {
        send_to_char(ch, "A copyover is scheduled.  Please wait until after to write your note.\r\n");
        return;
      }
/*
    send_to_char(ch, "Please use the following link to post a note: http://starwars.d20mud.com/boards\r\n\r\n"
                     "Don't forget to set your web password first by typing @Ywebsitepassword (password)@n\r\n");
    return;
*/
    // Open mysql connection
    conn2 = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
      log("Cannot connect to mysql database in note_list_all_cats.");
    }

    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

    char query[MAX_INPUT_LENGTH];
    int found = FALSE;

    sprintf(query, "SELECT name FROM player_note_categories LIMIT 1");
    mysql_query(conn2, query);
    res = mysql_use_result(conn2);
    if (res != NULL) {
      while ((row = mysql_fetch_row(res)) != NULL) {
        found = TRUE;        
      }
    }
    mysql_free_result(res);

    if (!found) {
      send_to_char(ch, "The in-game note writing command is broken at the moment.\r\n"
                       "For the time being please write your notes using our web interface at http://www.d20mud.com/d20/forum/\r\n"
                       "The problem is likely related to a database connection and will resolve itself shortly.\r\n");
      return;
    }

    mysql_close(conn2);

    if (!*arg2) {
      send_to_char(ch, "You must specify a category to post in.  To view a list of categories use the @Ynote list@n command.\r\n");
      return;
    }
    if (!*buf2) {
      send_to_char(ch, "You must provide a message subject.\r\n");
      return;
    }

    // Open mysql connection
    conn2 = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn2, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)) {
      log("Cannot connect to mysql database in note_list_all_cats.");
    }

    int cfound = FALSE;

    sprintf(query, "SELECT name FROM player_note_categories WHERE name='%s'", arg2);
    mysql_query(conn2, query);
    res = mysql_use_result(conn2);
    if (res != NULL) {
      if ((row = mysql_fetch_row(res)) != NULL) {
        cfound = TRUE;
      }
    }
    mysql_free_result(res);

    mysql_close(conn2);

    if (!cfound) {
      send_to_char(ch, "There is not category by that name.  To view a list of categories use the @Ynote list@n command.\r\n");
      return;
    }

    ch->player_specials->note_cat = strdup(arg2);
    ch->player_specials->note_subj = strdup(buf2);
    send_editor_help(ch->desc);
    act("$n starts to write a note.", TRUE, ch, 0, 0, TO_ROOM);
    SET_BIT_AR(PLR_FLAGS(ch), PLR_WRITING);     /* string_write() sets writing. */
    ch->player_specials->note = strdup("@n");
    string_write(ch->desc, &ch->player_specials->note, 20000, -999, NULL);
    STATE(ch->desc) = CON_NOTE_WRITE;
    return;

  } else {

  }
 

}
