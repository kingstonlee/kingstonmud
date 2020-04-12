/* ************************************************************************
*   File: Guild.c                                                         *
*  Usage: GuildMaster's: loading files, assigning spec_procs, and handling*
*                        practicing.                                      *
*                                                                         *
* Based on shop.c.  As such, the CircleMud License applies                *
* Written by Jason Goodwin.   jgoodwin@expert.cc.purdue.edu               *
************************************************************************ */


#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "spells.h"
#include "guild.h"
#include "shop.h" /* for NO_TRADE_WITH bits. */
#include "constants.h"


/* extern function prototypes */
ACMD(do_tell);
ACMD(do_say);
extern int guild_gain(struct char_data *ch, struct char_data *keeper) ;
extern int get_money(struct char_data *ch);
extern void min_coins(int *copper, int *silver, int *gold, int *platinum);
extern void actual_coins(struct char_data *ch, int *copper, int *silver, int *gold, int *platinum);
extern int copper_val(int *copper, int *silver, int *gold, int *platinum);
extern void pay_for_service(struct char_data *ch, int copperamt);

/* Local variables */
int spell_sort_info[MAX_SKILLS+1];

void what_does_gm_know(guild_rnum guild_nr, struct char_data *ch);

int compare_spells(const void *x, const void *y)
{
  int    a = *(const int *)x,
    b = *(const int *)y;

  return strcmp(spell_info[a].name, spell_info[b].name);
}

void sort_spells(void)
{
  int a;

  /* initialize array, avoiding reserved. */
  for (a = 1; a <= MAX_SKILLS; a++)
    spell_sort_info[a] = a;

  qsort(&spell_sort_info[1], MAX_SKILLS, sizeof(int), compare_spells);
}

const char *skill_how_good(int percent)
{
   int number;

   if (percent < 0)
     return " ERROR!";

   if (percent == 0)
     return " not";

   if (percent == 100)
     return " superior";

   number = percent % 10; 

   if (number == 1)
     return " novice";
   if (number == 2)
     return " junior";
   if (number == 3)
     return " apprentice";
   if (number == 4)
     return "";
   if (number == 5)
     return " confident";
   if (number == 6)
     return " seasoned";
   if (number == 7)
     return " expert";
   if (number == 8)
     return " eminent";
   if (number == 9)
     return " brilliant";

   return " superior";
}


const char *skill_level(int percent)
{
   int number;

   if (percent < 0)
     return " ERROR!";

   if (percent == 0)
     return " learned";

   if (percent == 100)
     return " guru";

   number = percent / 10; 

   if (number == 0 || percent == 10)
     return " student";
   if (number == 1 || percent == 20)
     return " amateur";
   if (number == 2 || percent == 30)
     return " layman";
   if (number == 3 || percent == 40)
     return " acolyte";
   if (number == 4 || percent == 50)
     return " journeyman";
   if (number == 5 || percent == 60)
     return " craftsman";
   if (number == 6 || percent == 70)
     return " professional";
   if (number == 7 || percent == 80)
     return " veteran";
   if (number == 8 || percent == 90)
     return " master";

   return " guru";
}

int is_guild_open(struct char_data *keeper, guild_rnum guild_nr, int msg)
{
#if 1 /* we don't actually use opening hours at the moment. */
  return (TRUE);
#else
  char buf[200];

  *buf = 0;
  if (GM_OPEN(guild_nr) > time_info.hours && GM_CLOSE(guild_nr) < time_info.hours)
    strcpy(buf, MSG_TRAINER_NOT_OPEN);

  if (!(*buf))
    return (TRUE);
  if (msg)
    do_say(keeper, buf, find_command("say"), 0);
  return (FALSE);
#endif
}

int is_gm_ok_char(struct char_data * keeper, struct char_data *ch, guild_rnum guild_nr)
{

  char buf[200];

  if (!(CAN_SEE(keeper, ch))) {
    do_say(keeper, MSG_TRAINER_NO_SEE_CH, cmd_say, 0);
    return (FALSE);
  }
  if (IS_NPC(ch)) {
    sprintf(buf, "%s %s", GET_NAME(ch), "Why would a mob need to practice?");
    do_tell(keeper, buf, cmd_tell, 0);
    return (FALSE);
  }
  if (GET_LEVEL(ch) >= LVL_IMMORT) { /* just list the skills available */
    what_does_gm_know(guild_nr, ch);
    return (FALSE);
  }
  
  if ((IS_MAGIC_USER(ch) && !TRAIN_MAGE(guild_nr)) ||
      (IS_CLERIC(ch)     && !TRAIN_CLERIC(guild_nr)) ||
      (IS_THIEF(ch)      && !TRAIN_THIEF(guild_nr)) ||
      (IS_WARRIOR(ch)    && !TRAIN_WARRIOR(guild_nr)) ||
      (IS_VAMPIRE(ch)    && !TRAIN_VAMPIRE(guild_nr)) ||
      (IS_BARBARIAN(ch)  && !TRAIN_BARBARIAN(guild_nr)) ||
      (IS_PSIONICIST(ch) && !TRAIN_PSIONICIST(guild_nr)) ||
      (IS_PALADIN(ch)    && !TRAIN_PALADIN(guild_nr)) ||
      (IS_RANGER(ch)     && !TRAIN_RANGER(guild_nr)) ||
      (IS_DRUID(ch)      && !TRAIN_DRUID(guild_nr)) ||
      (IS_LICH(ch)       && !TRAIN_LICH(guild_nr))) {
     
    sprintf(buf, GM_CLASS_NOT_TRAINED(guild_nr) ? GM_CLASS_NOT_TRAINED(guild_nr) :
             MSG_DEFAULT_CLASS_NOT_TRAINED , GET_NAME(ch));
    do_tell(keeper, buf, cmd_tell, 0);
    return (FALSE);
  }

  /* Only care if a bit has been set */
  if (TRAIN_GOOD(guild_nr) || TRAIN_NEUTRAL(guild_nr) || TRAIN_EVIL(guild_nr))
    if ((IS_GOOD(ch) && !TRAIN_GOOD(guild_nr)) ||
        (IS_EVIL(ch) && !TRAIN_EVIL(guild_nr)) ||
        (IS_NEUTRAL(ch) && !TRAIN_NEUTRAL(guild_nr))) {
    sprintf(buf, GM_ALIGN_NOT_TRAINED(guild_nr) ? GM_ALIGN_NOT_TRAINED(guild_nr) :
             MSG_DEFAULT_ALIGN_NOT_TRAINED , GET_NAME(ch));
      do_tell(keeper, buf, cmd_tell, 0);
      return (FALSE);
    }

  /* only matters if one is specified */
  if (TRAIN_UNDEAD(guild_nr) || TRAIN_LIVING(guild_nr)) 
    if ((IS_UNDEAD(ch)       && !TRAIN_UNDEAD(guild_nr)) ||
       (IS_LIVING(ch)        && !TRAIN_LIVING(guild_nr))) {
    sprintf(buf, GM_UNDEAD_LIVING_WRONG(guild_nr) ? GM_UNDEAD_LIVING_WRONG(guild_nr) :
             MSG_DEFAULT_UNDEAD_LIVING , GET_NAME(ch));
      do_tell(keeper, buf, cmd_tell, 0);
      return (FALSE);
    }


  return (TRUE);
}

int is_gm_ok(struct char_data * keeper, struct char_data *ch, guild_rnum guild_nr)
{
  if (is_guild_open(keeper, guild_nr, TRUE))
    return (is_gm_ok_char(keeper, ch, guild_nr));
  else
    return (FALSE);
}

int guild_teaches(struct guild_master_data *guild, int skill_num) {
  struct train_info *skill;
  
  for (skill = G_SKILLS(guild);skill;skill = skill->next)
    if (skill->skillnum == skill_num)
      return (1);
  
  return (0);
} 

int get_gm_lowest_taught(struct guild_master_data *guild, int skill_num) {
  struct train_info *skill;
  
  for (skill = G_SKILLS(guild);skill;skill = skill->next)
    if (skill->skillnum == skill_num)
      return (skill->lowest_taught);
  
  return (0);
} 

int get_gm_highest_taught(struct guild_master_data *guild, int skill_num) {
  struct train_info *skill;
  
  for (skill = G_SKILLS(guild);skill;skill = skill->next)
    if (skill->skillnum == skill_num)
      return (skill->highest_taught);
  
  return (0);
} 

int get_gm_price_add(struct guild_master_data *guild, int skill_num) {
  struct train_info *skill;
  
  for (skill = G_SKILLS(guild);skill;skill = skill->next)
    if (skill->skillnum == skill_num)
      return (skill->price_add);
  
  return (0);
} 

void what_does_gm_know(guild_rnum guild_nr, struct char_data *ch)
{
  int i, sortpos;
  char skill_cost[200];
  int copper = 0, silver = 0, gold = 0, platinum = 0, base_price;

  strcpy(buf, "I can teach you the following spells and skills:\r\n");

  strcpy(buf2, buf);

  for (sortpos = 1; sortpos <= MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");
      break;
    }
    if (GET_LEVEL(ch) >= spell_info[i].min_level[(int) GET_CLASS(ch)] &&
        guild_teaches(&gm_index[guild_nr], i)) {
      sprintf(buf, "%-20s ", spell_info[i].name);

      /* get the skill cost */
      base_price = spell_info[i].min_cost[(int) GET_CLASS(ch)] + 
                   get_gm_price_add(&gm_index[guild_nr], i);

      copper     = base_price * GM_CHARGE(guild_nr) *
                   (GET_SKILL(ch,i) + 1) * (GET_SKILL(ch, i) + 1);
      
      silver = gold = platinum = 0;

      min_coins(&copper, &silver, &gold, &platinum);

      memset(skill_cost, 0, sizeof(skill_cost));

      if(GET_SKILL(ch, i) < 95) {
          if(platinum > 0 && (gold > 0 || silver > 0 || copper > 0) )
            sprintf(END_OF(skill_cost), "%dp ", platinum);
          else if(platinum > 0)
            sprintf(END_OF(skill_cost), "%dp  ", platinum);
        
          if(gold > 0 && (silver > 0 || copper > 0) )
            sprintf(END_OF(skill_cost), "%dg ", gold);
          else if(gold > 0)
            sprintf(END_OF(skill_cost), "%dg  ", gold);

          if(silver > 0 && copper > 0)
            sprintf(END_OF(skill_cost), "%ds ", silver);
          else if(silver > 0)
            sprintf(END_OF(skill_cost), "%ds  ", silver);

          if(copper > 0)
            sprintf(END_OF(skill_cost), "%dc  ", copper);
      } else {
        sprintf(skill_cost, "-  ");
      }    

      sprintf(END_OF(buf), "%20s", skill_cost);

      sprintf(END_OF(buf), "%s%s", skill_how_good(GET_SKILL(ch, i)), skill_level(GET_SKILL(ch, i)));
                                   /* The above, ^ should always be safe to do. */
      if (GET_LEVEL(ch) > LVL_IMMORT) 
        sprintf(END_OF(buf), "(%2d%%-%2d%%)\r\n", 
                             get_gm_lowest_taught(&gm_index[guild_nr], i), 
                             get_gm_highest_taught(&gm_index[guild_nr], i));
      else
        strcat(buf, "\r\n");
        
      strcat(buf2, buf);    
    }
  }

  page_string(ch->desc, buf2, 1);
}

void list_skills(struct char_data *ch)
{
  int i, sortpos;

  sprintf(buf, "You know of the following %ss:\r\n", SPLSKL(ch));

  strcpy(buf2, buf);

  for (sortpos = 1; sortpos <= MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");
      break;
    }
    if (GET_LEVEL(ch) >= spell_info[i].min_level[(int) GET_CLASS(ch)]) {
      sprintf(buf, "%-30s ", spell_info[i].name);
      sprintf(END_OF(buf), "%s%s\r\n", skill_how_good(GET_SKILL(ch, i)), skill_level(GET_SKILL(ch, i)));
      strcat(buf2, buf);    /* The above, ^ should always be safe to do. */
    }
  }

  page_string(ch->desc, buf2, 1);
}

SPECIAL(guild_prac)
{
  int skill_num, percent, copperamt, base_price;

  /***  Let's see which guildmaster we're dealing with ***/

  struct char_data *keeper = (struct char_data *) me;
  guild_rnum guild_nr;

  for (guild_nr = 0; guild_nr < top_guild; guild_nr++)
    if (GM_TRAINER(guild_nr) == keeper->nr)
       break;

  if (guild_nr >= top_guild)
     return (FALSE);

  if (GM_FUNC(guild_nr))    /*check secondary if any, used to require guild_gain */
     if ((GM_FUNC(guild_nr)) (ch, me, cmd, arg))
        return(TRUE);

  if (IS_NPC(ch) || (!CMD_IS("practice") && !CMD_IS("gain")))
    return 0;

  if (CMD_IS("gain")) { /* We might want some level checks here */
    if (GET_LEVEL(ch) < GM_LOWEST_GAIN_LEV(guild_nr)) {
      sprintf(buf2, GM_NO_GAIN_LOW(guild_nr) ? GM_NO_GAIN_LOW(guild_nr) : MSG_DEFAULT_NO_GAIN_LOW, GET_NAME(ch));
      do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
      return 0;
    } else if (GM_HIGHEST_GAIN_LEV(guild_nr) && (GET_LEVEL(ch) > GM_HIGHEST_GAIN_LEV(guild_nr))) {
      sprintf(buf2, GM_NO_GAIN_HIGH(guild_nr) ? GM_NO_GAIN_HIGH(guild_nr) : MSG_DEFAULT_NO_GAIN_HIGH, GET_NAME(ch));
      do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
      return 0;
    } else 
      return (guild_gain(ch, keeper)) ;
  }
  
  skip_spaces(&argument);

  /*** Is the GM able to train?    ****/

  if (!AWAKE(keeper))
    return (FALSE);

  if (!(is_gm_ok(keeper, ch, guild_nr)))
    return 1;

  if (!*argument) {
    what_does_gm_know(guild_nr, ch);
    return 1;
  }

  skill_num = find_skill_num(argument);

  /**** Can the player learn the skill if the GM knows it?  ****/ 

  if (skill_num < 1 ||
      GET_LEVEL(ch) < spell_info[skill_num].min_level[(int) GET_CLASS(ch)]) {
    sprintf(buf, "You do not know of that %s.\r\n", SPLSKL(ch));
    send_to_char(buf, ch);
    return 1;
  }

  /****  Does the GM know the skill the player wants to learn?  ****/

  if (!(guild_teaches(&gm_index[guild_nr], skill_num))) {
    sprintf(buf2, GM_NO_SKILL(guild_nr), GET_NAME(ch));
    do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
    return 1;
  }

  /****  Is the player sufficiently skilled to get taught ?  ****/

  if (get_gm_lowest_taught(&gm_index[guild_nr], skill_num) > GET_SKILL(ch, skill_num)) {
    sprintf(buf2, GM_TOO_LOW(guild_nr), GET_NAME(ch));
    do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
    return 1;
  }

  /****  Can the GM teach the skill so high?  ****/

  if (get_gm_highest_taught(&gm_index[guild_nr], skill_num) < GET_SKILL(ch, skill_num)) {
    sprintf(buf2, GM_TOO_HIGH(guild_nr), GET_NAME(ch));
    do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
    return 1;
  }

  /****  Is the player maxxed out with the skill?  ****/

  if (GET_SKILL(ch, skill_num) >= LEARNED(ch)) {
     sprintf(buf2, "%s You are already learned in that area.", 
                   GET_NAME(ch));
     do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
    return 1;
  }

  /* can the char comprehend this just yet ? */

  if ( GET_SKILL(ch, skill_num) >= 
       (((GET_LEVEL(ch) + 1 ) - 
          spell_info[skill_num].min_level[(int) GET_CLASS(ch)] ) * 15)) {
     sprintf(buf2, "%s You can't yet learn more about '%s', come back when you are more experienced", 
                   GET_NAME(ch), spell_info[skill_num].name);
     do_tell(keeper, buf2, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
     return 1;
  }

  /****  Does the Player have enough gold to train?  ****/
  base_price = spell_info[skill_num].min_cost[(int) GET_CLASS(ch)] + get_gm_price_add(&gm_index[guild_nr], skill_num);
  copperamt = base_price * GM_CHARGE(guild_nr) *
              (GET_SKILL(ch,skill_num) + 1) * (GET_SKILL(ch, skill_num) + 1);

  /* seems the character didn't bring the cash */
  if (get_money(ch) < copperamt) {
    sprintf(buf1, GM_NO_GOLD(guild_nr) ? GM_NO_GOLD(guild_nr) : MSG_DEFAULT_NOT_ENOUGH_GOLD, GET_NAME(ch));
    do_tell(keeper, buf1, cmd_tell, SCMD_IGNORE_SOUNDPROOF);
    return 1;
  }

/****  If we've made it this far, then its time to practice  ****/

  send_to_char("You practice for a while...\r\n", ch);

  pay_for_service(ch, copperamt);

  percent = GET_SKILL(ch, skill_num);
  percent += MIN(MAXGAIN(ch), MAX(MINGAIN(ch), int_app[GET_INT(ch)].learn));

  SET_SKILL(ch, skill_num, MIN(LEARNED(ch), percent));

  if (GET_SKILL(ch, skill_num) >= LEARNED(ch))
    send_to_char("You are now learned in that area.\r\n", ch);
 
  save_char(ch); /* to avoid moneyloops, Exp loops, etc.*/
  return 1;
}

/**** This function is here just because I'm extremely paranoid.  Take it
      out if you aren't ;)  ****/
void clear_skills(guild_rnum index)
{
  if (GM_SKILLS(index)) {
    struct train_info *temp, *next;
    for (temp = GM_SKILLS(index);temp;temp = next) {
      next = temp->next;
      free(temp);
    }
    GM_SKILLS(index) = NULL;
  }  

}
/****  This is ripped off of read_line from shop.c.  They could be combined. But why? ****/

void read_gm_line(FILE * gm_f, char *string, void *data)
{
  if (!get_line(gm_f, buf)) {
    fprintf(stderr, "Error in guild #%d - no line!\n", GM_NUM(top_guild));
    exit(1);
  } else if (!sscanf(buf, string, data)) {
    fprintf(stderr, "Error in guild #%d - line:%s\n", GM_NUM(top_guild), buf);
    exit(1);
  }
}

void boot_the_guilds(FILE * gm_f, char *filename, int rec_count)
{
  char error_buf[150];
  char line[250], temp_bits[40];
  int t1, t2, t3, t4;
  int done = 0;
  struct train_info *newskill;
  
  sprintf(error_buf, "beginning of GM file %s", filename);

  get_line(gm_f, line);
  while (!done) {
    if (*line == '#') {        /* New Trainer */
      sscanf(line, "#%d (%*s)\n", &t1);
      sprintf(error_buf, "GM #%d in GM file %s", t1, filename);
      if (!top_guild)
        CREATE(gm_index, struct guild_master_data, rec_count);

      clear_skills(top_guild);
      GM_NUM(top_guild) = t1;

      get_line(gm_f, line);
      if (strn_cmp(line, "Skill list", 10)) {
        fprintf(stderr, "Error in guild #%d. No Skills section!\n", GM_NUM(top_guild));
        exit(1);
      }
      get_line(gm_f, line);
      if (!sscanf(line, "%d %d %d %d", &t1, &t2, &t3, &t4)) {
        fprintf(stderr, "Error in guild #%d - line:%s\n", GM_NUM(top_guild), line);
        exit(1);
      }
           while( t1 != -1) {
                CREATE(newskill, struct train_info, 1);
                newskill->next = GM_SKILLS(top_guild);
                GM_SKILLS(top_guild) = newskill;
                newskill->skillnum       = t1;
                newskill->lowest_taught  = t2;
                newskill->highest_taught = t3;
                newskill->price_add      = t4;
                
                get_line(gm_f, buf);
                if (!sscanf(buf, "%d %d %d %d", &t1, &t2, &t3, &t4)) {
                  fprintf(stderr, "Error in guild #%d - line:%s\n", GM_NUM(top_guild), buf);
                  exit(1);
                }
           }
                 
      gm_index[top_guild].no_such_skill = fread_string(gm_f, error_buf);
      gm_index[top_guild].not_enough_gold = fread_string(gm_f, error_buf);
      gm_index[top_guild].not_taught_so_high = fread_string(gm_f, error_buf);
      gm_index[top_guild].not_taught_so_low = fread_string(gm_f, error_buf);
      gm_index[top_guild].class_not_trained = fread_string(gm_f, error_buf);
      gm_index[top_guild].race_not_trained = fread_string(gm_f, error_buf);
      gm_index[top_guild].align_not_trained = fread_string(gm_f, error_buf);
      gm_index[top_guild].undead_living_wrong = fread_string(gm_f, error_buf);

      read_gm_line(gm_f, "Charge : %f", &GM_CHARGE(top_guild));

      read_gm_line(gm_f, "GMnum  : %d", &GM_TRAINER(top_guild));

      GM_TRAINER(top_guild) = real_mobile(GM_TRAINER(top_guild));
      read_gm_line(gm_f, "Withwho: %s", temp_bits);
      GM_WITH_WHO(top_guild) = asciiflag_conv(temp_bits);

      read_gm_line(gm_f, "Open   : %d", &GM_OPEN(top_guild));
      read_gm_line(gm_f, "Close  : %d", &GM_CLOSE(top_guild));

      /* special handling - added gain limits here */
      get_line(gm_f, line);
      if (sscanf(line, "Gainlvl: %d %d", 
                       &GM_LOWEST_GAIN_LEV(top_guild), 
                       &GM_HIGHEST_GAIN_LEV(top_guild)) != 2) {
        GM_LOWEST_GAIN_LEV(top_guild) = 0;
        GM_HIGHEST_GAIN_LEV(top_guild) = 0;
        GM_NO_GAIN_LOW(top_guild) = str_dup(MSG_DEFAULT_NO_GAIN_LOW); /* use defaults */
        GM_NO_GAIN_HIGH(top_guild) = str_dup(MSG_DEFAULT_NO_GAIN_HIGH);
      } else {
        gm_index[top_guild].no_gain_low_msg = fread_string(gm_f, error_buf);
        gm_index[top_guild].no_gain_high_msg = fread_string(gm_f, error_buf);
        get_line(gm_f, line); /* for next pass */
      }
      GM_FUNC(top_guild) = 0;
      top_guild++;
    } else {
      if (*line == '$')        /* EOF */
        done = TRUE;
    }
  }
}


void assign_the_gms(void)
{
  guild_rnum index;

  for (index = 0; index < top_guild; index++) {
    if (GM_TRAINER(index) == NOBODY)
       continue ;
    if (mob_index[GM_TRAINER(index)].func)
       GM_FUNC(index) = mob_index[GM_TRAINER(index)].func;
    mob_index[GM_TRAINER(index)].func = guild_prac;
  }
}

/* yet another function that was already written in shop.c that
   didn't need to be re-invented ;)  */

char *customer_string2(guild_rnum gm_nr, int detailed)
{
  int index, cnt = 1;
  static char buf[256];

  *buf = 0;
  for (index = 0; *trade_letters[index] != '\n'; index++, cnt *= 2)
    if ((GM_WITH_WHO(gm_nr) & cnt))
      if (detailed) {
        if (*buf)
          strcat(buf, ", ");
        strcat(buf, trade_letters[index]);
      } else
        sprintf(END_OF(buf), "%c", *trade_letters[index]);
    else if (!detailed)
      strcat(buf, "_");

  return (buf);
}


void list_all_gms(struct char_data *ch)
{
  guild_rnum gm_nr;

  strcpy(buf, "\r\n");
  for (gm_nr = 0; gm_nr < top_guild; gm_nr++) {
    if (!(gm_nr % 19)) {
      strcat(buf, "Virtual   G.Master    Charge     Members\r\n");
      strcat(buf, "----------------------------------------\r\n");
    }
    sprintf(buf2, " %6d    ", GM_NUM(gm_nr));

    if (GM_TRAINER(gm_nr) == NOBODY)
      strcpy(buf1, "<NONE>   ");
    else
      sprintf(buf1, "%6d   ", mob_index[GM_TRAINER(gm_nr)].vnum);

    sprintf(END_OF(buf2), "%s   %5.2f    ", buf1, GM_CHARGE(gm_nr));
    strcat(buf2, customer_string2(gm_nr, FALSE));

    sprintf(END_OF(buf), "%s\r\n", buf2);
  }

  page_string(ch->desc, buf, 1);
}

void list_detailed_gm(struct char_data *ch, guild_rnum gm_nr)
{
  int i, j;
  char temp[21];
  
    if (GM_TRAINER(gm_nr) == NOBODY)
      strcpy(buf1, "<NONE>");
    else
      sprintf(buf1, "%6d  %s", mob_index[GM_TRAINER(gm_nr)].vnum, mob_proto[GM_TRAINER(gm_nr)].player.short_descr);

   sprintf(buf, " Guild Master: %s\r\n", buf1);
   sprintf(END_OF(buf), " Hours: %4d to %4d,  Surcharge: %5.2f\r\n",
      GM_OPEN(gm_nr), GM_CLOSE(gm_nr), GM_CHARGE(gm_nr));
   sprintf(END_OF(buf), " Whom will train: %s\r\n",  customer_string2(gm_nr, TRUE));
   /* now for the REAL reason why someone would want to see a GM :) */

   strcat(buf, " The GM can teach the following skills and spells:\r\n");

   *buf2 = '\0';
   for (i = 0, j = 0; i <= MAX_SKILLS; i++) {
     if (guild_teaches(&gm_index[gm_nr], i)) {
        snprintf(temp, 21, "%s", spell_info[i].name);
        sprintf(END_OF(buf2), "[%2d-%2d](%2d) %-20s%s", 
                              get_gm_lowest_taught(&gm_index[gm_nr], i),
                              get_gm_highest_taught(&gm_index[gm_nr], i),
                              get_gm_price_add(&gm_index[gm_nr], i),
                              temp,
                              (!(++j % 2) ? "\r\n" : ""));
     }
   }

  strcat(buf, buf2);

  page_string(ch->desc, buf, 1);
}

  

void show_gm(struct char_data *ch, char *arg)
{
  guild_rnum gm_nr = NOBODY;
  
  skip_spaces(&arg);
  
  if (!*arg) {
    list_all_gms(ch);
    return;
  } 
  if ((gm_nr = real_gm(atoi(arg))) == NOBODY) {
    send_to_char("Illegal guild master number.\n\r", ch);
    return;
  }
  list_detailed_gm(ch, gm_nr);
}

void show_skill(struct char_data *ch, char *arg)
{
  int skillnum;
  guild_rnum rgm;
  char temp[25];
  
  if (!arg) {
    send_to_char("You must supply a skill name to list.\r\n", ch);
    return;
  }
  
  if ((skillnum = find_skill_num(arg)) == -1) {
    send_to_char("No such skill.\r\n", ch);
    return;
  }

  sprintf(buf, "'%s' is taught by the following guildmasters:\r\n"
               "Guild Mob                          From To Add  Customers\r\n"
               "------------------------------------------------------------------\r\n",
               spell_info[skillnum].name);
               
  for (rgm = 0;rgm < top_guild;rgm ++) {
    if (guild_teaches(&gm_index[rgm], skillnum)) {
      /* special handling of guildmaster short desc to make format work */
      snprintf(temp, 20, "%s", (GM_TRAINER(rgm) == NOBODY) ? "none" : mob_proto[GM_TRAINER(rgm)].player.short_descr);
      sprintf(buf2, "%-5d [%-5d] %-21s %2d  %2d  %2d  %s\r\n",
              GM_NUM(rgm),
              (GM_TRAINER(rgm) == NOBODY) ? -1 : mob_index[GM_TRAINER(rgm)].vnum,
              temp,
              get_gm_lowest_taught(&gm_index[rgm], skillnum),
              get_gm_highest_taught(&gm_index[rgm], skillnum),
              get_gm_price_add(&gm_index[rgm], skillnum),
              customer_string2(rgm, FALSE));
      strcat(buf, buf2);
    }
  }
  page_string(ch->desc, buf, 1);              
}
