/* ************************************************************************
*   File: Guild.h                                                         *
*  Usage: GuildMaster's: Definition for everything needed by shop.c       *
*                                                                         *
* Written by Jason Goodwin.   jgoodwin@expert.cc.purdue.edu               *
************************************************************************ */

struct train_info {
  int skillnum;               /* The skillnum */
  int lowest_taught;          /* The minimum the char must have learned the skill to be taught */
  int highest_taught;         /* The maximum this teacher teaches */
  int price_add;              /* Is this skill more expensive here ? */

  struct train_info *next;    /* next skill.. */
};

struct guild_master_data {
  guild_vnum num;                        /* virtual number of the guild */
  struct train_info *skills;             /* linked list of skills taught */
  int number_of_skills;                  /* The number of skills taught */
  float charge;                          /*charge * skill level^2 * level = how much we'll charge */
  char *no_such_skill;                   /* message when we don't teach that skill */
  char *not_enough_gold;                 /* message when the student doesn't have enough gold */
  char *not_taught_so_high;              /* message when the guildmaster cannot teach further */
  char *not_taught_so_low;               /* message when the guildmaster cannot teach so low */
  char *class_not_trained;               /* message when the student is the wrong class */
  char *race_not_trained;                /* message when the sutdent is of the wrong race */
  char *align_not_trained;               /* message when the student is of the wrong alignment */
  char *undead_living_wrong;             /* message when the student is of the wrong type */
  char *no_gain_low_msg;                 /* message when the student is too low level to gain */
  char *no_gain_high_msg;                /* message when the student is too high level to gain */
  mob_vnum gm;                           /* GM's vnum */
  bitvector_t with_who;                  /* whom we like */
  int open, close;                       /* when we will train - not currently used !*/
  SPECIAL(*func);                        /* secondary spec_proc for the GM */
  int lowest_level_for_gain;            /* lowest level we will allow to level here */
  int highest_level_for_gain;           /* highest level we will allow to level here */
};

/*-------------------------------------------------------------------*/
/*. Handy  macros  for use with guild rnum . */

#define GM_NUM(i)                 (gm_index[i].num)
#define GM_CHARGE(i)              (gm_index[i].charge)
#define GM_SKILLS(i)              (gm_index[i].skills)
#define GM_TRAINER(i)             (gm_index[i].gm)
#define GM_WITH_WHO(i)            (gm_index[i].with_who)
#define GM_OPEN(i)                (gm_index[i].open)
#define GM_CLOSE(i)               (gm_index[i].close)
#define GM_FUNC(i)                (gm_index[i].func)
#define GM_LOWEST_GAIN_LEV(i)     (gm_index[i].lowest_level_for_gain)
#define GM_HIGHEST_GAIN_LEV(i)    (gm_index[i].highest_level_for_gain)

#define GM_NO_SKILL(i)            (gm_index[i].no_such_skill)
#define GM_NO_GOLD(i)             (gm_index[i].not_enough_gold)
#define GM_TOO_HIGH(i)            (gm_index[i].not_taught_so_high)
#define GM_TOO_LOW(i)             (gm_index[i].not_taught_so_low)
#define GM_CLASS_NOT_TRAINED(i)   (gm_index[i].class_not_trained)
#define GM_RACE_NOT_TRAINED(i)    (gm_index[i].race_not_trained)
#define GM_ALIGN_NOT_TRAINED(i)   (gm_index[i].align_not_trained)
#define GM_UNDEAD_LIVING_WRONG(i) (gm_index[i].undead_living_wrong)
#define GM_NO_GAIN_LOW(i)         (gm_index[i].no_gain_low_msg)
#define GM_NO_GAIN_HIGH(i)        (gm_index[i].no_gain_high_msg)
/*-------------------------------------------------------------------*/
/*. Handy  macros  for use with *guild_master_data . */

#define G_NUM(guild)                  ((guild)->num)
#define G_CHARGE(guild)               ((guild)->charge)
#define G_SKILLS(guild)               ((guild)->skills)
#define G_TRAINER(guild)              ((guild)->gm)
#define G_WITH_WHO(guild)             ((guild)->with_who)
#define G_OPEN(guild)                 ((guild)->open)
#define G_CLOSE(guild)                ((guild)->close)
#define G_FUNC(guild)                 ((guild)->func)
#define G_LOWEST_GAIN_LEV(guild)      ((guild)->lowest_level_for_gain)
#define G_HIGHEST_GAIN_LEV(guild)     ((guild)->highest_level_for_gain)

#define G_NO_SKILL(guild)             ((guild)->no_such_skill)
#define G_NO_GOLD(guild)              ((guild)->not_enough_gold)
#define G_TOO_HIGH(guild)             ((guild)->not_taught_so_high)
#define G_TOO_LOW(guild)              ((guild)->not_taught_so_low)
#define G_CLASS_NOT_TRAINED(guild)    ((guild)->class_not_trained)
#define G_RACE_NOT_TRAINED(guild)     ((guild)->race_not_trained)
#define G_ALIGN_NOT_TRAINED(guild)    ((guild)->align_not_trained)
#define G_UNDEAD_LIVING_WRONG(guild)  ((guild)->undead_living_wrong)
#define G_NO_GAIN_LOW(guild)          ((guild)->no_gain_low_msg)
#define G_NO_GAIN_HIGH(guild)         ((guild)->no_gain_high_msg)

/* Whom will we train (we'll just use the TRADE_ bitvector for convenience) */
#define TRAIN_GOOD(i)         (IS_SET((GM_WITH_WHO(i)), TRADE_NOGOOD))
#define TRAIN_EVIL(i)         (IS_SET((GM_WITH_WHO(i)), TRADE_NOEVIL))
#define TRAIN_NEUTRAL(i)      (IS_SET((GM_WITH_WHO(i)), TRADE_NONEUTRAL))
#define TRAIN_MAGE(i)         (IS_SET((GM_WITH_WHO(i)), TRADE_NOMAGIC_USER))
#define TRAIN_CLERIC(i)       (IS_SET((GM_WITH_WHO(i)), TRADE_NOCLERIC))
#define TRAIN_THIEF(i)        (IS_SET((GM_WITH_WHO(i)), TRADE_NOTHIEF))
#define TRAIN_WARRIOR(i)      (IS_SET((GM_WITH_WHO(i)), TRADE_NOWARRIOR))
#define TRAIN_VAMPIRE(i)      (IS_SET((GM_WITH_WHO(i)), TRADE_NOVAMPIRE))
#define TRAIN_BARBARIAN(i)    (IS_SET((GM_WITH_WHO(i)), TRADE_NOBARBARIAN))
#define TRAIN_PSIONICIST(i)   (IS_SET((GM_WITH_WHO(i)), TRADE_NOPSIONICIST))
#define TRAIN_PALADIN(i)      (IS_SET((GM_WITH_WHO(i)), TRADE_NOPALADIN))
#define TRAIN_RANGER(i)       (IS_SET((GM_WITH_WHO(i)), TRADE_NORANGER))
#define TRAIN_DRUID(i)        (IS_SET((GM_WITH_WHO(i)), TRADE_NODRUID))
#define TRAIN_LICH(i)         (IS_SET((GM_WITH_WHO(i)), TRADE_NOLICH))
#define TRAIN_DOOMPRIEST(i)   (IS_SET((GM_WITH_WHO(i)), TRADE_NODOOMPRIEST))
#define TRAIN_DEATHKNIGHT(i)  (IS_SET((GM_WITH_WHO(i)), TRADE_NODEATHKNIGHT))
#define TRAIN_UNDEAD(i)       (IS_SET((GM_WITH_WHO(i)), TRADE_NOUNDEAD))
#define TRAIN_LIVING(i)       (IS_SET((GM_WITH_WHO(i)), TRADE_NOLIVING))

#define MSG_TRAINER_NOT_OPEN           "I'm busy! Come back later!"
#define MSG_TRAINER_NO_SEE_CH          "I don't train someone I can't see!"

#define MSG_DEFAULT_NO_SUCH_SKILL      "%s I've never heard of that skill!"
#define MSG_DEFAULT_NOT_ENOUGH_GOLD    "%s You cannot afford to pay my charge!"
#define MSG_DEFAULT_NOT_TAUGHT_SO_HIGH "%s I don't know that any better than you."
#define MSG_DEFAULT_NOT_TAUGHT_SO_LOW  "%s You must have a basic knowledge of that before I'll teach you."
#define MSG_DEFAULT_CLASS_NOT_TRAINED  "%s Seek another guild, if you want to train. I don't train your class."
#define MSG_DEFAULT_RACE_NOT_TRAINED   "%s I don't like the looks of you. Go somewhere that likes your race better!"
#define MSG_DEFAULT_ALIGN_NOT_TRAINED  "%s Get out, before I get mad!"
#define MSG_DEFAULT_UNDEAD_LIVING      "%s Yeach! I don't like your type!"
#define MSG_DEFAULT_NO_GAIN_LOW        "%s Go away! You're beneath me!"
#define MSG_DEFAULT_NO_GAIN_HIGH       "%s With your skills, you should teach me!"
  
#define LEARNED_LEVEL   0       /* % known which is considered "learned" */
#define MAX_PER_PRAC    1       /* max percent gain in skill per practice */
#define MIN_PER_PRAC    2       /* min percent gain in skill per practice */
#define PRAC_TYPE       3       /* should it say 'spell' or 'skill'?     */


#define LEARNED(ch) (prac_params[LEARNED_LEVEL][(int)GET_CLASS(ch)])
#define MINGAIN(ch) (prac_params[MIN_PER_PRAC][(int)GET_CLASS(ch)])
#define MAXGAIN(ch) (prac_params[MAX_PER_PRAC][(int)GET_CLASS(ch)])
#define SPLSKL(ch)  (prac_types[prac_params[PRAC_TYPE][(int)GET_CLASS(ch)]])

/* Local prototypes */

int guild_teaches(struct guild_master_data *guild, int skill_num);
int compare_spells(const void *x, const void *y);
void sort_spells(void);
const char *skill_how_good(int percent);
const char *skill_level(int percent);
int get_gm_price_add(struct guild_master_data *guild, int skill_num);
int get_gm_highest_taught(struct guild_master_data *guild, int skill_num);
int get_gm_lowest_taught(struct guild_master_data *guild, int skill_num);
guild_rnum real_gm(guild_vnum vgm_num);
void gedit_setup_new(struct descriptor_data *d);
void gedit_parse(struct descriptor_data *d, char *arg);
void gedit_disp_menu(struct descriptor_data *d);
void gedit_no_train_menu(struct descriptor_data *d);
void gedit_save_internally(struct descriptor_data *d);
void gedit_save_to_disk(struct descriptor_data *d);
void copy_gm(struct guild_master_data *tgm, struct guild_master_data *fgm);
void free_gm_strings(struct guild_master_data *guild);
void free_gm(struct guild_master_data *guild);
void gedit_modify_string(char **str, char *new_str);
void show_skill(struct char_data *ch, char *arg);
