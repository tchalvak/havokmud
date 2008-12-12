
/**
 * @file
 * @brief OLD!  List of all function prototypes.
 */

#ifndef _protos_h
#define _protos_h

#include "structs.h"
#include "area.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "hash.h"
#include "heap.h"
#include "interpreter.h"
#include "script.h"
#include "spells.h"
#include "trap.h"
#include "utils.h"
#include "vt100c.h"
#include "parser.h"
#include "dimd.h"
#include <stdio.h>

/*
 * From Heap.c 
 */

void            SmartStrCpy(char *s1, const char *s2);
void            StringHeap(char *string, struct StrHeap *Heap);
struct StrHeap *InitHeap(void);
void            DisplayStringHeap(struct StrHeap *Heap,
                                  struct char_data *ch, int type, int d);

/*
 * From Opinion.c 
 */

void            FreeHates(struct char_data *ch);
void            FreeFears(struct char_data *ch);
int             RemHated(struct char_data *ch, struct char_data *pud);
int             AddHated(struct char_data *ch, struct char_data *pud);
int AddHatred(struct char_data *ch, OpinionType_t parm_type, int parm);
int             RemHatred(struct char_data *ch, unsigned short bitv);
int             Hates(struct char_data *ch, struct char_data *v);
int             Fears(struct char_data *ch, struct char_data *v);
int             RemFeared(struct char_data *ch, struct char_data *pud);
int             AddFeared(struct char_data *ch, struct char_data *pud);
int AddFears(struct char_data *ch, OpinionType_t parm_type, int parm);
struct char_data *FindAHatee(struct char_data *ch);
struct char_data *FindAFearee(struct char_data *ch);
void            ZeroHatred(struct char_data *ch, struct char_data *v);
void            ZeroFeared(struct char_data *ch, struct char_data *v);
void            DeleteFears(struct char_data *ch);

/*
 * From Sound.c 
 */

int             RecGetObjRoom(struct obj_data *obj);
void            MakeNoise(int room, char *local_snd, char *distant_snd);
void            MakeSound(int pulse);

/*
 * From Trap.c 
 */

void            do_settrap(struct char_data *ch, char *arg, int cmd);
int             CheckForMoveTrap(struct char_data *ch, int dir);
int             CheckForInsideTrap(struct char_data *ch,
                                   struct obj_data *i);
int             CheckForAnyTrap(struct char_data *ch, struct obj_data *i);
int             CheckForGetTrap(struct char_data *ch, struct obj_data *i);
int             TriggerTrap(struct char_data *ch, struct obj_data *i);
void            FindTrapDamage(struct char_data *v, struct obj_data *i);
void            TrapDamage(struct char_data *v, int damtype, int amnt,
                           struct obj_data *t);
void            TrapDam(struct char_data *v, int damtype, int amnt,
                        struct obj_data *t);
void            TrapTeleport(struct char_data *v);
void            TrapSleep(struct char_data *v);
void            InformMess(struct char_data *v);

/*
 * From act.comm.c 
 */
void            do_telepathy(struct char_data *ch, char *argument,
                             int cmd);
void            do_new_say(struct char_data *ch, char *argument, int cmd);
void            do_speak(struct char_data *ch, char *argument, int cmd);
void            UpdateScreen(struct char_data *ch, int update);
void            InitScreen(struct char_data *ch);
void            do_report(struct char_data *ch, char *argument, int cmd);
void            do_say(struct char_data *ch, char *argument, int cmd);
void            do_shout(struct char_data *ch, char *argument, int cmd);
void            do_bid(struct char_data *ch, char *argument, int cmd);
void            do_auction(struct char_data *ch, char *argument, int cmd);
void            do_yell(struct char_data *ch, char *argument, int cmd);
void            do_commune(struct char_data *ch, char *argument, int cmd);
void            do_tell(struct char_data *ch, char *argument, int cmd);
void            do_gtell(struct char_data *ch, char *argument, int cmd);
void            do_split(struct char_data *ch, char *argument, int cmd);
void            do_whisper(struct char_data *ch, char *argument, int cmd);
void            do_ask(struct char_data *ch, char *argument, int cmd);
void            do_write(struct char_data *ch, char *argument, int cmd);
void            do_pray(struct char_data *ch, char *argument, int cmd);
char           *RandomWord(void);
void            do_sign(struct char_data *ch, char *argument, int cmd);
void            do_move(struct char_data *ch, char *argument, int cmd);
void            do_enter(struct char_data *ch, char *argument, int cmd);
void            do_rest(struct char_data *ch, char *argument, int cmd);
void            do_stand(struct char_data *ch, char *argument, int cmd);
void            do_sit(struct char_data *ch, char *argument, int cmd);
void            do_lock(struct char_data *ch, char *argument, int cmd);
void            do_unlock(struct char_data *ch, char *argument, int cmd);
void            do_pick(struct char_data *ch, char *argument, int cmd);
void            do_sleep(struct char_data *ch, char *argument, int cmd);
void            do_wake(struct char_data *ch, char *argument, int cmd);
void            do_trans(struct char_data *ch, char *argument, int cmd);
void            do_follow(struct char_data *ch, char *argument, int cmd);
void            do_open(struct char_data *ch, char *argument, int cmd);
void            do_close(struct char_data *ch, char *argument, int cmd);
void            do_enter(struct char_data *ch, char *argument, int cmd);
void            do_leave(struct char_data *ch, char *argument, int cmd);
void            do_talk(struct char_data *ch, char *argument, int cmd);

/*
 * From act.info.c 
 */
void            do_sea_commands(struct char_data *ch, char *argument,
                                int cmd);

void            list_groups(struct char_data *ch, char *argument, int cmd);
void            do_command_list(struct char_data *ch, char *arg, int cmd);
void            do_show_skill(struct char_data *ch, char *arg, int cmd);
void            do_resize(struct char_data *ch, char *arg, int cmd);
void            do_display(struct char_data *ch, char *arg, int cmd);
int             singular(struct obj_data *o);
void            show_obj_to_char(struct obj_data *object,
                                 struct char_data *ch, int mode);
void            show_mult_obj_to_char(struct obj_data *o,
                                      struct char_data *ch, int m, int n);
void            list_obj_in_heap(struct obj_data *list,
                                 struct char_data *ch);
void list_obj_in_list( LinkedList_t *list, int offset, struct char_data *ch,
                       int mode );
void            list_obj_to_char(struct obj_data *l, struct char_data *ch,
                                 int m, bool show);
void            show_char_to_char(struct char_data *i,
                                  struct char_data *ch, int mode);
void            show_mult_char_to_char(struct char_data *i,
                                       struct char_data *ch, int m, int n);
void            list_char_in_room(struct char_data *list,
                                  struct char_data *ch);
void            list_char_to_char(struct char_data *list,
                                  struct char_data *ch, int mode);

void            list_exits_in_room(struct char_data *ch);

void            do_look(struct char_data *ch, char *argument, int cmd);
void            do_read(struct char_data *ch, char *argument, int cmd);
void            do_examine(struct char_data *ch, char *argument, int cmd);
void            do_exits(struct char_data *ch, char *argument, int cmd);
void            do_score(struct char_data *ch, char *argument, int cmd);
void            do_time(struct char_data *ch, char *argument, int cmd);
void            do_weather(struct char_data *ch, char *argument, int cmd);
void            do_help(struct char_data *ch, char *argument, int cmd);

void            do_list_zones(struct char_data *ch, char *argument,
                              int cmd);
void            do_recallhome(struct char_data *victim, char *argument,
                              int cmd);

void            do_wizhelp(struct char_data *ch, char *argument, int cmd);
void            do_who(struct char_data *ch, char *argument, int cmd);
void            do_users(struct char_data *ch, char *argument, int cmd);
void            do_inventory(struct char_data *ch, char *argument,
                             int cmd);
void            do_equipment(struct char_data *ch, char *argument,
                             int cmd);
void            do_credits(struct char_data *ch, char *argument, int cmd);
void            do_news(struct char_data *ch, char *argument, int cmd);
void            do_info(struct char_data *ch, char *argument, int cmd);
void            do_wizlist(struct char_data *ch, char *argument, int cmd);
void            do_iwizlist(struct char_data *ch, char *argument, int cmd);
int             which_number_mobile(struct char_data *ch,
                                    struct char_data *mob);
char           *numbered_person(struct char_data *ch,
                                struct char_data *person);
void            do_where(struct char_data *ch, char *argument, int cmd);
void            do_levels(struct char_data *ch, char *argument, int cmd);
void            do_consider(struct char_data *ch, char *argument, int cmd);
void            do_spells(struct char_data *ch, char *argument, int cmd);
void            do_weapons(struct char_data *ch, char *argument, int cmd);
void            do_allweapons(struct char_data *ch, char *argument,
                              int cmd);
void            do_world(struct char_data *ch, char *argument, int cmd);
void            do_attribute(struct char_data *ch, char *argument,
                             int cmd);
void            do_resistances(struct char_data *ch, char *argument,
                               int cmd);
void            do_value(struct char_data *ch, char *argument, int cmd);
char           *ArmorDesc(int a);
char           *HitRollDesc(int a);
char           *DamRollDesc(int a);
char           *DescRatio(float f);
char           *DescDamage(float dam);
char           *DescAttacks(float a);
char           *SpeedDesc(int a);

/*
 * From act.obj1.c 
 */

void            get(struct char_data *ch, struct obj_data *obj_object,
                    struct obj_data *sub_object);
void            do_get(struct char_data *ch, char *argument, int cmd);
void            do_drop(struct char_data *ch, char *argument, int cmd);
void            do_put(struct char_data *ch, char *argument, int cmd);
int             newstrlen(char *p);
void            do_give(struct char_data *ch, char *argument, int cmd);

/*
 * From act.obj2.c 
 */

void            weight_change_object(struct obj_data *obj, int weight);
void            name_from_drinkcon(struct obj_data *obj);
void            name_to_drinkcon(struct obj_data *obj, int type);
void            do_drink(struct char_data *ch, char *argument, int cmd);
void            do_eat(struct char_data *ch, char *argument, int cmd);
void            do_pour(struct char_data *ch, char *argument, int cmd);
void            do_sip(struct char_data *ch, char *argument, int cmd);
void            do_taste(struct char_data *ch, char *argument, int cmd);
void            perform_wear(struct char_data *ch,
                             struct obj_data *obj_object, long keyword);
int             IsRestricted(int Mask, int Class);
void            wear(struct char_data *ch, struct obj_data *obj_object,
                     long keyword);
void            do_wear(struct char_data *ch, char *argument, int cmd);
void            do_wield(struct char_data *ch, char *argument, int cmd);
void            do_draw(struct char_data *ch, char *argument, int cmd);
void            do_grab(struct char_data *ch, char *argument, int cmd);
void            do_remove(struct char_data *ch, char *argument, int cmd);

/*
 * From act.off.c 
 */
void            do_weapon_load(struct char_data *ch, char *argument,
                               int cmd);
void            do_run(struct char_data *ch, char *argument, int cmd);
struct char_data *get_char_near_room_vis(struct char_data *ch, char *name,
                                         long next_room);
void            do_fire(struct char_data *ch, char *argument, int cmd);
void            do_throw(struct char_data *ch, char *argument, int cmd);
int             range_hit(struct char_data *ch, struct char_data *targ,
                          int rng, struct obj_data *missile, int tdir,
                          int max_rng);
struct char_data *get_char_linear(struct char_data *ch, char *arg, int *rf,
                                  int *df);
void            do_hit(struct char_data *ch, char *argument, int cmd);
void            do_kill(struct char_data *ch, char *argument, int cmd);
void            do_backstab(struct char_data *ch, char *argument, int cmd);
void            do_order(struct char_data *ch, char *argument, int cmd);
void            do_dismiss(struct char_data *ch, char *arg, int cmd);
void            do_flee(struct char_data *ch, char *argument, int cmd);
void            do_bash(struct char_data *ch, char *argument, int cmd);
void            do_leg_sweep(struct char_data *ch, char *argument,
                             int cmd);
void            do_rescue(struct char_data *ch, char *argument, int cmd);
void            do_assist(struct char_data *ch, char *argument, int cmd);
void            do_kick(struct char_data *ch, char *argument, int cmd);
void            do_wimp(struct char_data *ch, char *argument, int cmd);
void            do_breath(struct char_data *ch, char *argument, int cmd);
void            do_shoot(struct char_data *ch, char *argument, int cmd);
void            do_springleap(struct char_data *ch, char *argument,
                              int cmd);
void            do_quivering_palm(struct char_data *ch, char *arg,
                                  int cmd);
void            kick_messages(struct char_data *ch,
                              struct char_data *victim, int damage);
void            do_disengage(struct char_data *ch, char *argument,
                             int cmd);
/*
 * From act.other.c 
 */
void            do_set_bprompt(struct char_data *ch, char *argument,
                               int cmd);

void            do_set_prompt(struct char_data *ch, char *argument,
                              int cmd);
void            do_set_flags(struct char_data *ch, char *argument,
                             int cmd);
void            do_auto(struct char_data *ch, char *argument, int cmd);
void            do_set_afk(struct char_data *ch, char *argument, int cmd);
void            do_set_quest(struct char_data *ch, char *argument,
                             int cmd);
void            do_set_consent(struct char_data *ch, char *argument,
                               int cmd);
void            do_gain(struct char_data *ch, char *argument, int cmd);
void            do_guard(struct char_data *ch, char *argument, int cmd);
void            do_junk(struct char_data *ch, char *argument, int cmd);
void            do_qui(struct char_data *ch, char *argument, int cmd);
void            do_title(struct char_data *ch, char *argument, int cmd);
void            do_quit(struct char_data *ch, char *argument, int cmd);
void            do_save(struct char_data *ch, char *argument, int cmd);
void            do_not_here(struct char_data *ch, char *argument, int cmd);
void            do_sneak(struct char_data *ch, char *argument, int cmd);
void            do_hide(struct char_data *ch, char *argument, int cmd);
void            do_steal(struct char_data *ch, char *argument, int cmd);
void            do_practice(struct char_data *ch, char *arg, int cmd);
void            do_brief(struct char_data *ch, char *argument, int cmd);
void            do_compact(struct char_data *ch, char *argument, int cmd);
void            do_group(struct char_data *ch, char *argument, int cmd);
void            do_quaff(struct char_data *ch, char *argument, int cmd);
void            do_recite(struct char_data *ch, char *argument, int cmd);
void            do_use(struct char_data *ch, char *argument, int cmd);
void            do_plr_noshout(struct char_data *ch, char *argument,
                               int cmd);
void            do_plr_nogossip(struct char_data *ch, char *argument,
                                int cmd);
void            do_plr_noauction(struct char_data *ch, char *argument,
                                 int cmd);
void            do_plr_notell(struct char_data *ch, char *argument,
                              int cmd);
void            do_alias(struct char_data *ch, char *arg, int cmd);
void            Dismount(struct char_data *ch, struct char_data *h, int pos);
void            do_mount(struct char_data *ch, char *arg, int cmd);
void            do_promote(struct char_data *ch, char *arg, int cmd);

/*
 * From act.social.c 
 */

int             find_action(int cmd);
void            do_action(struct char_data *ch, char *argument, int cmd);
void            do_insult(struct char_data *ch, char *argument, int cmd);
void            do_pose(struct char_data *ch, char *argument, int cmd);

/*
 * From act.wizard.c 
 */
void            do_osave(struct char_data *ch, char *argument, int cmd);
void            do_finger(struct char_data *ch, char *argument, int cmd);

void            do_WorldSave(struct char_data *ch, char *argument,
                             int cmd);
void            do_god_interven(struct char_data *ch, char *argument,
                                int cmd);

void            do_nuke(struct char_data *ch, char *argument, int cmd);
void            do_scan(struct char_data *ch, char *arg, int cmd);
void            do_ghost(struct char_data *ch, char *argument, int cmd);

void            do_spot(struct char_data *ch, char *argument, int cmd);
void            do_cset(struct char_data *ch, char *arg, int cmd);
void            do_auth(struct char_data *ch, char *argument, int cmd);
void            do_passwd(struct char_data *ch, char *argument,
                          int cmdnum);
void            do_setsev(struct char_data *ch, char *arg, int cmd);
void            dsearch(char *string, char *tmp);
void            do_bamfin(struct char_data *ch, char *arg, int cmd);
void            do_bamfout(struct char_data *ch, char *arg, int cmd);
void            do_zload(struct char_data *ch, char *argument, int cmdnum);
void            do_zclean(struct char_data *ch, char *argument,
                          int cmdnum);
void            do_zsave(struct char_data *ch, char *argument, int cmdnum);
void            do_highfive(struct char_data *ch, char *argument, int cmd);
void            do_addhost(struct char_data *ch, char *argument,
                           int command);
void            do_removehost(struct char_data *ch, char *argument,
                              int command);
void            do_listhosts(struct char_data *ch, char *argument,
                             int command);
void            do_silence(struct char_data *ch, char *argument, int cmd);
void            do_rload(struct char_data *ch, char *argument, int cmd);
void            do_rsave(struct char_data *ch, char *argument, int cmd);
void            do_emote(struct char_data *ch, char *argument, int cmd);
void            do_echo(struct char_data *ch, char *argument, int cmd);
void            do_system(struct char_data *ch, char *argument, int cmd);
void            do_trans(struct char_data *ch, char *argument, int cmd);
void            do_at(struct char_data *ch, char *argument, int cmd);
void            do_goto(struct char_data *ch, char *argument, int cmd);
void            do_stat(struct char_data *ch, char *argument, int cmd);
void            do_set(struct char_data *ch, char *argument, int cmd);
void            do_snoop(struct char_data *ch, char *argument, int cmd);
void            do_switch(struct char_data *ch, char *argument, int cmd);
void            do_return(struct char_data *ch, char *argument, int cmd);
void            do_force(struct char_data *ch, char *argument, int cmd);
void            do_flux(struct char_data *ch, char *argument, int cmd);

void            do_genstatue(struct char_data *ch, char *argument,
                             int cmd);

void            do_mend(struct char_data *ch, char *argument, int cmd);
void            do_load(struct char_data *ch, char *argument, int cmd);
void            purge_one_room(int rnum, struct room_data *rp, int *range);
void            do_purge(struct char_data *ch, char *argument, int cmd);
void            do_start(struct char_data *ch);
void            do_advance(struct char_data *ch, char *argument, int cmd);
void            do_reroll(struct char_data *ch, char *argument, int cmd);
void            do_restore(struct char_data *ch, char *argument, int cmd);
void            do_noshout(struct char_data *ch, char *argument, int cmd);
void            do_nohassle(struct char_data *ch, char *argument, int cmd);
void            do_stealth(struct char_data *ch, char *argument, int cmd);
void            do_show(struct char_data *ch, char *argument, int cmd);
void            do_invis(struct char_data *ch, char *argument, int cmd);
void            do_create(struct char_data *ch, char *argument, int cmd);
void            CreateOneRoom(int loc_nr);
void            do_set_log(struct char_data *ch, char *arg, int cmd);
void            do_event(struct char_data *ch, char *arg, int cmd);
void            do_beep(struct char_data *ch, char *argument, int cmd);
void            do_disconnect(struct char_data *ch, char *argument,
                              int cmd);
void            do_freeze(struct char_data *ch, char *argument, int cmd);
void            do_drainlevel(struct char_data *ch, char *argument,
                              int cmd);
void            do_permission(struct char_data *ch, char *argument,
                              int cmd);
void            do_qtrans(struct char_data *ch, char *argument, int cmd);
void            do_set_nooutdoor(struct char_data *ch, char *argument,
                                 int cmd);
void            do_setwtype(struct char_data *ch, char *argument, int cmd);
void            do_setsound(struct char_data *ch, char *argument, int cmd);
void            do_tweak(struct char_data *ch, char *argument, int cmd);

/*
 * From board.c 
 */

void            board_write_msg(struct char_data *ch, char *arg, int bnum);
int             board_display_msg(struct char_data *ch, char *arg,
                                  int bnum);
int             board_remove_msg(struct char_data *ch, char *arg,
                                 int bnum);
void            board_save_board(void);
void            board_load_board(void);
int             board_show_board(struct char_data *ch, char *arg,
                                 int bnum);
int             board(struct char_data *ch, int cmd, char *arg,
                      struct obj_data *obj, int type);
char           *fix_returns(char *text_string);
int             board_check_locks(int bnum, struct char_data *ch);

/*
 * From comm.c 
 */

int             __main(void);
void            close_socket_fd(int desc);
int             main(int argc, char **argv);
int             run_the_game(int port);
void            game_loop(int s);

void            write_to_output(char *txt, struct descriptor_data *t);

struct timeval  timediff(struct timeval *a, struct timeval *b);
void            flush_queues(struct descriptor_data *d);
int             init_socket(int port);
int             new_connection(int s);

int             new_descriptor(int s);
int             process_output(struct descriptor_data *t);
int             write_to_descriptor(int desc, char *txt);
int             process_input(struct descriptor_data *t);
void            close_sockets(int s);
void            close_socket(struct descriptor_data *d);
void            nonblock(int s);
void            coma(int s);
void            send_to_char(char *messg, struct char_data *ch);
void            save_all(void);
void            act(char *str, int hide_invisible, struct char_data *ch,
                    struct obj_data *obj, void *vict_obj, int type);
void            act2(char *str, int hide_invisible, struct char_data *ch,
                     struct obj_data *obj, void *vict_obj,
                     struct char_data *vict, int type);

void             raw_force_all(char *to_force);

/*
 * From constants.c 
 */

/*
 * From db.c 
 */
void            SaveTheWorld(void);
void            boot_db(void);
void            reset_time(void);
void            update_time(void);
void            build_player_index(void);
void            boot_saved_zones(void);
void            renum_zone_table(int spec_zone);
void            boot_zones(void);
int             init_counter(void);
void            zone_update(void);
void            reset_zone(int zone, int cmd);
int             is_empty(int zone_nr);
int             create_entry(char *name);
void            save_char(struct char_data *ch, sh_int load_room);
int             compare(struct player_index_element *arg1,
                        struct player_index_element *arg2);
void            free_char(struct char_data *ch);
void            ClearDeadBit(struct char_data *ch);
void            reset_char(struct char_data *ch);
void            clear_char(struct char_data *ch);
void            init_char(struct char_data *ch);
int             load(void);
void            gr(int s);
int             workhours(void);
void            reboot_text(struct char_data *ch, char *arg, int cmd);
void            FreeZone(int zone_nr);

/*
 * From fight.c 
 */

void            appear(struct char_data *ch);
int             LevelMod(struct char_data *ch, struct char_data *v,
                         int exp);
int             RatioExp(struct char_data *ch, struct char_data *victim,
                         int total);
void            load_messages(void);
void            update_pos(struct char_data *victim);
int             check_peaceful(struct char_data *ch, char *msg);
void            set_fighting(struct char_data *ch, struct char_data *vict);
void            stop_fighting(struct char_data *ch);
void            change_alignment(struct char_data *ch,
                                 struct char_data *victim);
void            death_cry(struct char_data *ch);
void            group_gain(struct char_data *ch, struct char_data *victim);
void            dam_message(int dam, struct char_data *ch,
                            struct char_data *victim, int w_type);
int             DamCheckDeny(struct char_data *ch,
                             struct char_data *victim, int type);
int             DamDetailsOk(struct char_data *ch, struct char_data *v,
                             int dam, int type);
int             SetCharFighting(struct char_data *ch, struct char_data *v);
int             SetVictFighting(struct char_data *ch, struct char_data *v);

void            WeaponSkillCheck(struct char_data *ch);

int             ClassDamBonus(struct char_data *ch, struct char_data *v,
                              int dam);
int             DamageTrivia(struct char_data *ch, struct char_data *v,
                             int dam, int type);
int             DoDamage(struct char_data *ch, struct char_data *v,
                         int dam, int type);
void            DamageMessages(struct char_data *ch, struct char_data *v,
                               int dam, int attacktype);
void            specdamage(struct char_data *ch, struct char_data *victim);

void            do_style(struct char_data *ch, char *argument, int cmd);

void            make_corpse(struct char_data *ch, int killedbytype);
void            raw_kill(struct char_data *ch, int killedbytype);
void            die(struct char_data *ch, int killedbytype);
int             DamageEpilog(struct char_data *ch,
                             struct char_data *victim, int killedbytype);

int             MissileDamage(struct char_data *ch,
                              struct char_data *victim, int dam,
                              int attacktype);
int             damage(struct char_data *ch, struct char_data *victim,
                       int dam, int attacktype);
int             GetWeaponType(struct char_data *ch,
                              struct obj_data **wielded);
int             Getw_type(struct obj_data *wielded);
int             HitCheckDeny(struct char_data *ch,
                             struct char_data *victim, int type,
                             int DistanceWeapon);
int             CalcThaco(struct char_data *ch);
int             HitOrMiss(struct char_data *ch, struct char_data *victim,
                          int calc_thaco);
void            MissVictim(struct char_data *ch, struct char_data *v,
                           int type, int w_type, int (*dam_func) ());
int             GetWeaponDam(struct char_data *ch, struct char_data *v,
                             struct obj_data *wielded);
int             LoreBackstabBonus(struct char_data *ch,
                                  struct char_data *v);
void            HitVictim(struct char_data *ch, struct char_data *v,
                          int dam, int type, int w_type,
                          int (*dam_func) ());

void            root_hit(struct char_data *ch, struct char_data *victim,
                         int type, int (*dam_func) (), int DistanceWeapon);
void            MissileHit(struct char_data *ch, struct char_data *victim,
                           int type);
void            hit(struct char_data *ch, struct char_data *victim,
                    int type);
void            perform_violence(int pulse);
struct char_data *FindVictim(struct char_data *ch);
struct char_data *FindAnyVictim(struct char_data *ch);
struct char_data *CreateAMob(struct char_data *mob, int vmobnum,
                             int MobAdjust, char *buf);
struct char_data *AttackRandomChar(struct char_data *mob);
int             doroomdamage(struct char_data *tempchar, int dam,
                             int attacktype);
void            BreakLifeSaverObj(struct char_data *ch);
void            BrittleCheck(struct char_data *ch, struct char_data *v,
                             int dam);
int             PreProcDam(struct char_data *ch, int type, int dam);
int             DamageOneItem(struct char_data *ch, int dam_type,
                              struct obj_data *obj);
void            MakeScrap(struct char_data *ch, struct char_data *v,
                          struct obj_data *obj);
void            DamageAllStuff(struct char_data *ch, int dam_type);
int             DamageItem(struct char_data *ch, struct obj_data *o,
                           int num);
int             ItemSave(struct obj_data *i, int dam_type);
int             DamagedByAttack(struct obj_data *i, int dam_type);
int             WeaponCheck(struct char_data *ch, struct char_data *v,
                            int type, int dam);
void            DamageStuff(struct char_data *v, int type, int dam);
int             GetItemDamageType(int type);
int             SkipImmortals(struct char_data *v, int amnt,
                              int attacktype);
void            WeaponSpell(struct char_data *c, struct char_data *v,
                            struct obj_data *obj, int type);

struct char_data *FindAnAttacker(struct char_data *ch);
struct char_data *FindMetaVictim(struct char_data *ch);
void            NailThisSucker(struct char_data *ch);
int             GetFormType(struct char_data *ch);
int             MonkDodge(struct char_data *ch, struct char_data *v,
                          int *dam);
int             SmithShield(struct char_data *ch, struct char_data *v,
                            struct obj_data *obj, int *dam);
void            BurnWings(struct char_data *ch);

/*
 * From handler.c 
 */

char           *fname(char *namelist);
int             split_string(char *str, char *sep, char **argv);
int             isname(const char *str, const char *namelist);
void            affect_modify(struct char_data *ch, byte loc, long mod,
                              long bitv, bool add);
void            affect_total(struct char_data *ch);
void            affect_to_char(struct char_data *ch,
                               struct affected_type *af);
void            affect_remove(struct char_data *ch,
                              struct affected_type *af);
void            affect_from_char(struct char_data *ch, int skill);
bool            affected_by_spell(struct char_data *ch, int skill);
void            affect_join(struct char_data *ch, struct affected_type *af,
                            bool avg_dur, bool avg_mod);
void            char_from_room(struct char_data *ch);
void            char_to_room(struct char_data *ch, long room);
int             apply_ac(struct char_data *ch, int eq_pos);
void            equip_char(struct char_data *ch, struct obj_data *obj,
                           int pos);
int             GiveMinStrToWield(struct obj_data *obj,
                                  struct char_data *ch);
struct obj_data *unequip_char(struct char_data *ch, int pos);
struct char_data *get_char_room(char *name, int room);
struct char_data *get_char(char *name);
struct char_data *get_char_num(int nr);
void            update_object(struct obj_data *obj, int use);
void            update_char_objects(struct char_data *ch);
void            extract_char(struct char_data *ch);
void            extract_char_smarter(struct char_data *ch, long saveroom);
struct char_data *get_char_room_vis(struct char_data *ch, char *name);
struct char_data *get_char_vis_world(struct char_data *ch, char *name,
                                     int *count);
struct char_data *get_char_vis(struct char_data *ch, char *name);
struct obj_data *create_money(int amount);
int             generic_find(char *arg, int bitvector,
                             struct char_data *ch,
                             struct char_data **tar_ch,
                             struct obj_data **tar_obj);
void            AddAffects(struct char_data *ch, struct obj_data *o);

/*
 * From hash.c 
 */

void            init_hash_table(struct hash_header *ht, int rec_size,
                                int table_size);
void            init_world(struct room_data *room_db[]);
void            destroy_hash_table(struct hash_header *ht, void (*gman) ());
void            _hash_enter(struct hash_header *ht, long key, void *data);
struct room_data *room_find(struct room_data *room_db[], long key);
void           *hash_find(struct hash_header *ht, long key);
int             room_enter(struct room_data *rb[], long key,
                           struct room_data *rm);
int             hash_enter(struct hash_header *ht, long key, void *data);
struct room_data *room_find_or_create(struct room_data *rb[], long key);
void           *hash_find_or_create(struct hash_header *ht, long key);
int             room_remove(struct room_data *rb[], long key);
void           *hash_remove(struct hash_header *ht, long key);
void            room_iterate(struct room_data *rb[], void (*func) (),
                             void *cdata);
void            hash_iterate(struct hash_header *ht, void (*func) (),
                             void *cdata);

/*
 * From interpreter.c 
 */
void            do_clone(struct char_data *ch, char *argument, int cmd);
void            do_mforce(struct char_data *ch, char *argument, int cmd);

void            commando(int number, int min_pos, void (*pointer) (),
                         int min_level);
int             old_search_block(char *argument, int begin, int length,
                                 char **list, int mode);
void            command_interpreter(struct char_data *ch, char *argument);
char           *get_argument_nofill(char *line_in, char **arg_out);
char           *get_argument_delim(char *line_in, char **arg_out, char delim);
int             is_number(char *str);
int             fill_word(char *argument);
int             is_abbrev(char *arg1, char *arg2);
int             special(struct char_data *ch, int cmd, char *arg);
void            assign_command_pointers(void);
int             find_name(char *name);
int             _parse_name(char *arg, char *name);
#ifdef TODO
void            nanny(struct descriptor_data *d, char *arg);
#endif

/*
 * From limits.c 
 */

char           *ClassTitles(struct char_data *ch);
int             graf(int age, int race, int p0, int p1, int p2, int p3,
                     int p4, int p5, int p6);
int             mana_limit(struct char_data *ch);
int             hit_limit(struct char_data *ch);
int             move_limit(struct char_data *ch);
int             mana_gain(struct char_data *ch);
int             hit_gain(struct char_data *ch);
int             move_gain(struct char_data *ch);
void            advance_level(struct char_data *ch, int class);
void            drop_level(struct char_data *ch, int class, int goddrain);
void            set_title(struct char_data *ch);
void            gain_exp(struct char_data *ch, int gain);
void            gain_exp_regardless(struct char_data *ch, int gain,
                                    int class);
void            gain_condition(struct char_data *ch, int condition,
                               int value);
void            check_idling(struct char_data *ch);

/*
 * From magic.c 
 */

void            heat_blind(struct char_data *ch);
void            spell_magic_missile(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_chill_touch(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_burning_hands(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_shocking_grasp(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_lightning_bolt(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_colour_spray(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_energy_drain(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_fireball(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_earthquake(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_dispel_evil(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_call_lightning(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_harm(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_armor(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_astral_walk(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_teleport(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_bless(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_blindness(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_create_food(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_create_water(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_cure_blind(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_cure_critic(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_cure_light(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_curse(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_detect_evil(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_detect_good(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_detect_invisibility(int level, struct char_data *ch,
                                          struct char_data *victim,
                                          struct obj_data *obj);
void            spell_detect_magic(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_detect_poison(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_enchant_weapon(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_enchant_armor(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_heal(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_invisibility(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_locate_object(int level, struct char_data *ch,
                                    struct char_data *victim, char *arg);
void            spell_poison(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            spell_protection_from_evil(int level,
                                           struct char_data *ch,
                                           struct char_data *victim,
                                           struct obj_data *obj);
void            spell_remove_curse(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_remove_poison(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_fireshield(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_sanctuary(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_sleep(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_strength(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_word_of_recall(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_summon(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            RawSummon(struct char_data *v, struct char_data *c);
void            spell_charm_person(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_charm_monster(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_sense_life(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_identify(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_fire_breath(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_paralyze_breath(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_repulsion_breath(int level, struct char_data *ch,
                                       struct char_data *victim,
                                       struct obj_data *obj);
void            spell_slow_breath(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_desertheat_breath(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_frost_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_acid_breath(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_gas_breath(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_lightning_breath(int level, struct char_data *ch,
                                       struct char_data *victim,
                                       struct obj_data *obj);
void            spell_dehydration_breath(int level, struct char_data *ch,
                                         struct char_data *victim,
                                         struct obj_data *obj);
void            spell_vapor_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_sound_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_shard_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_sleep_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_light_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_dark_breath(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_lozenge_breath(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
/*
 * Fom magic2.c 
 */

void            spell_resurrection(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_cause_light(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_cause_critical(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_cause_serious(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_cure_serious(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_mana(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_second_wind(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_flamestrike(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_dispel_good(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_turn(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_remove_paralysis(int level, struct char_data *ch,
                                       struct char_data *victim,
                                       struct obj_data *obj);
void            spell_holy_word(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_unholy_word(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_succor(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            spell_true_seeing(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_major_track(int level, struct char_data *ch,
                                  struct char_data *targ,
                                  struct obj_data *obj);
void            spell_minor_track(int level, struct char_data *ch,
                                  struct char_data *targ,
                                  struct obj_data *obj);
void            spell_poly_self(int level, struct char_data *ch,
                                struct char_data *mob,
                                struct obj_data *obj);
void            spell_minor_create(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_stone_skin(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_infravision(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_shield(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            spell_weakness(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_invis_group(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_acid_blast(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_cone_of_cold(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_ice_storm(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_meteor_swarm(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_Create_Monster(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_light(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_fly(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void            spell_fly_group(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_refresh(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_water_breath(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_cont_light(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_animate_dead(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *corpse);
void            spell_know_alignment(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_dispel_magic(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_paralyze(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_fear(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_calm(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_web(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void            spell_heroes_feast(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_conjure_elemental(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
void            spell_faerie_fire(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_faerie_fog(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_cacaodemon(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_geyser(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            spell_green_slime(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);

/*
 * From magic3.c 
 */

void            spell_tree_travel(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_transport_via_plant(int level, struct char_data *ch,
                                          struct char_data *victim,
                                          char *arg);
void            spell_plant_gate(int level, struct char_data *ch,
                                 struct char_data *victim, char *arg);
void            spell_speak_with_plants(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
void            spell_changestaff(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_pword_kill(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_pword_blind(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_chain_lightn(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_scare(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_haste(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_slow(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_familiar(int level, struct char_data *ch,
                               struct char_data **victim,
                               struct obj_data *obj);
void            spell_aid(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void            spell_holyword(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_golem(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_feeblemind(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_shillelagh(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_goodberry(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_flame_blade(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_animal_growth(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_insect_growth(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_creeping_death(int level, struct char_data *ch,
                                     struct char_data *victim, int dir);
void            spell_commune(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_animal_summon(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_elemental_summoning(int level, struct char_data *ch,
                                          struct char_data *victim,
                                          int spell);
void            spell_reincarnate(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_charm_veggie(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_veggie_growth(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_tree(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void            spell_animate_rock(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_travelling(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_animal_friendship(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
void            spell_invis_to_animals(int level, struct char_data *ch,
                                       struct char_data *victim,
                                       struct obj_data *obj);
void            spell_slow_poison(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_snare(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_entangle(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_barkskin(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_gust_of_wind(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_silence(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_warp_weapon(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_heat_stuff(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_dust_devil(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_sunray(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void            spell_know_monster(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_find_traps(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_firestorm(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_teleport_wo_error(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
void            spell_portal(int level, struct char_data *ch,
                             struct char_data *tmp_ch,
                             struct obj_data *obj);
void            spell_mount(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_dragon_ride(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);

/*
 * New Paladin spells 
 */
void            spell_holy_strength(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_enlightenment(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_wrath_god(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_pacifism(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_aura_power(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_holy_armor(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_circle_protection(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
/*
 * From magicutils.c 
 */

void            SwitchStuff(struct char_data *giver,
                            struct char_data *taker);
void            FailCharm(struct char_data *victim, struct char_data *ch);
void            FailSnare(struct char_data *victim, struct char_data *ch);
void            FailSleep(struct char_data *victim, struct char_data *ch);
void            FailPara(struct char_data *victim, struct char_data *ch);
void            FailCalm(struct char_data *victim, struct char_data *ch);

/*
 * From mobact.c 
 */
int             UseViolentHeldItem(struct char_data *ch);
void            mobile_guardian(struct char_data *ch);
void            mobile_wander(struct char_data *ch);
void            MobHunt(struct char_data *ch);
void            MobScavenge(struct char_data *ch);
void            check_mobile_activity(int pulse);
void            mobile_activity(struct char_data *ch);
int             SameRace(struct char_data *ch1, struct char_data *ch2);
int             AssistFriend(struct char_data *ch);
int             FindABetterWeapon(struct char_data *mob);
int             GetDamage(struct obj_data *w, struct char_data *ch);
int             GetDamBonus(struct obj_data *w);
int             GetHandDamage(struct char_data *ch);
int             MobFriend(struct char_data *ch, struct char_data *f);
void            PulseMobiles(int type);
void            DoScript(struct char_data *ch);
int             CommandSearch(char *arg);
void CommandAssign(char *arg, void (*p)());
void            CommandSetup(void);
void            noop(char *arg, struct char_data *ch);
void            end2(char *arg, struct char_data *ch);
void            sgoto(char *arg, struct char_data *ch);
void            do_act(char *arg, struct char_data *ch);
void            do_jmp(char *arg, struct char_data *ch);
void            do_jsr(char *arg, struct char_data *ch);
void            do_rts(char *arg, struct char_data *ch);
void            MobHit(struct char_data *, struct char_data *, int);

/*
 * From modify.c 
 */

void            string_add(struct descriptor_data *d, char *str);
void            quad_arg(char *arg, int *type, char *name, int *field,
                         char *string);
void            do_string(struct char_data *ch, char *arg, int cmd);
void            bisect_arg(char *arg, int *field, char *string);
void            do_edit(struct char_data *ch, char *arg, int cmd);

void            do_setskill(struct char_data *ch, char *arg, int cmd);
void            page_string(struct descriptor_data *d, char *str,
                            int keep_internal);

void            show_string(struct descriptor_data *d, char *input);
void            night_watchman(void);
void            check_reboot(void);

/*
 * From multiclass.c 
 */

int             GetClassLevel(struct char_data *ch, int class);
int             CountBits(int class);
int             OnlyClass(struct char_data *ch, int class);
int             MainClass(struct char_data *ch, int indicator);
int             BestFightingClass(struct char_data *ch);
int             HasFightingClass(struct char_data *ch);
int             BestThiefClass(struct char_data *ch);
int             BestMagicClass(struct char_data *ch);
int             GetSecMaxLev(struct char_data *ch);
int             GetALevel(struct char_data *ch, int which);
int             GetThirdMaxLev(struct char_data *ch);
int             GetTotLevel(struct char_data *ch);
void            StartLevels(struct char_data *ch);
int             BestClassIND(struct char_data *ch);
int             BestClassBIT(struct char_data *ch);

/*
 * From reception.c 
 */

void            add_obj_cost(struct char_data *ch, struct char_data *re,
                             struct obj_data *obj, struct obj_cost *cost);
void add_obj_cost_chain(struct char_data *ch, struct char_data *re,
                        struct obj_data *obj, struct obj_cost *cost);

bool            recep_offer(struct char_data *ch,
                            struct char_data *receptionist,
                            struct obj_cost *cost, int forcerent);

void            update_obj_file(void);
void            CountLimitedItems(struct obj_file_u *st);
void            PrintLimitedItems(void);
int             receptionist(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
void            zero_rent(struct char_data *ch);
void            ZeroRent(char *n);

/*
 * From security.c 
 */

int             SecCheck(char *arg, char *site);

/*
 * From shop.c 
 */

int             is_ok(struct char_data *keeper, struct char_data *ch,
                      int shop_nr);
int             trade_with(struct obj_data *item, int shop_nr);
int             shop_producing(struct obj_data *item, int shop_nr);
void            shopping_buy(char *arg, struct char_data *ch,
                             struct char_data *keeper, int shop_nr);
void            shopping_sell(char *arg, struct char_data *ch,
                              struct char_data *keeper, int shop_nr);
void            shopping_value(char *arg, struct char_data *ch,
                               struct char_data *keeper, int shop_nr);
void            shopping_list(char *arg, struct char_data *ch,
                              struct char_data *keeper, int shop_nr);
void            shopping_kill(char *arg, struct char_data *ch,
                              struct char_data *keeper, int shop_nr);
int             shop_keeper(struct char_data *ch, int cmd, char *arg,
                            char *mob, int type);
void            boot_the_shops(void);
void            assign_the_shopkeepers(void);

/*
 * From signals.c 
 */

void            signal_setup(void);
void            checkpointing(int signal);
void            shutdown_request(int signal);
void            hupsig(int signal);
void            logsig(int signal);

/*
 * From skills.c 
 */
void            do_sending(struct char_data *ch, char *argument, int cmd);
void            do_esp(struct char_data *ch, char *argument, int cmd);
void            do_psi_shield(struct char_data *ch, char *argument, int cmd);
void            do_force_rent(struct char_data *ch, char *argument, int cmd);
void            do_doorway(struct char_data *ch, char *argument, int cmd);
void            do_psi_portal(struct char_data *ch, char *argument, int cmd);
void            do_mindsummon(struct char_data *ch, char *argument, int cmd);
void            do_canibalize(struct char_data *ch, char *argument, int cmd);
void            do_flame_shroud(struct char_data *ch, char *argument, int cmd);
void            do_aura_sight(struct char_data *ch, char *argument, int cmd);
void            do_great_sight(struct char_data *ch, char *argument, int cmd);
void            do_hypnosis(struct char_data *ch, char *argument, int cmd);
void            do_scry(struct char_data *ch, char *argument, int cmd);
void            do_adrenalize(struct char_data *ch, char *argument, int cmd);
void            do_blast(struct char_data *ch, char *argument, int cmd);
void            do_invisibililty(struct char_data *ch, char *argument, int cmd);
void            do_meditate(struct char_data *ch, char *argument, int cmd);

void            do_holy_warcry(struct char_data *ch, char *argument, int cmd);
void            do_lay_on_hands(struct char_data *ch, char *argument, int cmd);
void            do_blessing(struct char_data *ch, char *argument, int cmd);
void            do_heroic_rescue(struct char_data *ch, char *arguement,
                                 int command);

void            do_brew(struct char_data *ch, char *argument, int cmd);
void            do_scribe(struct char_data *ch, char *argument, int cmd);

void            check_memorize(struct char_data *ch,
                               struct affected_type *af);
void            do_memorize(struct char_data *ch, char *argument, int cmd);
void            do_disarm(struct char_data *ch, char *argument, int cmd);
void            do_track(struct char_data *ch, char *argument, int cmd);
int             track(struct char_data *ch, struct char_data *vict);
int             dir_track(struct char_data *ch, struct char_data *vict);
void            donothing(void);
int             find_path(int in_room, int (*predicate) (), void *c_data,
                          int depth, int in_zone);
int             choose_exit_global(int in_room, int tgt_room, int depth);
int             choose_exit_in_zone(int in_room, int tgt_room, int depth);
void            slam_into_wall(struct char_data *ch,
                               struct room_direction_data *exitp);
void            do_doorbash(struct char_data *ch, char *arg, int cmd);
void            do_swim(struct char_data *ch, char *arg, int cmd);
int             SpyCheck(struct char_data *ch);
void            do_spy(struct char_data *ch, char *arg, int cmd);
int             remove_trap(struct char_data *ch, struct obj_data *trap);
void            do_feign_death(struct char_data *ch, char *arg, int cmd);
void            do_first_aid(struct char_data *ch, char *arg, int cmd);
void            do_disguise(struct char_data *ch, char *argument, int cmd);
void            do_climb(struct char_data *ch, char *arg, int cmd);
void            slip_in_climb(struct char_data *ch, int dir, int room);
void            do_tan(struct char_data *ch, char *arg, int cmd);
void            do_berserk(struct char_data *ch, char *arg, int cmd);
void            do_carve(struct char_data *ch, char *arg, int cmd);

void            do_find(struct char_data *ch, char *arg, int cmd);
int             FSkillCheck(struct char_data *ch, int fskill);
void            do_flowerfist(struct char_data *ch, char *argument, int cmd);
void            do_flurry(struct char_data *ch, char *argument, int cmd);
/*
 * From spec_assign.c 
 */
int             is_murdervict(struct char_data *ch);
int             fighter_mage(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             fighter_cleric(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             cleric_mage(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             arena_prep_room(struct char_data *ch, int cmd, char *arg,
                                struct room_data *rp, int type);
int             arena_arrow_dispel_trap(struct char_data *ch, int cmd,
                                        char *arg, struct room_data *rp,
                                        int type);
int             arena_fireball_trap(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             arena_dispel_trap(struct char_data *ch, int cmd, char *arg,
                                  struct room_data *rp, int type);

/*
 * From spec_procs.c 
 */

int             is_target_room_p(int room, void *tgt_room);
int             named_object_on_ground(int room, void *c_data);
char           *how_good(int percent);
int             GainLevel(struct char_data *ch, int class);
struct char_data *FindMobInRoomWithFunction(int room, int (*func) ());
int             MageGuildMaster(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             SorcGuildMaster(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             ClericGuildMaster(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             ThiefGuildMaster(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             WarriorGuildMaster(struct char_data *ch, int cmd,
                                   char *arg, struct char_data *mob,
                                   int type);
int             dump(struct char_data *ch, int cmd, char *arg,
                     struct room_data *rp, int type);
int             mayor(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             andy_wilcox(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             eric_johnson(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
void            exec_social(struct char_data *npc, char *cmd,
                            int next_line, int *cur_line, void **thing);
void            npc_steal(struct char_data *ch, struct char_data *victim);
int             snake(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             MidgaardCityguard(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             PaladinGuildGuard(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             GameGuard(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             GreyParamedic(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int types);
int             AmberParamedic(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             blink(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             MidgaardCitizen(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             ghoul(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             CarrionCrawler(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             WizardGuard(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             vampire(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             wraith(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             shadow(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             geyser(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             green_slime(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             DracoLich(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             Drow(struct char_data *ch, int cmd, char *arg,
                     struct char_data *mob, int type);
int             thief(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             guild_guard(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             Inquisitor(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             puff(struct char_data *ch, int cmd, char *arg,
                     struct char_data *mob, int type);
int             regenerator(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             replicant(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob1, int type);
int             Tytan(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             AbbarachDragon(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             fido(struct char_data *ch, int cmd, char *arg,
                     struct char_data *mob, int type);
int             janitor(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             tormentor(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             RustMonster(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             temple_labrynth_liar(struct char_data *ch, int cmd,
                                     char *arg, struct char_data *mob,
                                     int type);
int             temple_labrynth_sentry(struct char_data *ch, int cmd,
                                       char *arg, struct char_data *mob,
                                       int type);
int             Whirlwind(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             NudgeNudge(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             AGGRESSIVE(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             citizen(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             Ringwraith(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             WarrenGuard(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             zm_tired(struct char_data *zmaster);
int             zm_stunned_followers(struct char_data *zmaster);
void            zm_init_combat(struct char_data *zmaster,
                               struct char_data *target);
int             zm_kill_fidos(struct char_data *zmaster);
int             zm_kill_aggressor(struct char_data *zmaster);
int             zombie_master(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             pet_shops(struct char_data *ch, int cmd, char *arg,
                          struct room_data *rp, int type);
int             Fountain(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);
int             bank(struct char_data *ch, int cmd, char *arg,
                     struct room_data *rp, int type);
int             pray_for_items(struct char_data *ch, int cmd, char *arg,
                               struct room_data *rp, int type);
int             chalice(struct char_data *ch, int cmd, char *arg);
int             kings_hall(struct char_data *ch, int cmd, char *arg);
int             Donation(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);
int             House(struct char_data *ch, int cmd, char *arg,
                      struct room_data *rp, int type);
int             sisyphus(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             jabberwocky(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             flame(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             banana(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             paramedics(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             jugglernaut(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             delivery_elf(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             delivery_beast(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             Keftab(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             StormGiant(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             fighter(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             NewThalosMayor(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             SultanGuard(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             NewThalosCitizen(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             NewThalosGuildGuard(struct char_data *ch, int cmd,
                                    char *arg, struct char_data *mob,
                                    int type);
int             magic_user2(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             MordGuard(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             MordGuildGuard(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             CaravanGuildGuard(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             StatTeller(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             Thunder_Fountain(struct char_data *ch, int cmd, char *arg,
                                 struct room_data *rp, int type);
int             rope_room(struct char_data *ch, int cmd, char *arg,
                          struct room_data *rp, int type);
void            ThrowChar(struct char_data *ch, struct char_data *v,
                          int dir);
int             ThrowerMob(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             Tyrannosaurus_swallower(struct char_data *ch, int cmd,
                                        char *arg, struct char_data *mob,
                                        int type);
int             soap(struct char_data *ch, int cmd, char *arg,
                     struct obj_data *tobj, int type);
int             nodrop(struct char_data *ch, int cmd, char *arg,
                       struct obj_data *tobj, int type);
int             lattimore(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             coldcaster(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             trapper(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             trogcook(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             shaman(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             golgar(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             troguard(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             ghostsoldier(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             Valik(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             guardian(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             web_slinger(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);

/*
 * From spec_procs2.c 
 */
int             snake_avt(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             virgin_sac(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             EvilBlade(struct char_data *ch, int cmd, char *arg,
                          struct obj_data *tobj, int type);
int             PrisonGuard(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             acid_monster(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             FireBreather(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             FrostBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             AcidBreather(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             GasBreather(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             LightningBreather(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             DehydBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             VaporBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             SoundBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             ShardBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             SleepBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             LightBreather(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             DarkBreather(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             magic_user_imp(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             snake_guardians(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             RangerGuildmaster(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             PsiGuildmaster(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             PaladinGuildmaster(struct char_data *ch, int cmd,
                                   char *arg, struct char_data *mob,
                                   int type);

int             death_knight(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             DogCatcher(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             mad_gertruda(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             mad_cyrus(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             raven_iron_golem(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);

int             ghost(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             druid_protector(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             Magic_Fountain(struct char_data *ch, int cmd, char *arg,
                               struct room_data *rp, int type);
int             DruidAttackSpells(struct char_data *ch,
                                  struct char_data *vict, int level);
int             Summoner(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             monk(struct char_data *ch, int cmd, char *arg,
                     struct char_data *mob, int type);
char           *invert(char *arg1);
int             jive_box(struct char_data *ch, int cmd, char *arg,
                         struct obj_data *obj, int type);
int             godsay(struct char_data *ch, int cmd, char *argument,
                       struct obj_data *obj, int type);
int             magic_user(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             druid(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             necromancer(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             cleric(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             ninja_master(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             barbarian_guildmaster(struct char_data *ch, int cmd,
                                      char *arg, struct char_data *mob,
                                      int type);
int             mage_specialist_guildmaster(struct char_data *ch, int cmd,
                                            char *arg,
                                            struct char_data *mob,
                                            int type);
int             cleric_specialist_guildmaster(struct char_data *ch,
                                              int cmd, char *arg,
                                              struct char_data *mob,
                                              int type);
int             RepairGuy(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             Samah(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             BitterBlade(struct char_data *ch, int cmd, char *arg,
                            struct obj_data *tobj, int type);
int             MakeQuest(struct char_data *ch, struct char_data *gm,
                          int Class, char *arg, int cmd);
int             AbyssGateKeeper(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             creeping_death(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
void            Submit(struct char_data *ch, struct char_data *t);
void            SayHello(struct char_data *ch, struct char_data *t);
void            GreetPeople(struct char_data *ch);
int             GenericCityguardHateUndead(struct char_data *ch, int cmd,
                                           char *arg,
                                           struct char_data *mob,
                                           int type);
int             GenericCityguard(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             PrydainGuard(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
struct breath_victim *choose_victims(struct char_data *ch,
                                     struct char_data *first_victim);
void            free_victims(struct breath_victim *head);
void            breath_weapon(struct char_data *ch,
                              struct char_data *target, int mana_cost,
                              void (*func) ());
void            use_breath_weapon(struct char_data *ch,
                                  struct char_data *target, int cost,
                                  void (*func) ());
int             BreathWeapon(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             sailor(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             loremaster(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             hunter(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             monk_master(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             DruidGuildMaster(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             Devil(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             Demon(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
void            DruidHeal(struct char_data *ch, int level);
void            DruidTree(struct char_data *ch);
void            DruidMob(struct char_data *ch);
int             DruidChallenger(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             MonkChallenger(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             druid_challenge_prep_room(struct char_data *ch, int cmd,
                                          char *arg, struct room_data *rp,
                                          int type);
int             druid_challenge_room(struct char_data *ch, int cmd,
                                     char *arg, struct room_data *rp,
                                     int type);
int             monk_challenge_room(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             monk_challenge_prep_room(struct char_data *ch, int cmd,
                                         char *arg, struct room_data *rp,
                                         int type);
int             portal(struct char_data *ch, int cmd, char *arg,
                       struct obj_data *obj, int type);
int             scraps(struct char_data *ch, int cmd, char *arg,
                       struct obj_data *obj, int type);
int             attack_rats(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             DragonHunterLeader(struct char_data *ch, int cmd,
                                   char *arg, struct char_data *mob,
                                   int type);
int             HuntingMercenary(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             SlotMachine(struct char_data *ch, int cmd, char *arg,
                            struct obj_data *obj, int type);
int             astral_portal(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             DwarvenMiners(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             real_rabbit(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             real_fox(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             antioch_grenade(struct char_data *ch, int cmd, char *arg,
                                struct obj_data *obj, int type);
int             ABShout(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             AvatarPosereisn(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             dispel_room(struct char_data *ch, int cmd, char *arg,
                            struct room_data *rp, int type);
int             fiery_alley(struct char_data *ch, int cmd, char *arg,
                            struct room_data *rp, int type);

/*
 * From spec_procs3.c 
 */
int             archer_instructor(struct char_data *ch, int cmd, char *arg,
                                  struct char_data *mob, int type);
int             archer(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             Beholder(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             BerserkerItem(struct char_data *ch, int cmd, char *arg,
                              struct obj_data *obj, int type);
int             AntiSunItem(struct char_data *ch, int cmd, char *arg,
                            struct obj_data *obj, int type);
int             ChurchBell(struct char_data *ch, int cmd, char *arg,
                           struct room_data *rp, int type);
int             Slavalis(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             TreeThrowerMob(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             PostMaster(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
struct room_data *forward_square(struct room_data *room);
struct room_data *back_square(struct room_data *room);
struct room_data *left_square(struct room_data *room);
struct room_data *right_square(struct room_data *room);
struct room_data *forward_left_square(struct room_data *room);
struct room_data *forward_right_square(struct room_data *room);
struct room_data *back_right_square(struct room_data *room);
struct room_data *back_left_square(struct room_data *room);
struct char_data *square_contains_enemy(struct room_data *square);
int             square_contains_friend(struct room_data *square);
int             square_empty(struct room_data *square);
int             chess_game(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             AcidBlob(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             avatar_celestian(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             lizardman_shaman(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             village_princess(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             strahd_vampire(struct char_data *ch, int cmd, char *arg,
                               struct char_data *mob, int type);
int             strahd_zombie(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             banshee(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             baby_bear(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             timnus(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             winger(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);

int             Barbarian(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             Paladin(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             Psionist(struct char_data *ch, int cmd, char *arg,
                         struct char_data *mob, int type);
int             Ranger(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);

int             goblin_sentry(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             lich_church(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             medusa(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             GoodBlade(struct char_data *ch, int cmd, char *arg,
                          struct obj_data *tobj, int type);
int             NeutralBlade(struct char_data *ch, int cmd, char *arg,
                             struct obj_data *tobj, int type);

/*
 * Procs for lennyas sin zone 
 */
int             sinpool(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             pridemirror(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             sin_spawner(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             sinbarrel(struct char_data *ch, int cmd, char *arg,
                          struct room_data *rp, int type);
int             altarofsin(struct char_data *ch, int cmd, char *argument,
                           struct obj_data *obj, int type);
int             applepie(struct char_data *ch, int cmd, char *argument,
                         struct obj_data *obj, int type);
int             trinketcount(struct char_data *ch, int cmd, char *argument,
                             struct obj_data *obj, int type);
int             trinketlooter(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             guardian_sin(struct char_data *ch, struct char_data *vict);
int             lag_room(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);
int             greed_disabler(struct char_data *ch, int cmd, char *arg,
                               struct room_data *rp, int type);
int             pride_disabler(struct char_data *ch, int cmd, char *arg,
                               struct room_data *rp, int type);
int             pride_remover_one(struct char_data *ch, int cmd, char *arg,
                                  struct room_data *rp, int type);
int             pride_remover_two(struct char_data *ch, int cmd, char *arg,
                                  struct room_data *rp, int type);
int             pride_remover_three(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             pride_remover_four(struct char_data *ch, int cmd,
                                   char *arg, struct room_data *rp,
                                   int type);
int             pride_remover_five(struct char_data *ch, int cmd,
                                   char *arg, struct room_data *rp,
                                   int type);
int             pride_remover_six(struct char_data *ch, int cmd, char *arg,
                                  struct room_data *rp, int type);
int             pride_remover_seven(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             lust_sinner(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
/*
 * mermaid's lagoon 
 */
int             mermaid(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);

int             preperationproc(struct char_data *ch, int cmd, char *arg,
                                struct room_data *rp, int type);
int             chestproc(struct char_data *ch, int cmd, char *argument,
                          struct obj_data *obj, int type);
/*
 * Assigns for the Town of Sundhaven 
 */
int             sund_earl(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             hangman(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             blinder(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             silktrader(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             butcher(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);
int             idiot(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             athos(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             stu(struct char_data *ch, int cmd, char *arg,
                    struct char_data *mob, int type);
int             marbles(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);

/*
 * Assigns for Thunder Mountain Keep 
 */
int             bahamut_home(struct char_data *ch, int cmd, char *arg,
                             struct room_data *rp, int type);
int             bahamut_prayer(struct char_data *ch,
                               struct char_data *vict);
int             bahamut_armor(struct char_data *ch,
                              struct char_data *vict);
int             cronus_pool(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             tmk_guard(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob, int type);
int             braxis_swamp_dragon(struct char_data *ch, int cmd,
                                    char *arg, struct char_data *mob,
                                    int type);
int             nadia(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);
int             mime_jerry(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             elamin(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type);
int             goblin_chuirgeon(struct char_data *ch, int cmd, char *arg,
                                 struct char_data *mob, int type);
int             zork(struct char_data *ch, int cmd, char *arg,
                     struct char_data *mob, int type);
int             thunder_blue_pill(struct char_data *ch, int cmd, char *arg,
                                  struct room_data *rp, int type);
int             thunder_black_pill(struct char_data *ch, int cmd,
                                   char *arg, struct room_data *rp,
                                   int type);
int             thunder_sceptre_one(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             thunder_sceptre_two(struct char_data *ch, int cmd,
                                    char *arg, struct room_data *rp,
                                    int type);
int             starving_man(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             grayswandir(struct char_data *ch, int cmd, char *arg,
                            struct room_data *rp, int type);

/*
 * Assigns for Sentinel's The Estate 
 */
int             janaurius(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob);

/*
 * necro spell procedure 
 */
int             vampiric_embrace(struct char_data *ch,
                                 struct char_data *vict);

/*
 * Assigns for Leaves of Silver 
 */
int             Jessep(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob);
int             Tysha(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);

/*
 * From spell_parser.c 
 */
void            spello(int nr, byte beat, byte pos, byte mlev, byte clev,
                       byte dlev, byte slev, byte plev, byte rlev,
                       byte ilev, ubyte mana, int tar, void (*func)(),
                       sh_int sf, byte brewable, byte blev, byte nlev);

int             SPELL_LEVEL(struct char_data *ch, int sn);
void            affect_update(int pulse);
void            update_mem(int pulse);
void            stop_memorizing(struct char_data *ch);
void            do_id(struct char_data *ch, char *argument, int cmd);
bool            circle_follow(struct char_data *ch,
                              struct char_data *victim);
void            stop_follower(struct char_data *ch);
void            die_follower(struct char_data *ch);
void            add_follower(struct char_data *ch,
                             struct char_data *leader);
void            say_spell(struct char_data *ch, int si);
bool            saves_spell(struct char_data *ch, sh_int save_type);
bool            ImpSaveSpell(struct char_data *ch, sh_int save_type,
                             int mod);
char           *skip_word(char *string);
void            do_cast(struct char_data *ch, char *argument, int cmd);
void            assign_spell_pointers(void);
void            SpellWearOffSoon(int s, struct char_data *ch);
void            SpellWearOff(int s, struct char_data *ch);
void            check_decharm(struct char_data *ch);
int             check_falling(struct char_data *ch);
void            check_drowning(struct char_data *ch);
void            check_falling_obj(struct obj_data *obj, int room);
int             check_nature(struct char_data *i);

/*
 * From spells1.c 
 */

void            cast_burning_hands(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            cast_call_lightning(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *victim,
                                    struct obj_data *tar_obj);
void            cast_chill_touch(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *victim,
                                 struct obj_data *tar_obj);
void            cast_shocking_grasp(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *victim,
                                    struct obj_data *tar_obj);
void            cast_colour_spray(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_earthquake(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *victim,
                                struct obj_data *tar_obj);
void            cast_energy_drain(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_fireball(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *victim,
                              struct obj_data *tar_obj);
void            cast_harm(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *victim,
                          struct obj_data *tar_obj);
void            cast_lightning_bolt(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *victim,
                                    struct obj_data *tar_obj);
void            cast_acid_blast(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *victim,
                                struct obj_data *tar_obj);
void            cast_cone_of_cold(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_ice_storm(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *victim,
                               struct obj_data *tar_obj);
void            cast_meteor_swarm(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_flamestrike(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *victim,
                                 struct obj_data *tar_obj);
void            cast_magic_missile(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            cast_cause_light(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *victim,
                                 struct obj_data *tar_obj);
void            cast_cause_serious(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            cast_cause_critic(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_geyser(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *victim,
                            struct obj_data *tar_obj);
void            cast_green_slime(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *victim,
                                 struct obj_data *tar_obj);

/*
 * From spells2.c 
 */

void            cast_resurrection(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_major_track(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_minor_track(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_mana(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_armor(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_stone_skin(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_astral_walk(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_teleport(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_bless(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_infravision(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_true_seeing(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_blindness(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_light(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_cont_light(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_web(int level, struct char_data *ch, char *arg,
                         int type, struct char_data *tar_ch,
                         struct obj_data *tar_obj);
void            cast_control_weather(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_create_food(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_create_water(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_water_breath(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_flying(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_fly_group(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_heroes_feast(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_cure_blind(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_cure_critic(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_cure_light(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_cure_serious(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_refresh(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_second_wind(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_shield(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_curse(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_detect_evil(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_detect_good(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_detect_invisibility(int level, struct char_data *ch,
                                         char *arg, int type,
                                         struct char_data *tar_ch,
                                         struct obj_data *tar_obj);
void            cast_detect_magic(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_detect_poison(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_dispel_evil(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_dispel_good(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_faerie_fire(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_enchant_weapon(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_enchant_armor(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_heal(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_invisibility(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_locate_object(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_poison(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_protection_from_evil(int level, struct char_data *ch,
                                          char *arg, int type,
                                          struct char_data *tar_ch,
                                          struct obj_data *tar_obj);
void            cast_remove_curse(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_remove_poison(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_remove_paralysis(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *tar_ch,
                                      struct obj_data *tar_obj);
void            cast_sanctuary(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_fireshield(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_sleep(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_strength(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_ventriloquate(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_word_of_recall(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_summon(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_charm_person(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_charm_monster(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_sense_life(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_identify(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_fire_breath(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_frost_breath(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_acid_breath(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_gas_breath(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_lightning_breath(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *tar_ch,
                                      struct obj_data *tar_obj);
void            cast_knock(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_know_alignment(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_weakness(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_dispel_magic(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_animate_dead(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_succor(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_paralyze(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_fear(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_turn(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_faerie_fog(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *victim,
                                struct obj_data *tar_obj);
void            cast_poly_self(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_minor_creation(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_conjure_elemental(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            cast_cacaodemon(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_mon_sum1(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum2(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum3(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum4(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum5(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum6(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_mon_sum7(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_tree_travel(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_speak_with_plants(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            cast_transport_via_plant(int level, struct char_data *ch,
                                         char *arg, int type,
                                         struct char_data *tar_ch,
                                         struct obj_data *tar_obj);
void            cast_plant_gate(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_haste(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_slow(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_reincarnate(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_changestaff(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_pword_kill(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_pword_blind(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_chain_lightn(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_scare(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_familiar(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_aid(int level, struct char_data *ch, char *arg,
                         int type, struct char_data *tar_ch,
                         struct obj_data *tar_obj);
void            cast_holyword(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_unholyword(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_golem(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_command(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_change_form(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_shillelagh(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_goodberry(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_flame_blade(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_animal_growth(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_insect_growth(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_creeping_death(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_commune(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_feeblemind(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_animal_summon_1(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_animal_summon_2(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_animal_summon_3(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_fire_servant(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_earth_servant(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_water_servant(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_wind_servant(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_veggie_growth(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_charm_veggie(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_tree(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_animate_rock(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_travelling(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_animal_friendship(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            cast_invis_to_animals(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *tar_ch,
                                      struct obj_data *tar_obj);
void            cast_slow_poison(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_entangle(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_snare(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_gust_of_wind(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_barkskin(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_warp_weapon(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_heat_stuff(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_sunray(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_find_traps(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_firestorm(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_dust_devil(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_know_monster(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_silence(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_dragon_ride(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_mount(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_portal(int level, struct char_data *ch, char *arg,
                            int type, struct char_data *tar_ch,
                            struct obj_data *tar_obj);
void            cast_teleport_wo_error(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);

/*
 * New Paladin Spells (GH) 
 */
void            cast_holy_armor(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_holy_strength(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_enlightenment(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);

void            cast_wrath_god(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_pacifism(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);
void            cast_aura_power(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_circle_protection(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            do_charge(struct char_data *ch, char *argument, int cmd);
void            do_steed(struct char_data *ch, char *argument, int cmd);
void            do_stop(struct char_data *ch, char *argument, int cmd);
void            do_throw_voice(struct char_data *ch, char *argument, int cmd);
void            do_detect_sound(struct char_data *ch, char *argument, int cmd);
void            do_know(struct char_data *ch, char *argument, int cmd);
void            do_legend_lore(struct char_data *ch, char *argument, int cmd);
void            do_ventriloquate(struct char_data *ch, char *argument, int cmd);

/*
 * From utility.c 
 */
void            Log(char *s, ...);
int             advatoi(const char *s);
int             fighting_in_room(int room_n);
void            do_open_exit(struct char_data *ch, char *argument, int cmd);
int             IsGoodSide(struct char_data *ch);
int             IsBadSide(struct char_data *ch);
int             CanFightEachOther(struct char_data *ch,
                                  struct char_data *ch2);
int             IS_LINKDEAD(struct char_data *ch);
int             MaxCanMemorize(struct char_data *ch, int spell);
int             MEMORIZED(struct char_data *ch, int spl);
int             in_group(struct char_data *ch1, struct char_data *ch2);

int             IS_MURDER(struct char_data *ch);
int             IS_STEALER(struct char_data *ch);

int             MAX_SPECIALS(struct char_data *ch);
void            do_bellow(struct char_data *ch, char *arg, int cmd);
int             MaxDexForRace(struct char_data *ch);
int             MaxIntForRace(struct char_data *ch);
int             MaxWisForRace(struct char_data *ch);
int             MaxConForRace(struct char_data *ch);
int             MaxChrForRace(struct char_data *ch);
int             MaxStrForRace(struct char_data *ch);

int             MaxLimited(int lev);
int             IsSpecialized(int sk_num);
int             CanSeeTrap(int num, struct char_data *ch);
int             anti_barbarian_stuff(struct obj_data *obj_object);
int             CheckGetBarbarianOK(struct char_data *ch,
                                    struct obj_data *obj_object);
int             CheckGiveBarbarianOK(struct char_data *ch,
                                     struct char_data *vict,
                                     struct obj_data *obj);
int             EgoBladeSave(struct char_data *ch);
int             GetItemClassRestrictions(struct obj_data *obj);
int             OnlyClassItemValid(struct char_data *ch,
                                   struct obj_data *obj);
int             CAN_SEE(struct char_data *s, struct char_data *o);
int             exit_ok(struct room_direction_data *exit,
                        struct room_data **rpp);
int             WeaponImmune(struct char_data *ch);
unsigned        IsImmune(struct char_data *ch, int bit);
unsigned        IsResist(struct char_data *ch, int bit);
unsigned        IsSusc(struct char_data *ch, int bit);
int             scan_number(char *text, int *rval);
void            log_sev(char *str, int sev);
void            slog(char *str);
void            sprintbit(unsigned long vektor, char *names[],
                          char *result);
void            sprinttype(int type, char *names[], char *result);
struct time_info_data real_time_passed(time_t t2, time_t t1);
struct time_info_data mud_time_passed(time_t t2, time_t t1);
void            mud_time_passed2(time_t t2, time_t t1,
                                 struct time_info_data *t);
void            age2(struct char_data *ch, struct time_info_data *g);
struct time_info_data age(struct char_data *ch);
char            getall(char *name, char *newname);
int             getabunch(char *name, char *newname);
int             DetermineExp(struct char_data *mob, int exp_flags);
void            down_river(int pulse);
void            RoomSave(struct char_data *ch, long start, long end);
void            RoomLoad(struct char_data *ch, int start, int end);
int             IsHumanoid(struct char_data *ch);
int             IsRideable(struct char_data *ch);
int             IsAnimal(struct char_data *ch);
int             IsVeggie(struct char_data *ch);
int             IsUndead(struct char_data *ch);
int             IsLycanthrope(struct char_data *ch);
int             IsDiabolic(struct char_data *ch);
int             IsReptile(struct char_data *ch);
int             HasHands(struct char_data *ch);
int             HasWings(struct char_data *ch);
int             IsPerson(struct char_data *ch);
int             IsGiantish(struct char_data *ch);
int             IsSmall(struct char_data *ch);
int             IsGiant(struct char_data *ch);
int             IsExtraPlanar(struct char_data *ch);
int             IsOther(struct char_data *ch);
int             IsDragon(struct char_data *ch);
int             IsGodly(struct char_data *ch);
int             IsDarkrace(struct char_data *ch);
void            SetHunting(struct char_data *ch, struct char_data *tch);
void            CallForGuard(struct char_data *ch, struct char_data *vict,
                             int lev, int area);
void            StandUp(struct char_data *ch);
void            MakeNiftyAttack(struct char_data *ch);
void            FighterMove(struct char_data *ch);
void            MonkMove(struct char_data *ch);
void            DevelopHatred(struct char_data *ch, struct char_data *v);
int             HasObject(struct char_data *ch, int ob_num);
int             room_of_object(struct obj_data *obj);
struct char_data *char_holding(struct obj_data *obj);
int             RecCompObjNum(struct obj_data *o, int obj_num);
void            RestoreChar(struct char_data *ch);
void            RemAllAffects(struct char_data *ch);
int             CheckForBlockedMove(struct char_data *ch, int cmd,
                                    char *arg, int room, int dir,
                                    int class);
void            TeleportPulseStuff(int pulse);
void            AdvicePulseStuff(int pulse);
void            DarknessPulseStuff(int pulse);
void            ArenaPulseStuff(int pulse);
void            traveling_qp(int pulse);
void            PlaysongPulseStuff(int pulse);
void            AuctionPulseStuff(int pulse);
void            TrollRegenPulseStuff(int pulse);
void            RiverPulseStuff(int pulse);
int             apply_soundproof(struct char_data *ch);
int             check_soundproof(struct char_data *ch);
int             MobCountInRoom(struct char_data *list);
void           *Mymalloc(long size);
int             CountLims(struct obj_data *obj);
struct obj_data *find_a_rare(struct obj_data *obj);
void            CheckLegendStatus(struct char_data *ch);
char           *lower(char *s);
int             getFreeAffSlot(struct obj_data *obj);
void            SetRacialStuff(struct char_data *mob);
int             check_nomagic(struct char_data *ch, char *msg_ch,
                              char *msg_rm);
int             NumCharmedFollowersInRoom(struct char_data *ch);
struct char_data *FindMobDiffZoneSameRace(struct char_data *ch);
int             NoSummon(struct char_data *ch);
int             GetApprox(int num, int perc);
int             MountEgoCheck(struct char_data *ch,
                              struct char_data *horse);
int             RideCheck(struct char_data *ch, int mod);
void            FallOffMount(struct char_data *ch, struct char_data *h);
int             LearnFromMistake(struct char_data *ch, int sknum,
                                 int silent, int max);
int             IsOnPmp(int room_nr);
int             GetSumRaceMaxLevInRoom(struct char_data *ch);
int             too_many_followers(struct char_data *ch);
int             follow_time(struct char_data *ch);
int             ItemAlignClash(struct char_data *ch, struct obj_data *obj);
int             ItemEgoClash(struct char_data *ch, struct obj_data *obj,
                             int bon);
void            IncrementZoneNr(int nr);
int             CheckEgo(struct char_data *ch, struct obj_data *obj);
int             CheckGetEgo(struct char_data *ch, struct obj_data *obj);
int             CheckEgoGive(struct char_data *ch, struct char_data *vict,
                             struct obj_data *obj);
int             HowManySpecials(struct char_data *ch);
void            tweak(struct obj_data *obj);
int             MobCastCheck(struct char_data *ch, int psi);

struct obj_data *find_tqp(int tqp_nr);
int             count_tqp(void);
/*
 * From weather.c 
 */

void            weather_and_time(int mode);
void            another_hour(int mode);
void            ChangeSeason(int month);
void            weather_change(void);
void            ChangeWeather(int change);
void            GetMonth(int month);
void            switch_light(byte why);

/*
 * From create.c 
 */

void            ChangeRoomFlags(struct room_data *rp, struct char_data *ch,
                                char *arg, int type);
void            ChangeRoomDesc(struct room_data *rp, struct char_data *ch,
                               char *arg, int type);
void            UpdateRoomMenu(struct char_data *ch);
void            do_redit(struct char_data *ch, char *arg, int cmd);
void            RoomEdit(struct char_data *ch, char *arg);
void            ChangeRoomName(struct room_data *rp, struct char_data *ch,
                               char *arg, int type);
void            ChangeRoomType(struct room_data *rp, struct char_data *ch,
                               char *arg, int type);
void            ChangeExitDir(struct room_data *rp, struct char_data *ch,
                              char *arg, int type);
void            ChangeExitNumber(struct room_data *rp,
                                 struct char_data *ch, char *arg,
                                 int type);
void            ChangeKeyNumber(struct room_data *rp, struct char_data *ch,
                                char *arg, int type);
void            AddExitToRoom(struct room_data *rp, struct char_data *ch,
                              char *arg, int type);
void            DeleteExit(struct room_data *rp, struct char_data *ch,
                           char *arg, int type);

/*
 * From parser.c 
 */

NODE           *SearchForNodeByName(NODE * head, char *name, int length);
int             FindCommandNumber(char *cmd);
char           *FindCommandName(int num);

/*
 * mail.c 
 */

struct char_data *find_mailman(struct char_data *ch);
void            postmaster_send_mail(struct char_data *ch, int cmd,
                                     char *arg);
void            postmaster_check_mail(struct char_data *ch, int cmd,
                                      char *arg);
void            postmaster_receive_mail(struct char_data *ch, int cmd,
                                        char *arg);
int             scan_mail_file(void);

/*
 * create.*.c 
 */
void            do_ooedit(struct char_data *ch, char *argument, int cmd);
void            do_medit(struct char_data *ch, char *argument, int cmd);
void            do_oedit(struct char_data *ch, char *argument, int cmd);
void            do_hedit(struct char_data *ch, char *argument, int cmd);
void            MobEdit(struct char_data *ch, char *arg);
void            ObjEdit(struct char_data *ch, char *arg);
void            HelpEdit(struct char_data *ch, char *arg);

void            memory_check(char *p);
void            dlog(char *s);

void            do_ooc(struct char_data *ch, char *argument, int cmd);
void            do_wiznoooc(struct char_data *ch, char *argument, int cmd);
void            do_ooc(struct char_data *ch, char *argument, int cmd);
void            do_plr_noooc(struct char_data *ch, char *argument,
                             int cmd);
void            do_msave(struct char_data *ch, char *argument, int cmd);
void            do_wizreport(struct char_data *ch, char *argument,
                             int cmd);
void            do_donate(struct char_data *ch, char *argument, int cmd);
void            do_reply(struct char_data *ch, char *argument, int cmd);
void            raw_kill_arena(struct char_data *ch);

/*
 * from act.move.c (I don't know why the others aren't here)
 */
void            do_land(struct char_data *ch, char *argument, int cmd);
void            do_launch(struct char_data *ch, char *argument, int cmd);
int             HasFlyItem(struct char_data *ch);


void            stop_follower_quiet(struct char_data *ch);
/*
 * act.other.c
 */
void            do_behead(struct char_data *ch, char *argument, int cmd);
void            do_flag_status(struct char_data *ch, char *argument, int cmd);

/*
 * Spec_procs.c
 */
int             QPSalesman(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             NecromancerGuildMaster(struct char_data *ch, int cmd,
                                       char *arg, struct char_data *mob,
                                       int type);
int             FightingGuildMaster(struct char_data *ch, int cmd,
                                    char *arg, struct char_data *mob,
                                    int type);
/*
 * Spec_procs3
 */
int             DispellerIncMob(struct char_data *ch, int cmd, char *arg,
                                struct char_data *mob, int type);
int             Magic_Pool(struct char_data *ch, int cmd, char *arg,
                           struct room_data *rp, int type);
int             Read_Room(struct char_data *ch, int cmd, char *arg,
                          struct room_data *rp, int type);
int             QuestorGOD(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             QuestMobProc(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);

/*
 * Spec_procs2
 */

int             Etheral_post(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             board_ship(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             CorsairPush(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
/*
 * act.comm.c - used for mobs to tell people stuff 
 */
void            do_mobTell(struct char_data *ch, char *mob,
                           char *sentence);

/*
 * Utilities 
 */
void qlog(struct char_data *ch, char *text);
void            do_mrebuild(struct char_data *ch, char *argument,
                            int cmd);

/*
 * Leaves of Silver 
 */
int             Vaelhar(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob);

bool            dimd_can_see(int slev, struct char_data *o);
void            dimd_broadcast(char *buf);
char           *one_lc_dimd_argument(char *argument, char *first_arg);
char           *one_dimd_argument(char *argument, char *first_arg);
bool            dimd_credit(struct char_data *ch, int credits);
int             matchmud(char *mudname);
int             getlocalmud(void);
int             getmud(struct char_data *ch, char *mudname,
                       bool checkforup);
void            do_dgossip(struct char_data *ch, char *argument, int cmd);
void            do_dlist(struct char_data *ch, char *argument, int cmd);
void            do_dmanage(struct char_data *ch, char *argument, int cmd);
void            do_dlink(struct char_data *ch, char *argument, int cmd);
void            do_dunlink(struct char_data *ch, char *argument, int cmd);
void            do_dmuse(struct char_data *ch, char *argument, int cmd);
void            do_dtell(struct char_data *ch, char *argument, int cmd);
void            do_dwho(struct char_data *ch, char *argument, int cmd);
void            do_drestrict(struct char_data *ch, char *argument, int cmd);
void            do_dthink(struct char_data *ch, char *argument, int cmd);

bool            call_a_mud(int mud);
bool            answer_a_mud(void);
void            hangup_on_a_mud(int mud);
void            close_dimd(void);
void            dimd_loop(void);
int             process_dimd_output(int mud);
int             process_dimd_input(int mud);

void            do_zconv(struct char_data *ch, char *argument, int cmdnum);

long            NewExpCap(struct char_data *ch, long total);

int             ValidRoom(struct char_data *ch);

/*
 * Spec_procs4.c 
 */
int             TrainingGuild(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             qp_potion(struct char_data *ch, int cmd, char *arg);
int             close_doors(struct char_data *ch, struct room_data *rp,
                            int cmd);
int             generic_guildmaster(struct char_data *ch, int cmd,
                                    char *arg, struct char_data *mob,
                                    int type);
/* Temporary guy to fix skills */
int skillfixer(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type);

int mazekeeper(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);
int mazekeeper_riddle_master(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);

int mazekeeper_riddle_one(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);
int mazekeeper_riddle_two(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);
int mazekeeper_riddle_three(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);
int mazekeeper_riddle_four(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob);
int level_limiter(struct char_data *ch, int cmd, char *argument,
                  struct obj_data *obj, int type);
int mazekeeper_portal(struct char_data *ch, int cmd, char *argument,
                  struct obj_data *obj, int type);
/*
 * King's Grove 
 */
int             climb_room(struct char_data *ch, int cmd, char *arg,
                           struct room_data *rp, int type);
int             generate_legend_statue(void);
int             legendfountain(struct char_data *ch, int cmd, char *arg,
                               struct room_data *rp, int type);
int             pick_berries(struct char_data *ch, int cmd, char *arg,
                             struct room_data *rp, int type);
int             pick_acorns(struct char_data *ch, int cmd, char *arg,
                            struct room_data *rp, int type);
int             gnome_home(struct char_data *ch, int cmd, char *arg,
                           struct room_data *rp, int type);
int             gnome_collector(struct char_data *ch, int cmd, char *arg,
                                struct room_data *rp, int type);
void            do_sharpen(struct char_data *ch, char *argument, int cmd);

/*
 * Sentinel's Zone 
 */
int             cog_room(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);

/*
 * Talesian's zone 
 */
int             sageactions(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             traproom(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);
int             guardianroom(struct char_data *ch, int cmd, char *arg,
                             struct room_data *rp, int type);
int             guardianextraction(struct char_data *ch, int cmd,
                                   char *arg, struct char_data *mob,
                                   int type);
int             trapjawsroom(struct char_data *ch, int cmd, char *arg,
                             struct room_data *rp, int type);
int             confusionmob(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             ventroom(struct char_data *ch, int cmd, char *arg,
                         struct room_data *rp, int type);
int             ghastsmell(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             ghoultouch(struct char_data *ch, int cmd, char *arg,
                           struct char_data *mob, int type);
int             shadowtouch(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             moldexplosion(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             mistgolemtrap(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             mirrorofopposition(struct char_data *ch, int cmd,
                                   char *arg, struct obj_data *obj,
                                   int type);

/*
 * Citystate of Tarantis 
 */
int             nightwalker(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
int             timed_door(struct char_data *ch, struct room_data *rp,
                           int cmd);
int             master_smith(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);
int             portal_regulator(struct char_data *ch,
                                 struct room_data *rp, int cmd);

/*
 * Heximal's zone 
 */
int             riddle_exit(struct char_data *ch, int cmd, char *arg,
                            struct room_data *rp, int type);
int             Deshima(struct char_data *ch, int cmd, char *arg,
                        struct char_data *mob, int type);

int             WeaponsMaster(struct char_data *ch, int cmd, char *arg,
                              struct char_data *mob, int type);
int             knockproc(struct char_data *ch, int cmd, char *arg,
                          struct room_data *rp, int type);

int             shopkeeper(struct char_data *ch, int cmd, char *arg,
                           struct char_data *shopkeeper, int type);
int             troll_regen(struct char_data *ch);

void            do_top10(struct char_data *ch, char *arg, int cmd);

void            do_expel(struct char_data *ch, char *argument, int cmd);
void            do_induct(struct char_data *ch, char *argument, int cmd);
void            do_chat(struct char_data *ch, char *argument, int cmd);
void            do_qchat(struct char_data *ch, char *argument, int cmd);

void            mind_wall_of_thought(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            mind_use_wall_of_thought(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);

void            spell_cold_light(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_disease(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_invis_to_undead(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_life_tap(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_suit_of_bone(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_spectral_shield(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_clinging_darkness(int level, struct char_data *ch,
                                        struct char_data *victim,
                                        struct obj_data *obj);
void            spell_dominate_undead(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_unsummon(int level, struct char_data *ch,
                               struct char_data *victim,
                               struct obj_data *obj);
void            spell_siphon_strength(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_gather_shadows(int level, struct char_data *ch,
                                     struct char_data *victim,
                                     struct obj_data *obj);
void            spell_mend_bones(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_trace_corpse(int level, struct char_data *ch,
                                   struct char_data *victim, char *arg);
void            spell_endure_cold(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_life_draw(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_numb_dead(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_binding(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_decay(int level, struct char_data *ch,
                            struct char_data *victim,
                            struct obj_data *obj);
void            spell_shadow_step(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_cavorting_bones(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_mist_of_death(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_nullify(int level, struct char_data *ch,
                              struct char_data *victim,
                              struct obj_data *obj);
void            spell_dark_empathy(int level, struct char_data *ch,
                                   struct char_data *victim,
                                   struct obj_data *obj);
void            spell_eye_of_the_dead(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_soul_steal(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_life_leech(int level, struct char_data *ch,
                                 struct char_data *victim,
                                 struct obj_data *obj);
void            spell_dark_pact(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void            spell_darktravel(int level, struct char_data *ch,
                                 struct char_data *tmp_ch,
                                 struct obj_data *obj);
void            spell_vampiric_embrace(int level, struct char_data *ch,
                                       struct char_data *victim,
                                       struct obj_data *obj);
void            spell_bind_affinity(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);
void            spell_scourge_warlock(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_finger_of_death(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void            spell_flesh_golem(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);
void            spell_chillshield(int level, struct char_data *ch,
                                  struct char_data *victim,
                                  struct obj_data *obj);

void            spell_blade_barrier(int level, struct char_data *ch,
                                    struct char_data *victim,
                                    struct obj_data *obj);

void            do_eval(struct char_data *ch, char *arg, int cmd);
void            do_chtextfile(struct char_data *ch, char *argument,
                              int cmd);
void            TfdEdit(struct char_data *ch, char *arg);

int             remort_guild(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob, int type);

void            do_OOCaction(struct char_data *ch, char *argument,
                             int cmd);

int             ships_helm(struct char_data *ch, int cmd, char *argument,
                           struct obj_data *obj, int type);
int             embark_ship(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob, int type);
char           *formatNum(int foo);
int MoveOne(struct char_data *ch, int dir);
int clearpath(struct char_data *ch, long room, int direc);
long            CalcPowerLevel(struct char_data *ch);
char           *PowerLevelDesc(long a);
int find_door(struct char_data *ch, char *type, char *dir);
int TotalMemorized(struct char_data *ch);
int TotalMaxCanMem(struct char_data *ch);
void list_init(struct descriptor_data *d);
void list_append(struct descriptor_data *d, char *fmt, ...);
void list_end(struct descriptor_data *d);
int fwrite_string(FILE * fl, char *buf);
char           *strip_cr(char *newbuf, const char *orig, size_t maxlen);
void             construct_prompt(char *buf, struct char_data *ch);
void ReadTextZone(FILE * fl);
int CheckKillFile(long virtual);
void CleanZone(int zone);
int LoadZoneFile(FILE * fl, int zon);
void ClassSpecificStuff(struct char_data *ch);
void ObjFromCorpse(struct obj_data *c);
char           *ArmorSize(int a);
int             eval(struct obj_data *object);
void FailPoison(struct char_data *victim, struct char_data *ch);
int IS_FOLLOWING(struct char_data *tch, struct char_data *person);
void mind_sense_object(int level, struct char_data *ch,
                       struct char_data *victim, char *arg);
void spell_disintegrate(int level, struct char_data *ch,
                        struct char_data *victim, struct obj_data *obj);
void raw_lock_door(struct char_data *ch,
                   struct room_direction_data *exitp, int door);
void raw_unlock_door(struct char_data *ch,
                     struct room_direction_data *exitp, int door);
void mind_burn(int level, struct char_data *ch,
               struct char_data *victim, struct obj_data *obj);
void mind_teleport(int level, struct char_data *ch,
                   struct char_data *victim, struct obj_data *obj);
void mind_probability_travel(int level, struct char_data *ch,
                             struct char_data *victim, struct obj_data *obj);
void mind_danger_sense(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void mind_clairvoyance(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void mind_disintegrate(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void mind_telekinesis(int level, struct char_data *ch, 
                      struct char_data *victim, int dir_num);
void mind_levitation(int level, struct char_data *ch,
                     struct char_data *victim, struct obj_data *obj);
void mind_cell_adjustment(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void mind_chameleon(int level, struct char_data *ch,
                    struct char_data *victim, struct obj_data *obj);
void mind_psi_strength(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void mind_mind_over_body(int level, struct char_data *ch,
                         struct char_data *victim, struct obj_data *obj);
void mind_mind_wipe(int level, struct char_data *ch,
                    struct char_data *victim, struct obj_data *obj);
void mind_psychic_crush(int level, struct char_data *ch,
                        struct char_data *victim, struct obj_data *obj);
void mind_tower_iron_will(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void mind_mindblank(int level, struct char_data *ch,
                    struct char_data *victim, struct obj_data *obj);
void mind_psychic_impersonation(int level, struct char_data *ch,
                                struct char_data *victim, struct obj_data *obj);
void mind_ultra_blast(int level, struct char_data *ch,
                      struct char_data *victim, struct obj_data *obj);
void mind_mind_tap(int level, struct char_data *ch,
                   struct char_data *victim, struct obj_data *obj);
void mind_kinolock(int level, struct char_data *ch, char *arg, int type,
                   struct char_data *tar_ch, struct obj_data *tar_obj);
void            UpdateHelpMenu(struct char_data *ch);
int open_door(struct char_data *ch, int dir);
int raw_open_door(struct char_data *ch, int dir);
int DisplayMove(struct char_data *ch, int dir, int was_in, int total);
void go_direction(struct char_data *ch, int dir);

int ValidMove(struct char_data *ch, int cmd);
void spell_wizard_eye(int level, struct char_data *ch,
                      struct char_data *victim, struct obj_data *obj);
void spell_prismatic_spray(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void spell_incendiary_cloud(int level, struct char_data *ch,
                            struct char_data *victim, struct obj_data *obj);
void spell_energy_restore(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void spell_protection_from_evil_group(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void spell_protection_from_good(int level, struct char_data *ch,
                                struct char_data *victim,
                                struct obj_data *obj);
void spell_protection_from_good_group(int level, struct char_data *ch,
                                      struct char_data *victim,
                                      struct obj_data *obj);
void spell_globe_darkness(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void spell_globe_minor_inv(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void spell_globe_major_inv(int level, struct char_data *ch,
                           struct char_data *victim, struct obj_data *obj);
void spell_prot_energy_drain(int level, struct char_data *ch,
                             struct char_data *victim,
                             struct obj_data *obj);
void spell_anti_magic_shell(int level, struct char_data *ch,
                            struct char_data *victim, struct obj_data *obj);
void spell_comp_languages(int level, struct char_data *ch,
                          struct char_data *victim, struct obj_data *obj);
void spell_prot_fire(int level, struct char_data *ch,
                     struct char_data *victim, struct obj_data *obj);
void spell_prot_cold(int level, struct char_data *ch,
                     struct char_data *victim, struct obj_data *obj);
void spell_prot_energy(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void spell_prot_elec(int level, struct char_data *ch,
                     struct char_data *victim, struct obj_data *obj);
void spell_giant_growth(int level, struct char_data *ch,
                        struct char_data *victim, struct obj_data *obj);
void spell_mana_shield(int level, struct char_data *ch,
                       struct char_data *victim, struct obj_data *obj);
void spell_group_heal(int level, struct char_data *ch,
                      struct char_data *victim, struct obj_data *obj);
void spell_iron_skins(int level, struct char_data *ch,
                      struct char_data *victim, struct obj_data *obj);
void SetDefaultLang(struct char_data *ch);
int opdir(int dir);
int countPeople(int zonenr);
int count_People_in_room(int room);
void FORGET(struct char_data *ch, int spl);
void SpaceForSkills(struct char_data *ch);

void            do_wclean(struct char_data *ch, char *argument, int cmd);
void            do_setobjmax(struct char_data *ch, char *argument,
                             int cmd);
void            do_setobjspeed(struct char_data *ch, char *argument,
                               int cmd);
void            do_spend(struct char_data *ch, char *argument, int cmd);
void            bugmail(struct char_data *ch, char *argument, int cmd);
void            do_see_points(struct char_data *ch, char *argument,
                              int cmd);
void            do_reward(struct char_data *ch, char *argument, int cmd);
void            do_punish(struct char_data *ch, char *argument, int cmd);
void            do_set_spy(struct char_data *ch, char *argument, int cmd);

void            do_wizset(struct char_data *ch, char *argument, int cmd);
void            do_home(struct char_data *ch, char *argument, int cmd);
void            do_lgos(struct char_data *ch, char *argument, int cmd);
#ifdef TODO
void            show_race_choice(struct descriptor_data *d);
#endif
void            do_glance(struct char_data *ch, char *argument, int cmd);
void            do_startarena(struct char_data *ch, char *argument, int cmd);
void            do_arena(struct char_data *ch, char *argument, int cmd);
void            do_whoarena(struct char_data *ch, char *argument, int cmd);

void            update_char_objects(struct char_data *ch);
void            do_save(struct char_data *ch, char *arg, int cmd);

void            NailThisSucker(struct char_data *ch);
void            do_look(struct char_data *ch, char *arg, int cmd);
void            DamageAllStuff(struct char_data *ch, int dam_type);

void            cast_calm(int level, struct char_data *ch, char *arg,
                          int si, struct char_data *tar_ch,
                          struct obj_data *tar_obj);
void            cast_energy_restore(int level, struct char_data *ch, 
                                    char *arg, int type, 
                                    struct char_data *victim, 
                                    struct obj_data *tar_obj);
void            cast_protection_from_good(int level, struct char_data *ch, char
                                          *arg, int si,
                                          struct char_data *tar_ch,
                                          struct obj_data *tar_obj);
void            cast_protection_from_good_group(int level,
                                                struct char_data *ch,
                                                char *arg, int type,
                                                struct char_data *tar_ch,
                                                struct obj_data *tar_obj);
void            cast_giant_growth(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_globe_darkness(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_globe_minor_inv(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_globe_major_inv(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_prot_energy_drain(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            cast_anti_magic_shell(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *tar_ch,
                                      struct obj_data *tar_obj);
void            cast_protection_from_evil(int level, struct char_data *ch,
                                          char *arg, int si,
                                          struct char_data *tar_ch,
                                          struct obj_data *tar_obj);
void            cast_protection_from_evil_group(int level,
                                                struct char_data *ch,
                                                char *arg, int type,
                                                struct char_data *tar_ch,
                                                struct obj_data *tar_obj);
void            cast_incendiary_cloud(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *victim,
                                      struct obj_data *tar_obj);
void cast_prismatic_spray(int level, struct char_data *ch, char *arg,
                          int type, struct char_data *victim,
                          struct obj_data *tar_obj);


void            cast_comp_languages(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);
void            cast_prot_fire(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_prot_cold(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_prot_energy(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_prot_elec(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);

void            cast_disintegrate(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            cast_wizard_eye(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *victim,
                                struct obj_data *tar_obj);
void            mind_use_burn(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *victim,
                              struct obj_data *tar_obj);
void            mind_use_danger_sense(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *victim,
                                      struct obj_data *tar_obj);
void            mind_use_clairvoyance(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *victim,
                                      struct obj_data *tar_obj);
void            mind_use_disintegrate(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *victim,
                                      struct obj_data *tar_obj);
void            mind_use_telekinesis(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *victim,
                                     struct obj_data *tar_obj);
void            mind_use_levitation(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *victim,
                                    struct obj_data *tar_obj);
void            mind_use_cell_adjustment(int level, struct char_data *ch,
                                         char *arg, int type,
                                         struct char_data *victim,
                                         struct obj_data *tar_obj);
void            mind_use_chameleon(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            mind_use_psi_strength(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *victim,
                                      struct obj_data *tar_obj);
void            mind_use_mind_over_body(int level, struct char_data *ch,
                                        char *arg, int type,
                                        struct char_data *victim,
                                        struct obj_data *tar_obj);
void            mind_use_teleport(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);
void            mind_use_probability_travel(int level,
                                            struct char_data *ch,
                                            char *arg, int type,
                                            struct char_data *victim,
                                            struct obj_data *tar_obj);
void            mind_use_mind_wipe(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            mind_use_psychic_crush(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *victim,
                                       struct obj_data *tar_obj);
void            mind_use_tower_iron_will(int level, struct char_data *ch,
                                         char *arg, int type,
                                         struct char_data *victim,
                                         struct obj_data *tar_obj);
void            mind_use_mindblank(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *victim,
                                   struct obj_data *tar_obj);
void            mind_use_psychic_impersonation(int level,
                                               struct char_data *ch,
                                               char *arg, int type,
                                               struct char_data *victim,
                                               struct obj_data *tar_obj);
void            mind_use_ultra_blast(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *victim,
                                     struct obj_data *tar_obj);
void            mind_use_mind_tap(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *victim,
                                  struct obj_data *tar_obj);

void            cast_cold_light(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);

void            cast_disease(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);

void            cast_invis_to_undead(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);

void            cast_suit_of_bone(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);

void            cast_spectral_shield(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);

void            cast_clinging_darkness(int level, struct char_data *ch,
                                       char *arg, int type,
                                       struct char_data *tar_ch,
                                       struct obj_data *tar_obj);
void            cast_life_tap(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);

void            cast_dominate_undead(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);

void            cast_unsummon(int level, struct char_data *ch, char *arg,
                              int type, struct char_data *tar_ch,
                              struct obj_data *tar_obj);

void            cast_siphon_strength(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);

void            cast_gather_shadows(int level, struct char_data *ch,
                                    char *arg, int type,
                                    struct char_data *tar_ch,
                                    struct obj_data *tar_obj);

void            cast_mend_bones(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_trace_corpse(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);

void            cast_endure_cold(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);

void            cast_life_draw(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);

void            cast_numb_dead(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_binding(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_decay(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch,
                           struct obj_data *tar_obj);
void            cast_shadow_step(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_cavorting_bones(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_mist_of_death(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_nullify(int level, struct char_data *ch, char *arg,
                             int type, struct char_data *tar_ch,
                             struct obj_data *tar_obj);
void            cast_dark_empathy(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);
void            cast_eye_of_the_dead(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_soul_steal(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_life_leech(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_dark_pact(int level, struct char_data *ch, char *arg,
                               int type, struct char_data *tar_ch,
                               struct obj_data *tar_obj);
void            cast_darktravel(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            cast_vampiric_embrace(int level, struct char_data *ch,
                                      char *arg, int type,
                                      struct char_data *tar_ch,
                                      struct obj_data *tar_obj);
void            cast_bind_affinity(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);
void            cast_scourge_warlock(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_finger_of_death(int level, struct char_data *ch,
                                     char *arg, int type,
                                     struct char_data *tar_ch,
                                     struct obj_data *tar_obj);
void            cast_flesh_golem(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_chillshield(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);

void            cast_blade_barrier(int level, struct char_data *ch,
                                   char *arg, int type,
                                   struct char_data *tar_ch,
                                   struct obj_data *tar_obj);

void            cast_mana_shield(int level, struct char_data *ch,
                                 char *arg, int type,
                                 struct char_data *tar_ch,
                                 struct obj_data *tar_obj);
void            cast_iron_skins(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);

void            cast_group_heal(int level, struct char_data *ch,
                                char *arg, int type,
                                struct char_data *tar_ch,
                                struct obj_data *tar_obj);
void            mind_use_kinolock(int level, struct char_data *ch,
                                  char *arg, int type,
                                  struct char_data *tar_ch,
                                  struct obj_data *tar_obj);

void mind_use_sense_object(int level, struct char_data *ch, char *arg,
                           int type, struct char_data *tar_ch, 
                           struct obj_data *tar_obj);


char *file_to_string(char *name);

void show_class_skills(struct char_data *ch, char *buffer, int classnum,
                       int skills);
void            do_mobTell2(struct char_data *ch,
                            struct char_data *mob, char *sentence);

FILE           *MakeZoneFile(struct char_data *c, int zone);
int             SaveZoneFile(FILE * fp, int start_room, int end_room);
FILE           *OpenZoneFile(struct char_data *c, int zone);

void ShowHates(struct char_data *ch, char *buffer);
void ShowFears(struct char_data *ch, char *buffer);
void sprintclasses(unsigned long vektor, char *result);

int make_exit_ok(struct char_data *ch, struct room_data **rpp, int dir);
int find_direction(char *arg);
char           *fread_string(FILE * fl);
int             IS_UNDERGROUND(struct char_data *ch);
int isname2(const char *str, const char *namelist);
char           *DescAge(int age, int race);
int bitvector_num(int temp);
int color_strlen(struct char_data *ch, char *arg);

void do_OOCemote(struct char_data *ch, char *argument, int cmd);

int dragon(struct char_data *ch, int cmd, char *arg, 
           struct char_data *mob, int type);

/**
 * @todo Crap to be removed
 */
void            oldSendOutput(struct char_data *ch, char *fmt, ...);



/*
 * TOTALLY break log() if someone tries to use it!  Use Log()
 */
#define log(x) use_Log error


#endif

/*
 * vim:ts=4:sw=4:ai:et:si:sts=4
 */
