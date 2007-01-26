#include "config.h"
#include "environment.h"
#include "platform.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "protos.h"
#include "externs.h"

/*
 * external vars 
 */

struct riddle_answer {
    char   *answer;
    int     reward;
    char   *rewardText;
};
    
int mazekeeper_riddle_common(struct char_data *ch, char *arg,
                             struct char_data *mob, struct riddle_answer *rid,
                             int ridCount, int exp, int portal);

void            printmap(struct char_data *ch, int x, int y, int sizex,
                         int sizey);
struct obj_data *SailDirection(struct obj_data *obj, int direction);
int             CanSail(struct obj_data *obj, int direction);
int ReadObjs(FILE * fl, struct obj_file_u *st);

/*
 * two global integers for Sentinel's cog room procedure 
 */
extern int      cog_sequence;
int             chest_pointer = 0;

#define IS_IMMUNE(ch, bit) (IS_SET((ch)->M_immune, bit))
extern struct obj_data *object_list;



int vampiric_embrace(struct char_data *ch, struct char_data *vict)
{
    struct obj_data *obj;
    int             dam;

    if (IsImmune(vict, IMM_DRAIN)) {
        return (FALSE);
    }
    if (ch->equipment[WIELD]) {
        obj = ch->equipment[WIELD];
        act("$c0008The negative aura surrounding your $p lashes out at $N, "
            "draining some of $S life.", FALSE, ch, obj, vict, TO_CHAR);
        act("$c0008The negative aura surrounding $n's $p lashes out at $N, "
            "draining some of $S life.", FALSE, ch, obj, vict, TO_NOTVICT);
        act("$c0008The negative aura surrounding $n's $p lashes out at you, "
            "draining some of your life.", FALSE, ch, obj, vict, TO_VICT);
    } else {
        act("$c0008The negative aura surrounding your hands lashes out at $N, "
            "draining some of $S life.", FALSE, ch, 0, vict, TO_CHAR);
        act("$c0008The negative aura surrounding $n's hands lashes out at $N, "
            "draining some of $S life.", FALSE, ch, 0, vict, TO_NOTVICT);
        act("$c0008The negative aura surrounding $n's hands lashes out at you, "
            "draining some of your life.", FALSE, ch, 0, vict, TO_VICT);
    }
    dam = dice(3, 8);
    if (IsResist(vict, IMM_DRAIN)) {     
        /* 
         * half damage for resist 
         */
        dam >>= 1;
    }
    GET_HIT(ch) += dam;
    GET_HIT(vict) -= dam;
    return (FALSE);
}

int creeping_death(struct char_data *ch, int cmd, char *arg,
                   struct char_data *mob, int type)
{
    struct char_data *t,
                   *next;
    struct room_data *rp;
    struct obj_data *co,
                   *o;

    if (cmd) {
        return (FALSE);
    }
    if (check_peaceful(ch, 0)) {
        act("$n dissipates, you breathe a sigh of relief.", FALSE, ch, 0,
            0, TO_ROOM);
        extract_char(ch);
        return (TRUE);
    }

    if (ch->specials.fighting && IS_SET(ch->specials.act, ACT_SPEC)) {
        /* 
         * kill 
         */
        t = ch->specials.fighting;
        if (t->in_room == ch->in_room) {
#if 0
            Log("woah, you dare fight me? dead you are");
#endif
            act("$N is engulfed by $n!", FALSE, ch, 0, t, TO_NOTVICT);
            act("You are engulfed by $n, and are quickly disassembled.",
                FALSE, ch, 0, t, TO_VICT);
            act("$N is quickly reduced to a bloody pile of bones by $n.",
                FALSE, ch, 0, t, TO_NOTVICT);
            GET_HIT(ch) -= GET_HIT(t);
            die(t, '\0');

            /*
             * find the corpse and destroy it 
             */
            rp = real_roomp(ch->in_room);
            if (!rp) {
                Log("invalid room in creeping death?! oddness!");
                return (FALSE);
            }

            for (co = rp->contents; co; co = co->next_content) {
                if (IS_CORPSE(co)) {
                    /* 
                     * assume 1st corpse is victim's 
                     */
                    while (co->contains) {
                        o = co->contains;
                        obj_from_obj(o);
                        obj_to_room(o, ch->in_room);
                    }

                    /* remove the corpse */
                    extract_obj(co);
                }
            }
        }

        if (GET_HIT(ch) < 0) {
#if 0
            Log("death due to lack of hps");
#endif
            act("$n dissipates, you breathe a sigh of relief.", FALSE, ch,
                0, 0, TO_ROOM);
            extract_char(ch);
        }
        return (TRUE);
    }

    /*
     * the generic is the direction of travel 
     */
    if (number(0, 2) == 0) {
        /* 
         * move 
         */
        if (!ValidMove(ch, (ch->generic) - 1)) {
            if (number(0, 2) != 0) {
                /* 
                 * 66% chance it dies 
                 */
                act("$n dissipates, you breathe a sigh of relief.", FALSE,
                    ch, 0, 0, TO_ROOM);
                extract_char(ch);
            }
        } else {
            do_move(ch, NULL, ch->generic);
        }
        return (FALSE);
    } else {
        /*
         * make everyone with any brains flee 
         */
        for (t = real_roomp(ch->in_room)->people; t; t = next) {
            next = t->next_in_room;
            if (t != ch && !saves_spell(t, SAVING_PETRI)) {
                do_flee(t, NULL, 0);
            }
        }

        /*
         * find someone in the room to flay 
         */
        for (t = real_roomp(ch->in_room)->people; t; t = next) {
            if (!t) {
                Log("found no mobiles in creeping death?! oddness!");
                return (FALSE);
            }

            next = t->next_in_room;
            if (!IS_IMMORTAL(t) && t != ch && !number(0, 1) && 
                IS_SET(ch->specials.act, ACT_SPEC)) {
                act("$N is engulfed by $n!", FALSE, ch, 0, t, TO_NOTVICT);
                act("You are engulfed by $n, and are quickly disassembled.",
                    FALSE, ch, 0, t, TO_VICT);
                act("$N is quickly reduced to a bloody pile of bones by $n.",
                    FALSE, ch, 0, t, TO_NOTVICT);
                GET_HIT(ch) -= GET_HIT(t);
                die(t, '\0');

                /*
                 * find the corpse and destroy it 
                 */
                rp = real_roomp(ch->in_room);
                if (!rp) {
                    Log("invalid room called in creeping death?! oddness!");
                    return (FALSE);
                }

                for (co = rp->contents; co; co = co->next_content) {
                    if (IS_CORPSE(co)) {
                        /* assume 1st corpse is victim's */
                        while (co->contains) {
                            o = co->contains;
                            obj_from_obj(o);
                            obj_to_room(o, ch->in_room);
                        }
                        /* 
                         * remove the corpse 
                         */
                        extract_obj(co);
                    }
                }

                if (GET_HIT(ch) < 0) {
                    act("$n dissipates, you breathe a sigh of relief.",
                        FALSE, ch, 0, 0, TO_ROOM);
                    extract_char(ch);
                    return (TRUE);
                }
                break;
            }
        }
#if 0
        Log("finished finding targets, wait for next func call");
#endif
    }
    return( FALSE );
}

int Deshima(struct char_data *ch, int cmd, char *arg,
            struct char_data *mob, int type)
{
    struct char_data *vict,
                   *next,
                   *i;
    struct room_data *rp;
    char            buf[MAX_STRING_LENGTH + 30];

    /*
     * Deshima was meant to paralyze through immunity 
     */

    if (!ch) {
        return (FALSE);
    }
    if (ch->in_room < 0) {
        return (FALSE);
    }
    rp = real_roomp(ch->in_room);

    if (!rp) {
        return (FALSE);
    }
    if (cmd || !AWAKE(ch)) {
        return (FALSE);
    }
    if ((GET_DEX(ch)) != 21) {
        /* 
         * Deshima wants to have a nice dex value - how about 21? 
         */
        GET_DEX(ch) = 21;
    }

    if (!ch->specials.fighting) {
        return (FALSE);
    } else {
        /* 
         * we're fighting 
         */
        vict = ch->specials.fighting;
        if (!IS_AFFECTED(vict, AFF_PARALYSIS)) {
            if (!IS_IMMORTAL(vict) && (ch != vict)) {
                /* 
                 * let's see if he got lucky (25%) 
                 */
                switch (number(0, 3)) {
                case 1: 
                    /* he got lucky */
                    act("$c0008Deshima's eyes glow a dark c$c000rr$c000Rims"
                        "$c000ro$c0008n as he gazes into $n's eyes.", FALSE, 
                        vict, 0, 0, TO_NOTVICT);
                    act("$c0008$n's muscles seem to contract, but he manages "
                        "to break away from Deshima's gaze in time.$c0007", 
                        FALSE, vict, 0, 0, TO_NOTVICT);

                    sprintf(buf, "$c0008Deshima's eyes glow a dark c$c000rr"
                                 "$c000Rims$c000ro$c0008n as he gazes into "
                                 "your soul.\n\r");
                    send_to_char(buf, vict);

                    sprintf(buf, "$c0008You feel your muscles turn to stone, "
                                 "but manage to break the eye contact in time."
                                 "$c0007\n\r");
                    send_to_char(buf, vict);
                    break;
                default:
                    /* 
                     * let's nail that bugger! 
                     */
                    act("$c0008Deshima's eyes glow a dark c$c000rr$c000Rims"
                        "$c000ro$c0008n as he gazes into $n's eyes.", FALSE,
                        vict, 0, 0, TO_NOTVICT);
                    act("$c0008$n's muscles turn all rigid, making him unable "
                        "to fight.$c0007", FALSE, vict, 0, 0, TO_NOTVICT);

                    sprintf(buf, "$c0008Deshima's eyes glow a dark c$c000rr"
                                 "$c000Rims$c000ro$c0008n as he gazes into "
                                 "your soul.\n\r");
                    send_to_char(buf, vict);

                    sprintf(buf, "$c0008You feel your muscles turn to stone, "
                                 "and lose the ability to fight back."
                                 "$c0007\n\r");
                    send_to_char(buf, vict);
                    SET_BIT(vict->specials.affected_by, AFF_PARALYSIS);
                    break;
                }
            }
        } else {
            /* 
             * he's paralyzed, let's look for a new victim 
             */
            for (i = rp->people; i; i = next) {
                next = i->next_in_room;
                if (!IS_IMMORTAL(i) && ch != i && 
                    !IS_AFFECTED(i, AFF_PARALYSIS)) {
                    /*
                     * We got a fresh target on our hands here, let's
                     * start picking on him 
                     */
                    sprintf(buf, "$c0008Having effectively disabled %s, "
                                 "Deshima turns his attention to %s.$c0007",
                            GET_NAME(vict), GET_NAME(i));
                    act(buf, FALSE, i, 0, 0, TO_ROOM);
                    sprintf(buf, "$c0008Having effectively disabled %s, "
                                 "Deshima turns his attention to you!"
                                 "$c0007\n\r", GET_NAME(vict));
                    send_to_char(buf, i);
                    stop_fighting(ch);
                    set_fighting(ch, i);
                    return (FALSE);
                }
            }
        }
    }
    return (FALSE);
}


int mermaid(struct char_data *ch, int cmd, char *arg,
            struct char_data *mob, int type)
{
    struct char_data *i,
                   *next;
    struct affected_type af;
    char            buf[128];

    if (cmd || !AWAKE(ch)) {
        return (FALSE);
    }
    if (GET_POS(ch) < POSITION_SITTING) {
        return (FALSE);
    }
    if (check_soundproof(ch)) {
        return (FALSE);
    }
    if (check_nomagic(ch, 0, 0)) {
        return (FALSE);
    }
    /*
     * if ch is fighting, don't fire 
     */
    if (ch->specials.fighting) {
        return (FALSE);
    }
    /*
     * if ch already has a follower, don't fire 
     */
    if (ch->followers) {
        return (FALSE);
    }
    /*
     * ch not fighting, let's look for a victim 
     */
    if (ch->in_room <= -1) {
        return( FALSE );
    }

    /*
     * there's victims, let's see if we can harrass one 
     */
    for (i = real_roomp(ch->in_room)->people; i; i = next) {
        next = i->next_in_room;
        if (GET_RACE(i) == RACE_HUMAN && GET_SEX(i) == SEX_MALE && !IS_NPC(i) &&
            !IS_LINKDEAD(i) && !IS_IMMORTAL(i) && 
            !affected_by_spell(i, SPELL_CHARM_PERSON)) {
            if (!IsImmune(i, IMM_CHARM)) {
                if (!saves_spell(i, SAVING_PARA)) {
                    /* didn't make his save, his ass is mine! */
                    act("$n sings a beautiful song, oh my.. Your heart is sold"
                        " to $m.", FALSE, ch, 0, i, TO_VICT);
                    act("$n hums a merry tune while looking $N in the eyes.",
                        TRUE, ch, 0, i, TO_NOTVICT);
                    act("$N grins like the moron he is. $n's charms enchanted "
                        "him.", TRUE, ch, 0, i, TO_NOTVICT);

                    if (i->master) {
                        stop_follower(i);
                    }
                    add_follower(i, ch);
                    af.type = SPELL_CHARM_PERSON;
                    af.duration = 24;
                    af.modifier = 0;
                    af.location = 0;
                    af.bitvector = AFF_CHARM;
                    affect_to_char(i, &af);
                    return (TRUE);
                } else {
                    /* 
                     * made his save, give him some notice 
                     */
                    act("$n sings a beautiful song, oh my.. You're almost "
                        "willing to follow $m.", FALSE, ch, 0, i, TO_VICT);
                    act("$n hums a merry tune while looking $N in the eyes.", 
                        TRUE, ch, 0, i, TO_NOTVICT);
                }
            } else {
                /* 
                 * victim imm:charm, make him pay! 
                 */
                act("$n sings a beautiful song. At the end, you applaud and "
                    "give $m some coins.", FALSE, ch, 0, i, TO_VICT);
                act("$n hums a merry tune while looking $N in the eyes.", TRUE,
                    ch, 0, i, TO_NOTVICT);
                act("When it's finished, $N gives $m a round of applause and "
                    "hands over some coins.", TRUE, ch, 0, i, TO_NOTVICT);
                sprintf(buf, "give 20 coins %s", GET_NAME(ch));
                command_interpreter(i, buf);
            }
        }
    }
    return( TRUE );
}

/*
 * procs for the King's Grove 
 */
#define LEGEND_STATUE 52851
#define LEGEND_PAINTING 52852
#define LEGEND_BIOGRAPHY 52853
#if 0 
struct char_data *ch, char *argument, int cmd)
#endif
int generate_legend_statue(void)
{
    struct obj_data *obj;
    struct char_data *tmp;
    struct char_file_u player;
    struct extra_descr_data *ed;
    char            name[254],
                    shdesc[254],
                    desc[254],
                    exdesc[500];
    int             i = 0,
                    itype = 0,
                    rnum = 0;
    extern int      top_of_p_table;
    extern struct player_index_element *player_table;

    /*
     * Determine number of pfiles. Add one for last player made, though
     * that one isn't very likely to have enough kills. Still, we wanna be 
     * thorough. 
     */
    for (i = 0; i < top_of_p_table + 1; i++) {
        /*
         * load up each of them 
         */
        if (load_char((player_table + i)->name, &player) > -1) {
            /*
             * store to a tmp char that we can deal with 
             */
            CREATE(tmp, struct char_data, 1);
            clear_char(tmp);
            store_to_char(&player, tmp);
            /*
             * are they entitled to an item? 
             */
            if (tmp->specials.m_kills >= 10000) {
                /*
                 * coolness, the are! Determine the item. 
                 */
                if (tmp->specials.m_kills >= 40000) {
                    itype = LEGEND_BIOGRAPHY;
                    rnum = number(52892, 52895);
                    sprintf(name, "biography tome %s", GET_NAME(tmp));
                    sprintf(shdesc, "a biography of %s", GET_NAME(tmp));
                    sprintf(desc, "A large tome lies here, titled "
                                  "'The Biography of %s'.", GET_NAME(tmp));
                    sprintf(exdesc, "This book is a treatise on the life and "
                                    "accomplishments of %s.\n\rIt is an "
                                    "extensive volume, detailing many a feat. "
                                    "Most impressive.", GET_NAME(tmp));
                } else if (tmp->specials.m_kills >= 20000) {
                    itype = LEGEND_PAINTING;
                    rnum = number(52886, 52891);
                    sprintf(name, "painting %s", GET_NAME(tmp));
                    sprintf(shdesc, "a painting of %s", GET_NAME(tmp));
                    sprintf(desc, "On the wall, one can admire a painting of "
                                  "%s, slaying a fearsome beast.", 
                                  GET_NAME(tmp));
                    sprintf(exdesc, "%s is in the process of slaying a fearsome"
                                    " beast.\n\rTruly, %s is one of the "
                                    "greatest of these times.",
                            GET_NAME(tmp), GET_NAME(tmp));
                } else {
                    itype = LEGEND_STATUE;
                    rnum = number(52861, 52884);
                    sprintf(name, "statue %s", GET_NAME(tmp));
                    sprintf(shdesc, "a statue of %s", GET_NAME(tmp));
                    sprintf(desc, "A statue of the legendary %s has been "
                                  "erected here.", GET_NAME(tmp));
                    sprintf(exdesc, "This is a statue of %s, the legendary "
                                    "slayer.", GET_NAME(tmp));
                }
                if (itype == 0) {
                    Log("Oddness in statue generation, no type found");
                    return (TRUE);
                }
                if (rnum == 0) {
                    Log("Oddness in statue generation, no rnum found");
                    return (TRUE);
                }

                /*
                 * load the generic item 
                 */
                if ((obj = read_object(itype, VIRTUAL))) {
                    /*
                     * and string it up a bit 
                     */
                    if (obj->short_description) {
                        free(obj->short_description);
                        obj->short_description = strdup(shdesc);
                    }

                    if (obj->description) {
                        free(obj->description);
                        obj->description = strdup(desc);
                    }

                    if (obj->name) {
                        free(obj->name);
                        obj->name = strdup(name);
                    }

                    if (obj->ex_description) {
                        Log("trying to string invalid item in statue "
                            "generation");
                        return (TRUE);
                    } else {
                        /*
                         * create an extra desc structure for the object 
                         */
                        CREATE(ed, struct extra_descr_data, 1);
                        ed->next = obj->ex_description;
                        obj->ex_description = ed;

                        /*
                         * define the keywords 
                         */
                        CREATE(ed->keyword, char, strlen(name) + 1);
                        strcpy(ed->keyword, name);

                        /*
                         * define the description 
                         */
                        CREATE(ed->description, char, strlen(exdesc) + 1);
                        strcpy(ed->description, exdesc);
                    }

                    /*
                     * and finally place it in a room 
                     */
                    obj_to_room(obj, rnum);
                }
            }
            free(tmp);
        } else {
            Log("screw up bigtime in load_char");
            return (TRUE);
        }
    }

    Log("processed %d pfiles for legend statue check", top_of_p_table + 1);
    return( TRUE );
}





#define COLLECTIBLE_1 3998
#define COLLECTIBLE_2 47961
#define COLLECTIBLE_3 51838
#define COLLECTIBLE_4 51839
#define REWARD_GNOME  52870
int gnome_collector(struct char_data *ch, int cmd, char *arg,
                    struct room_data *rp, int type)
{
    char           *obj_name,
                   *vict_name,
                   *arg1;
    struct char_data *gnome,
                   *tmp_ch = NULL,
                   *j,
                   *receiver = NULL;
    struct obj_data *obj,
                   *i,
                   *reward;
    int             obj_num = 0,
                    x = 0;
    int             HasCollectibles[4] = { 0, 0, 0, 0 };
    int             winner = 0,
                    found = 0;

    if (!ch || !AWAKE(ch)) {
        return (FALSE);
    }

    if (ch->in_room) {
        rp = real_roomp(ch->in_room);
    } else {
        Log("weirdness in gnome_collector, char not in a room");
        return (FALSE);
    }

    if (!rp) {
        Log("weirdness in gnome_collector, char's room does not exist");
        return (FALSE);
    }

    /*
     * let's make sure gnome doesn't get killed or robbed 
     */
    if (!IS_SET(rp->room_flags, PEACEFUL)) {
        SET_BIT(rp->room_flags, PEACEFUL);
    }
    if (!(gnome = get_char_room("gnome female collector", ch->in_room))) {
        Log("gnome_collector proc is attached to a mob without "
            "proper name, in room %ld", ch->in_room);
        return (FALSE);
    }

    if (!gnome) {
        Log("weirdness in gnome_collector, gnome found but not assigned");
        return (FALSE);
    }

    if (!IS_NPC(gnome)) {
        Log("weirdness in gnome_collector, gnome is not a mob");
        return (FALSE);
    }

    /*
     * if there's no pcs present in room, I'm gonna get going, get some
     * work done mate! 
     */

    j = rp->people;
    found = 0;

    while (j && !found) {
        if (IS_PC(j)) {
            /*
             * first PC in room (should be only cuz it's tunnel 1)
             * receives prize, if winner 
             */
            receiver = j;
            found = 1;
        }
        j = j->next_in_room;
    }

    if (!found) {
        /* 
         * no pcs in room! what am I doin here?!  let's get lost 
         */
        act("$n steps into $s little door, and it seals shut behind $s.",
            FALSE, gnome, 0, 0, TO_ROOM);

        while (gnome->carrying) {
            extract_obj(gnome->carrying);
        }

        extract_char(gnome);
        return (TRUE);
    }

    if (!IS_SET(gnome->specials.act, ACT_SENTINEL)) {
        SET_BIT(gnome->specials.act, ACT_SENTINEL);
    }
    if ((i = gnome->carrying)) {
        for (; i; i = i->next_content) {
            if (obj_index[i->item_number].virtual == COLLECTIBLE_1) {
                HasCollectibles[0] = 1;
            } else if (obj_index[i->item_number].virtual == COLLECTIBLE_2) {
                HasCollectibles[1] = 1;
            } else if (obj_index[i->item_number].virtual == COLLECTIBLE_3) {
                HasCollectibles[2] = 1;
            } else if (obj_index[i->item_number].virtual == COLLECTIBLE_4) {
                HasCollectibles[3] = 1;
            }
        }
        winner = 1;

        for (x = 0; x < 4; x++) {
            if (HasCollectibles[x] == 0) {
                winner = 0;
            }
        }

        if (winner) {
            act("$n says, 'Woop, I got everything i need now! Thank you ever "
                "so much.", FALSE, gnome, 0, 0, TO_ROOM);
            if ((reward = read_object(REWARD_GNOME, VIRTUAL))) {
                act("I would express my gratitude by presenting you with this "
                    "magical ring.", FALSE, gnome, 0, 0, TO_ROOM);
                act("I came across it in an ancient traveller's corpse, back "
                    "in the day when", FALSE, gnome, 0, 0, TO_ROOM);
                act("I still got around. I never quite figured out its use, "
                    "but I'm sure it's", FALSE, gnome, 0, 0, TO_ROOM);
                act("more than it seems. I hope you will make good use of "
                    "it.'\n\r", FALSE, gnome, 0, 0, TO_ROOM);
                act("$N gives $p to $n.\n\r", FALSE, receiver, reward,
                    gnome, TO_ROOM);
                act("$N gives $p to you.\n\r", FALSE, receiver, reward,
                    gnome, TO_CHAR);

                obj_to_char(reward, receiver);
            }

            act("$n says, 'Right, gotta get going now, I'm impatient to start "
                "my experiments.", FALSE, gnome, 0, 0, TO_ROOM);
            act("$n steps into $s little door, and it seals shut behind $s.",
                FALSE, gnome, 0, 0, TO_ROOM);

            /*
             * extract carried items if any 
             */
            while (gnome->carrying) {
                extract_obj(gnome->carrying);
            }
            extract_char(gnome);
            return (TRUE);
        }
    }

    /*
     * talk does nothing, she's silent 
     */
    if (cmd == 531) {
        arg = get_argument(arg, &arg1);
        if (arg1 && (tmp_ch = get_char_room_vis(ch, arg1)) && 
            tmp_ch == gnome ) {
            oldSendOutput(ch, "%s looks at you in a meaningful way, but stays "
                          "silent.\n\r", gnome->player.short_descr);
            return (TRUE);
        }
        return (FALSE);
    }

    /*
     * give 
     */
    if (cmd == 72) {
        /*
         * determine the correct obj 
         */
        arg = get_argument(arg, &obj_name);
        if (!obj_name || 
            !(obj = get_obj_in_list_vis(ch, obj_name, ch->carrying))) {
            return (FALSE);
        }

        obj_num = obj_index[obj->item_number].virtual;

        arg = get_argument(arg, &vict_name);

        if ((!vict_name || !(tmp_ch = get_char_room_vis(ch, vict_name))) && 
            tmp_ch != gnome) {
            return (FALSE);
        }

        /*
         * found an object, and the correct person to give it to 
         */
        if (gnome->specials.fighting) {
            send_to_char("Not while she is fighting!\n\r", ch);
            return (TRUE);
        }

        /*
         * correct object? 
         */
        if (obj_num != COLLECTIBLE_1 && obj_num != COLLECTIBLE_2 &&
            obj_num != COLLECTIBLE_3 && obj_num != COLLECTIBLE_4) {
            /*
             * nope 
             */
            oldSendOutput(ch, "%s doesn't seem to be interested in that sort of "
                          "junk.\n\r", gnome->player.short_descr);
            return (TRUE);
        }
        
        act("$n says, 'Woah, good stuff! I've been looking for this thing for "
            "ages.'", FALSE, gnome, 0, 0, TO_ROOM);
        act("You give $p to $N.", FALSE, ch, obj, gnome, TO_CHAR);
        act("$n gives $p to $N.", FALSE, ch, obj, gnome, TO_ROOM);
        obj_from_char(obj);
        obj_to_char(obj, gnome);
        return (TRUE);
    }
    return (FALSE);
}



#define SHARPENING_STONE 52887
void do_sharpen(struct char_data *ch, char *argument, int cmd)
{
    struct obj_data *obj,
                   *cmp,
                   *stone;
    char            buf[254];
    char           *arg;
    int             w_type = 0;

    if (!ch || !cmd || cmd != 602) {
        /* 
         * sharpen 
         */
        return;
    }

    if (ch->specials.fighting) {
        send_to_char("In the middle of a fight?! Hah.\n\r", ch);
        return;
    }

    if (ch->equipment && 
        (!(stone = ch->equipment[HOLD]) || 
         obj_index[stone->item_number].virtual != SHARPENING_STONE)) {
        send_to_char("How can you sharpen stuff if you're not holding a "
                     "sharpening stone?\n\r", ch);
        return;
    }

    /*
     * is holding the stone 
     */
    if (!argument) {
        send_to_char("Sharpen what?\n\r", ch);
        return;
    }

    argument = get_argument(argument, &arg);
    if (!arg) {
        send_to_char("Sharpen what?\n\r", ch);
        return;
    }

    if ((obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
        if ((ITEM_TYPE(obj) == ITEM_WEAPON)) {
            /*
             * can only sharpen edged weapons 
             */
            switch (obj->obj_flags.value[3]) {
            case 0:
                w_type = TYPE_SMITE;
                break;
            case 1:
                w_type = TYPE_STAB;
                break;
            case 2:
                w_type = TYPE_WHIP;
                break;
            case 3:
                w_type = TYPE_SLASH;
                break;
            case 4:
                w_type = TYPE_SMASH;
                break;
            case 5:
                w_type = TYPE_CLEAVE;
                break;
            case 6:
                w_type = TYPE_CRUSH;
                break;
            case 7:
                w_type = TYPE_BLUDGEON;
                break;
            case 8:
                w_type = TYPE_CLAW;
                break;
            case 9:
                w_type = TYPE_BITE;
                break;
            case 10:
                w_type = TYPE_STING;
                break;
            case 11:
                w_type = TYPE_PIERCE;
                break;
            case 12:
                w_type = TYPE_BLAST;
                break;
            case 13:
                w_type = TYPE_IMPALE;
                break;
            case 14:
                w_type = TYPE_RANGE_WEAPON;
                break;
            default:
                w_type = TYPE_HIT;
                break;
            }

            if ((w_type >= TYPE_PIERCE && w_type <= TYPE_STING) || 
                (w_type >= TYPE_CLEAVE && w_type <= TYPE_STAB) || 
                w_type == TYPE_IMPALE) {

                if (obj->obj_flags.value[2] == 0) {
                    Log("%s tried to sharpen a weapon with invalid value: %s, "
                        "vnum %d.", GET_NAME(ch), obj->short_description,
                        obj->item_number);
                    return;
                }

                if (!(cmp = read_object(obj->item_number, REAL))) {
                    Log("Could not load comparison weapon in do_sharpen");
                    return;
                }

                if (cmp->obj_flags.value[2] == 0) {
                    Log("%s tried to sharpen a weapon with invalid value: %s, "
                        "vnum %d.", GET_NAME(ch), obj->short_description,
                        obj->item_number);
                    extract_obj(cmp);
                    return;
                }

                if (cmp->obj_flags.value[2] == obj->obj_flags.value[2]) {
                    send_to_char("That item has no need of your attention.\n\r",
                                 ch);
                    extract_obj(cmp);
                    return;
                } 
                
                obj->obj_flags.value[2] = cmp->obj_flags.value[2];
                if (GET_POS(ch) > POSITION_RESTING) {
                    do_rest(ch, NULL, -1);
                }
                sprintf(buf, "%s diligently starts to sharpen %s.",
                        GET_NAME(ch), obj->short_description);
                act(buf, FALSE, ch, 0, 0, TO_ROOM);

                sprintf(buf, "You diligently sharpen %s.\n\r",
                        obj->short_description);
                send_to_char(buf, ch);

                extract_obj(cmp);
                WAIT_STATE(ch, PULSE_VIOLENCE * 2);
            } else {
                send_to_char("You can only sharpen edged or pointy "
                             "weapons.\n\r", ch);
            }
        } else {
            send_to_char("You can only sharpen weapons.\n\r", ch);
        }
    } else {
        send_to_char("You don't seem to have that.\n\r", ch);
    }
}

/*
 * Sentinel's Zone 
 */
int janaurius(struct char_data *ch, int cmd, char *arg,
              struct char_data *mob)
{

    struct char_data *i,
                   *target;

    if (!ch->specials.fighting && !ch->attackers) {
        ch->generic = 0;
    }
    if (cmd || !AWAKE(ch)) {
        return (FALSE);
    }
    if (ch->generic) {
        return (FALSE);
    }
    if ((target = ch->specials.fighting)) {
        ch->generic = 1;
        for (i = character_list; i; i = i->next) {
            if (mob_index[i->nr].virtual == 51166) {
                AddHated(i, target);
                SetHunting(i, target);
            }
        }
    }
    return( FALSE );
}



/*
 * Ashamael's Citystate of Tarantis 
 */
#define NIGHTWALKER 49600
#define SPAWNROOM 49799
#define DEST_ROOM 49460
#define REAVER_RM 49701
#define WAITROOM 49601

#define TARANTIS_PORTAL 49701
#define REAVER_PORTAL 49460

/*
 * Proc that makes a portal spawn at nighttime 8pm, and close at dawn 5pm. 
 */
int portal_regulator(struct char_data *ch, struct room_data *rp, int cmd)
{
    struct room_data *spawnroom;
    struct char_data *nightwalker;
    struct obj_data *obj;
    extern struct time_info_data time_info;
    int             check = 0;

    if (time_info.hours < 20 && time_info.hours > 5) {
        /* 
         * it should not be there 
         */
        rp = real_roomp(WAITROOM);
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == TARANTIS_PORTAL) {
                    send_to_room("$c0008The dark portal suddenly turns "
                                 "sideways, shrinks to a mere sliver, and "
                                 "disappears completely!\n\r", WAITROOM);
                    extract_obj(obj);
                }
            }
        }

        rp = real_roomp(REAVER_RM);
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == TARANTIS_PORTAL) {
                    send_to_room("$c0008The dark portal suddenly turns "
                                 "sideways, shrinks to a mere sliver, and "
                                 "disappears completely!\n\r", REAVER_RM);
                    extract_obj(obj);
                }
            }
        }

        rp = real_roomp(DEST_ROOM);
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == REAVER_PORTAL) {
                    send_to_room("$c0008The dark portal suddenly turns "
                                 "sideways, shrinks to a mere sliver, and "
                                 "disappears completely!\n\r", DEST_ROOM);
                    extract_obj(obj);
                }
            }
        }                       
        
        /* 
         * all portals are gone, now do the transfer mob thing 
         */
        if (time_info.hours == 9) {
            spawnroom = real_roomp(SPAWNROOM);
            if (!spawnroom) {
                Log("No nightwalker spawnroom found, blame Ash.");
                return (FALSE);
            }

            while (spawnroom->people) {
                nightwalker = spawnroom->people;
                char_from_room(nightwalker);
                char_to_room(nightwalker, WAITROOM);
            }
        }
    } else {
        /* 
         * portals should appear 
         */
        rp = real_roomp(WAITROOM);
        check = 0;
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == TARANTIS_PORTAL) {
                    check = 1;
                }
            }

            if (!check && (obj = read_object(TARANTIS_PORTAL, VIRTUAL))) {
                send_to_room("$c0008A sliver of darkness suddenly appears."
                             " It widens, turns sideways, and becomes a "
                             "portal!\n\r", WAITROOM);
                obj_to_room(obj, WAITROOM);
            }
        }

        rp = real_roomp(REAVER_RM);
        check = 0;
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == TARANTIS_PORTAL) {
                    check = 1;
                }
            }

            if (!check && (obj = read_object(TARANTIS_PORTAL, VIRTUAL))) {
                send_to_room("$c0008A sliver of darkness suddenly appears. It "
                             "widens, turns sideways, and becomes a "
                             "portal!\n\r", REAVER_RM);
                obj_to_room(obj, REAVER_RM);
            }
        }

        rp = real_roomp(DEST_ROOM);
        check = 0;
        if (rp) {
            for (obj = rp->contents; obj; obj = obj->next_content) {
                if (obj_index[obj->item_number].virtual == REAVER_PORTAL) {
                    check = 1;
                }
            }

            if (!check && (obj = read_object(REAVER_PORTAL, VIRTUAL))) {
                send_to_room("$c0008A sliver of darkness suddenly appears. It "
                             "widens, turns sideways, and becomes a "
                             "portal!\n\r", DEST_ROOM);
                obj_to_room(obj, DEST_ROOM);
            }
        }
    }
    return (FALSE);
}


#define ING_1   52870
#define ING_2   52878
#define ING_3   52879
#define ING_4   52880
#define ING_5   44180

#define SMITH_SHIELD 52877

int master_smith(struct char_data *ch, int cmd, char *arg,
                 struct char_data *mob, int type)
{
    struct char_data *yeelorn;
    struct room_data *rp = NULL;
    struct obj_data *obj,
                   *i,
                   *obj1,
                   *obj2,
                   *obj3,
                   *obj4,
                   *obj5;
    int             found = 0;
    int             found1 = 0;
    int             found2 = 0;
    int             found3 = 0;
    int             found4 = 0;
    int             found5 = 0;

    if (!ch || !cmd || IS_NPC(ch)) {
        return (FALSE);
    }

    if (ch->in_room) {
        rp = real_roomp(ch->in_room);
    }
    if (!rp) {
        return (FALSE);
    }
    if (!(yeelorn = FindMobInRoomWithFunction(ch->in_room, master_smith))) {
        Log("proc assigned to a mob that cannot be found?!");
        return (FALSE);
    }

    if (cmd == 72) { 
        /* 
         * give 
         */
        send_to_char("Giving away your stuff for free? Better rethink "
                     "this.\n\r", ch);
        return (TRUE);
    }

    if (cmd == 86) {
        /* 
         * ask 
         */
        send_to_char("Yeelorn says, 'Aye, yer curious if yev got summat that I"
                     " could use, eh? Right, lemme\n\r", ch);
        send_to_char("rummage through yer packs and see if there's aught I kin"
                     " use fer raw material.'\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("Yeelorn searches through your bags, mumbling to himself"
                     " about people toting along\n\r", ch);
        send_to_char("all kinds of useless junk, and should they not put a "
                     "little more thought to what\n\r", ch);
        send_to_char("they're packing and what a tasty looking loaf of "
                     "pipeweed is that.'\n\r", ch);
        send_to_char("\n\r", ch);

        /*
         * check items 
         */
        for (i = object_list; i; i = i->next) {
            if (obj_index[i->item_number].virtual == ING_1) {
                obj = i;

                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    found1 = 1;
                }
            }   

            if (obj_index[i->item_number].virtual == ING_2) {
                obj = i;

                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    found2 = 1;
                }
            }

            if (obj_index[i->item_number].virtual == ING_3) {
                obj = i;
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    found3 = 1;
                }
            }

            if (obj_index[i->item_number].virtual == ING_4) {
                obj = i;
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    found4 = 1;
                }
            }

            if (obj_index[i->item_number].virtual == ING_5) {
                obj = i;
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    found5 = 1;
                }
            }
        }

        if (!found1 && !found2 && !found3 && !found4 && !found5) {
            /* 
             * nothing found 
             */
            send_to_char("When he's finished, Yeelorn says, 'Hmm, no, ye gots "
                         "nothin of interest to me, matey.\n\r", ch);
            send_to_char("Come back when ya got some. Quit wasting me time "
                         "now, there's work to do.'\n\r", ch);
            return (TRUE);
        } 
        
        if (found1 && found2 && found3 && found4 && found5) {
            /* 
             * has everything, tell him the price 
             */
            send_to_char("When he's finished, Yeelorn exclaims, 'Woah mate, I"
                         " see ye'ns got everything I be needin\n\r", ch);
            send_to_char("to fix you up with a pretty shield. The shield of "
                         "thought, to start out with. Then we\n\r", ch);
            send_to_char("gots to reinforce it with a dragon bone and a shield"
                         " spell, enamel it with the silver\n\r", ch);
            send_to_char("from the sheet, and finally, we bring it to life "
                         "with the runes from the ring. Aye, that\n\r", ch);
            send_to_char("will be a nice piece of work. Course, I'll have to "
                         "charge ye a builder's fee. I'd think\n\r", ch);
            send_to_char("a million coins would do nicely. So. Whatcha think? "
                         "You wanna buy this service from me?'\n\r", ch);
            return (TRUE);
        }
        
        if (found1 || found2 || found3 || found4 || found5) {
            /* 
             * has at least one item 
             */
            send_to_char("When he's finished, Yeelorn says, 'Hmm, I see ye've "
                         "got yerself some pretty items.", ch);
            if (found1 == 1) {
                send_to_char("\n\rThis runed ring here, for instance. With the"
                             " aid of them runes, I may be able to embue\n\r",
                             ch);
                send_to_char("a creation of mine with some powerful magicks.  "
                             "Aye, sure looks promisin.", ch);
            }

            if (found2 == 1) {
                send_to_char("\n\rThis here dragon bone looks like the right "
                             "kind to reinforce a certain piece of armor\n\r",
                             ch);
                send_to_char("with.  Oh golly, I bet I could do somethin funky"
                             " with that.", ch);
            }

            if (found3 == 1) {
                send_to_char("\n\rWhat a pretty silver sheet ye've got there, "
                             "mate.  Could put that to some good use\n\r", ch);
                send_to_char("if I were to have to pretty up a little summat "
                             "or another.", ch);
            }

            if (found4 == 1) {
                send_to_char("\n\rHmmm, a scroll of shield.. I could use that "
                             "when hammering some crafty bit of armor.", ch);
            }

            if (found5 == 1) {
                send_to_char("\n\rA shield with a mind of its own, eh? That "
                             "looks int'resting. Bet I could sharpen up\n\r",
                             ch);
                send_to_char("that mind a wee bit. Aye, but I'd need some more"
                             " materials to do so..", ch);
            }

            /*
             * maybe they'll get a hint 
             */
            switch (number(1, 20)) {
            case 1:
                if (!found1) {
                    send_to_char("\n\r\n\rThe other day, I heard there's this"
                                 " tiny gnome who's nicked herself a useful "
                                 "ring.", ch);
                }
                break;
            case 2:
                if (!found2) {
                    send_to_char("\n\r\n\rYa know, dragonbone is ever a useful"
                                 " ingredient for crafty pieces of armor.", ch);
                }
                break;
            case 3:
                if (!found3) {
                    send_to_char("\n\r\n\rI just ran out of silver filigree "
                                 "too. Maybe ye kin find me a sheet or two?",
                                 ch);
                }
                break;
            case 4:
                if (!found4) {
                    send_to_char("\n\r\n\rAye, iffen ye want to imbue the metal"
                                 " with real power, ye'd need a scroll of "
                                 "spell too.", ch);
                }
                break;
            case 5:
                if (!found5) {
                    send_to_char("\n\r\n\rHeard there's a shield with a mind "
                                 "of its own. Wouldn't that be something to "
                                 "see, eh?", ch);
                }
                break;
            default:
                break;
            }
            send_to_char("'\n\r", ch);
        }
        return (TRUE);
    }

    if (cmd == 56) {
        /* 
         * buy 
         */
        obj1 = obj2 = obj3 = obj4 = obj5 = 0;
        for (i = object_list; i; i = i->next) {
            if (obj_index[i->item_number].virtual == ING_1) {
                obj = i;

                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    obj1 = i;
                    found = 1;
                }
            }

            if (obj_index[i->item_number].virtual == ING_2) {
                obj = i;
                
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }    
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    obj2 = i;
                    found = 1;
                }
            }
            
            if (obj_index[i->item_number].virtual == ING_3) {
                obj = i;
            
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    obj3 = i;
                    found = 1;
                }
            }
            
            if (obj_index[i->item_number].virtual == ING_4) {
                obj = i;
                
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    obj4 = i;
                    found = 1;
                }
            }
            
            if (obj_index[i->item_number].virtual == ING_5) {
                obj = i;
            
                while (obj->in_obj) {
                    obj = obj->in_obj;
                }    
                if (obj->carried_by == ch || obj->equipped_by == ch) {
                    obj5 = i;
                    found = 1;
                }
            }
            
            if (found) {
                /* 
                 * transfer items to inventory for easy work later 
                 */
                if (i->equipped_by) {
                    obj = unequip_char(ch, i->eq_pos);
                    obj_to_char(obj, ch);
                } else if (!i->carried_by && i->in_obj) {
                    obj_from_obj(i);
                    obj_to_char(i, ch);
                } else {
                    Log("where is this item!?! bad spot in master_smith");
                    send_to_char("Ugh, something wrong with this proc, "
                                 "sorry.\n\r", ch);
                    return (TRUE);
                }
            }
            found = 0;
        }

        if (obj1 && obj2 && obj3 && obj4 && obj5) {
            /*
             * got all the items.. how about a million coins? 
             */
            if (GET_GOLD(ch) < 1000000) {
                send_to_char("Yeelorn says, 'Aye laddie, ye've got yerself "
                             "the items, but yer still some money short.'\n\r",
                             ch);
                return (TRUE);
            } 

            GET_GOLD(ch) -= 1000000;
            if (obj1->carried_by) {
                obj_from_char(obj1);
                extract_obj(obj1);
            }
            if (obj2->carried_by) {
                obj_from_char(obj2);
                extract_obj(obj2);
            }
            if (obj3->carried_by) {
                obj_from_char(obj3);
                extract_obj(obj3);
            }
            if (obj4->carried_by) {
                obj_from_char(obj4);
                extract_obj(obj4);
            }
            if (obj5->carried_by) {
                obj_from_char(obj5);
                extract_obj(obj5);
            }

            if ((obj = read_object(SMITH_SHIELD, VIRTUAL))) {
                obj_to_char(obj, ch);
                send_to_char("You give your items to Yeelorn, along with an "
                             "incredible heap of coins.\n\r", ch);
                send_to_char("Yeelorn pokes up his forge, and starts heating "
                             "the shield. Once it's red hot,\n\r", ch);
                send_to_char("he bends a few edges, bangs a few times, and the"
                             " dragonbone collar is firmly\n\r", ch);
                send_to_char("attached. He then proceeds with folding the "
                             "silver sheet over the shield, and\n\r", ch);
                send_to_char("heats it some more. More hammering melds the "
                             "silver with the shield and bone,\n\r", ch);
                send_to_char("making it look rather impressive. Then, Yeelorn "
                             "places it in a barrel ot brine,\n\r", ch);
                send_to_char("causing great clouds of noxious fumes to fill "
                             "the air. Next he orates the prayer\n\r", ch);
                send_to_char("from the scroll, which bursts into flame while "
                             "the spell is sucked into the\n\r", ch);
                send_to_char("shield. Once more he heats it up, and when it's "
                             "about to go to pieces from the\n\r", ch);
                send_to_char("heat, he takes it out and presses the runed ring"
                             " in the center of it. The shield\n\r", ch);
                send_to_char("seems to shudder. That must have been your "
                             "imagination. Shields are not alive.\n\r", ch);
                send_to_char("Or are they?\n\r", ch);
                send_to_char("Once the shield has cooled down, Yeelorn takes a"
                             " rag, and polishes it til it\n\r", ch);
                send_to_char("shines as bright as full moon. He giggles as he"
                             " hands it over.\n\r", ch);
                send_to_char("'Bout time yeh got on with yer adventuring' he "
                             "says, as he winks at you.\n\r", ch);
            } else {
                Log("could not load up prize for master_smith proc");
                send_to_char("Ugh, something wrong with this proc, "
                             "sorry..\n\r", ch);
            }

            return (TRUE);
        }
    }
    return (FALSE);
}

int nightwalker(struct char_data *ch, int cmd, char *arg,
                struct char_data *mob, int type)
{
    struct char_data *freshmob;
    struct room_data *rp;
    struct obj_data *obj;
    char                buf[MAX_STRING_LENGTH];

    if (!ch) {
        return (FALSE);
    }
    if (!IS_NPC(ch)) {
        return (FALSE);
    }
    /*
     * if event death, do the die thing and load up a new mob at a
     * spawn_room 
     */
    if (type == EVENT_DEATH) {
        freshmob = read_mobile(real_mobile(NIGHTWALKER), REAL);
        char_to_room(freshmob, SPAWNROOM);
        act("$n crumbles to a pile of dust, but a tiny cloud seems to escape.",
            FALSE, ch, 0, 0, TO_ROOM);
        return (TRUE);
    }

    /*
     * else if light && not in spawn_room && not in a DARK room then burn
     * to a cinder, and load up a new one in the spawn room 
     */
    if (ch->in_room != SPAWNROOM && ch->in_room != WAITROOM && 
        !IS_SET(real_roomp(ch->in_room)->room_flags, DARK) && 
        time_info.hours > 6 && time_info.hours < 19) {
        act("A young ray of sunlight peeps over the horizon and strikes $n.",
            FALSE, ch, 0, 0, TO_ROOM);
        act("The power of the beam instantly reduces $n to a pile of dust!",
            FALSE, ch, 0, 0, TO_ROOM);
        GET_HIT(ch) = -1;
        die(ch, '\0');
        return (TRUE);
    }

    /*
     * Make them enter portal if they're in the right spot 
     */
    if (ch->in_room && ch->in_room == WAITROOM && (rp = real_roomp(WAITROOM))) {
        for (obj = rp->contents; obj; obj = obj->next_content) {
            if (obj_index[obj->item_number].virtual == TARANTIS_PORTAL) {
                strcpy(buf, "portal");
                do_enter(ch, buf, 7);
                return (FALSE);
            }
        }
    }
    return (FALSE);
}


/*
 * start Talesian's procs 
 */

/* 
 * vnum assignments
 */
#define WAITTOGOHOME  98765
#define INPEACEROOM  98764
#define CORPSEOBJVNUM  37817
#define VENTOBJVNUM 37813
#define VENTROOMVNUM 37865
#define PHYLOBJVNUM 37812
#define MOBROOMVNUM 37859
#define HOMEZONE 115
#define ZOMBIEVNUM 37800
#define SKELETONVNUM 37801
/* 
 * vnums for the rooms where the 'specialty' mobs are stored
 */
#define DEMILICHSTORAGE  37860
#define DRACOLICHSTORAGE  37861
#define DEATHKNIGHTSTORAGE  37862
#define VAMPIRESTORAGE  37863
#define UNDEADDEMONSTORAGE 37864
#define GUARDIANMOBVNUM 37803   
/* 
 * MUST be same as guardianextraction proc mob
 */
#define GUARDIANHOMEROOMVNUM 37823
#define CLOSETROOMVNUM 37839
#define MISTROOMVNUM 37866
#define GENERICMOBVNUM 37816

/*
 * @Name:           sageactions 
 * @description:    The sage acts like a lich with phylactery.  
 *                  If the item (37812) is not where it should 
 *                  be, he chases it.  If it doesn't exist at all, 
 *                  he dies.  In combat he can summon help
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37802) 
 */
int sageactions(struct char_data *ch, int cmd, char *arg,
                struct char_data *mob, int type)
{
    int             i = 0;
    int             j = 0;
    int             temp = 0;
    int             currroomnum = -1;
    char            buf[80];
    struct obj_data *curritem;
    struct obj_data *theitem = NULL;
    struct obj_data *ventobj;
    struct room_data *ventroom;
    struct obj_data *corpse;
    struct obj_data *tempobj;
    struct obj_data *nextobj;
    struct char_data *tempchar;
    struct room_data *currroom;
    int             k = 1;
    struct obj_data *remobj;
    struct obj_data *parentobj;
    int             GoodItemReal = -1;
    int             whatundead = 1;
    int             realcorpseid = -1;
    int             ventobjrnum = -1;

    if (cmd) {
        return (FALSE);
    }

    /* 
     * For the corpse that we toss around
     */
    GoodItemReal = real_object(PHYLOBJVNUM);
    realcorpseid = real_object(CORPSEOBJVNUM);
    ventroom = real_roomp(VENTROOMVNUM);
    ventobjrnum = real_object(VENTOBJVNUM);
    ventobj = get_obj_in_list_num(ventobjrnum, ventroom->contents);
    if(ventobjrnum == -1 || realcorpseid == -1 || GoodItemReal == -1) {
        /*
        * There are objects missing, which probably means all the
        * objects for the zone are bad.  Remove the mob.
         */
        Log("spec_procs4.c: sageactions() - could not find necessary object");
        for (j = 0; j < MAX_WEAR; j++) {
            if ((tempobj = mob->equipment[j])) {
                if (tempobj->contains) {
                    obj_to_char(unequip_char(mob, j), mob);
                } else {
                    MakeScrap(mob, NULL, tempobj);
                }
            }
        }

        while ((tempobj = mob->carrying)) {
            if (!tempobj->contains) {
                MakeScrap(mob, NULL, tempobj);
            } else {
                while ((nextobj = tempobj->contains)) {
                    obj_from_obj(nextobj);
                    obj_to_char(nextobj, mob);
                }
            }
        }
        extract_char(mob);
        return (FALSE);
    }

    if (mob->specials.fighting || GET_POS(mob) < POSITION_STANDING) {
        if (!number(0, 3)) {
            /* 
             * 1 in 4 chance of summoning an undead 
             * helper - the special caster types load 
             * with zone in a dark-no-everything room, 
             * then get transferred in, so that they 
             * are spelled up
             */
            whatundead = number(1, 75);

            switch (whatundead) {
            case 1:
                /* 
                 * demi-lich v25000 r2069 in DEMILICHSTORAGE 
                 * if demi-lich in rnum
                 */
                if ((tempchar = get_char_room("demi lich", DEMILICHSTORAGE))) {
                    do_say(mob, "I call for you, your debt will be removed for"
                                " this service!", 0);
                    char_from_room(tempchar);
                    char_to_room(tempchar, mob->in_room);
                    act("$n suddenly appears, with cold hostility in the "
                        "remains of its eyes.", TRUE, tempchar, 0, 0, TO_ROOM);
                    AttackRandomChar(tempchar);
                }
                break;
            case 2:
                /*
                 * dracolich v5010 r942 in DRACOLICHSTORAGE 
                 * if dracolich in rnum
                 */
                if ((tempchar = get_char_room("dracolich lich",
                                              DRACOLICHSTORAGE))) {
                    do_say(mob, "I helped create you, come and fulfill your "
                                "debt!", 0);
                    char_from_room(tempchar);
                    char_to_room(tempchar, mob->in_room);
                    act("$n abruptly appears, filling the room with its bulk "
                        "and its malevolence.", TRUE, tempchar, 0, 0, TO_ROOM);
                    AttackRandomChar(tempchar);
                }
                /* 
                 * otherwise he has enough help!
                 */
                break;
            case 3:
                /* 
                 * The Wretched Vampire v2573 r2473 in VAMPIRESTORAGE 
                 * if The Wretched Vampire in rnum
                 */
                if ((tempchar = get_char_room("The Wretched Vampire",
                                              VAMPIRESTORAGE))) {
                    do_say(mob, "I provide you with sustenance, avail yourself"
                                " and aid me!", 0);
                    char_from_room(tempchar);
                    char_to_room(tempchar, mob->in_room);
                    act("A swirl of darkness coalesces, and the horrific figure"
                        " of $n becomes visible.", TRUE, tempchar, 0, 0, 
                        TO_ROOM);
                    AttackRandomChar(tempchar);
                }
                /* 
                 * otherwise he has enough help!
                 */
                break;
            case 4:
                /* 
                 * death-knight v16219 r1664 in DEATHKNIGHTSTORAGE
                 * if death knight in rnum
                 */
                if ((tempchar = get_char_room("death knight",
                                              DEATHKNIGHTSTORAGE))) {
                    do_say(mob, "I can aid you in your quest of death if you "
                                "come to my side now!", 0);
                    GET_RACE(tempchar) = RACE_UNDEAD;
                    char_from_room(tempchar);
                    char_to_room(tempchar, mob->in_room);
                    act("A dark portal appears, and the terrifying figure of "
                        "the $n steps through.", TRUE, tempchar, 0, 0, TO_ROOM);
                    AttackRandomChar(tempchar);
                }
                /* 
                 * else he has enough help!
                 */
                break;
            case 5:
                /* 
                 * Ghastly Undead Demon v23022 r2439 in UNDEADDEMONSTORAGE
                 * if ghastly-undead-demon in rnum
                 */
                if ((tempchar = get_char_room("Ghastly Undead Demon",
                                              UNDEADDEMONSTORAGE))) {
                    do_say(mob, "I will remove your torment, if you remove my "
                                "foes!", 0);
                    char_from_room(tempchar);
                    char_to_room(tempchar, mob->in_room);
                    act("From the ground, $n rises up, clawing his way towards "
                        "you.", TRUE, tempchar, 0, 0, TO_ROOM);
                    AttackRandomChar(tempchar);
                }
                /* 
                 * else he has enough help!
                 */
                break;
            default:
                /* 
                 * load a slightly random, but straight fighting undead
                 */
                act("$n makes an arcane gesture and shouts, 'Come servant, "
                    "come to my aid!'", TRUE, mob, 0, 0, TO_ROOM);
                switch (number(1, 50)) {
                case 1:
                    /* 
                     * Death v6999 r1118
                     */
                    CreateAMob(mob, 6999, 5, "");
                    break;
                case 2:
                    /* 
                     * master-centaur v13769 r1446
                     */
                    CreateAMob(mob, 13769, 5, "");
                    break;
                case 3:
                    /* 
                     * ringwraith v26900 r2576
                     */
                    CreateAMob(mob, 26900, 5, "");
                    break;
                case 4:
                    /* 
                     * ghost-captain v21140 r1902
                     */
                    CreateAMob(mob, 21140, 5, "");
                    break;
                case 5:
                    /* 
                     * soul-lost v30118 r2239
                     */
                    CreateAMob(mob, 30118, 5, "");
                    break;
                default:
                    /* 
                     * load either a zombie or a skeleton
                     */
                    if (number(0, 1)) {
                        /* 
                         * zombie v37800 r3104
                         */
                        CreateAMob(mob, ZOMBIEVNUM, 5, "");
                    } else {
                        /*
                         * skeleton v37801 r3105
                         */
                        CreateAMob(mob, SKELETONVNUM, 5, "");
                    }
                    break;
                }
                break;
            }
        }

        return (necromancer(mob, cmd, arg, mob, type));
    }

    for (curritem = object_list; curritem; curritem = curritem->next) {
        if (curritem->item_number == GoodItemReal) {
            theitem = curritem;
            i++;
        }
    }

    if (!number(0, 2)) {
        return (necromancer(mob, cmd, arg, mob, type));
    }

    if (i == 0) {
        /* 
         * Kill off char, and all that he carries
         */
        act("$n suddenly screams in agony and falls into a pile of dust.",
            FALSE, mob, 0, 0, TO_ROOM);

        for (j = 0; j < MAX_WEAR; j++) {
            if ((tempobj = mob->equipment[j])) {
                if (tempobj->contains) {
                    obj_to_char(unequip_char(mob, j), mob);
                } else {
                    MakeScrap(mob, NULL, tempobj);
                }
            }
        }

        while ((tempobj = mob->carrying)) {
            if (!tempobj->contains) {
                MakeScrap(mob, NULL, tempobj);
            } else {
                while ((nextobj = tempobj->contains)) {
                    obj_from_obj(nextobj);
                    obj_to_char(nextobj, mob);
                }
            }
        }

        GET_RACE(mob) = RACE_UNDEAD_ZOMBIE;
        damage(mob, mob, GET_MAX_HIT(mob) * 2, TYPE_SUFFERING);
        return (TRUE);
    }
    
    if (i == 1) {
        /* 
         * ok there's only one item found, now what is it on?
         * Check to see if it's in the correct obj, if not go hunt it
         */
        if (theitem->in_obj == ventobj) {
            /* 
             * In correct obj
             */
            if (mob->generic == WAITTOGOHOME) {
                currroom = real_roomp(mob->in_room);
                temp = 0;
                for (corpse = currroom->contents; corpse;
                     corpse = corpse->next_content) {
                    if (corpse->item_number == realcorpseid) {
                        temp = 1;
                        break;
                    }
                }

                if (temp) {
                    /* 
                     * remove the corpse, head back for home;
                     */
                    act("With a glance, $n dissolves the corpse, looking "
                        "stronger for it.", TRUE, mob, 0, 0, TO_ROOM);
                    extract_obj(corpse);
                }

                act("$n seems to biodegrade into nothingness, leaving only"
                    " the stench of decay.", TRUE, mob, 0, 0, TO_ROOM);
                char_from_room(mob);
                char_to_room(mob, MOBROOMVNUM);

                act("A smell of death and decay wafts by as $n emerges "
                    "from nothingness.", TRUE, mob, 0, 0, TO_ROOM);
                mob->generic = 0;
                return (TRUE);
            } 

            mob->generic = 0;
            return (FALSE);
        } 
        
        /* 
         * Not in the vent
         */
        if (theitem->in_room != -1) {
            /* 
             * In room, just go get it (but use messages, and 
             * ch->generic to have him take his time)
             */
            if (mob->generic != theitem->in_room) {
                /* 
                 * we will set ch->generic to in_room, and drop
                 * corpse there, after that we will go to corpse
                 * and get item, then return home
                 */
                mob->generic = theitem->in_room;
                for (tempobj = real_roomp(theitem->in_room)->contents;
                     tempobj; tempobj = tempobj->next) {
                    if (tempobj->item_number == realcorpseid) {
                        return (TRUE);
                    }
                }

                corpse = read_object(CORPSEOBJVNUM, VIRTUAL);
                obj_to_room(corpse, theitem->in_room);
                currroomnum = mob->in_room;
                char_from_room(mob);
                char_to_room(mob, mob->generic);
                act("An old rotted corpse suddenly appears ten feet "
                    "above the ground and falls to the ground with a "
                    "sickening thud.", FALSE, mob, 0, 0, TO_ROOM);
                char_from_room(mob);
                char_to_room(mob, currroomnum);
            } else {
                /* 
                 * go to room, send item to storage, wait a pulse, 
                 * set ch->generic to go home again
                 */
                temp = 0;
                for (tempobj = real_roomp(theitem->in_room)->contents;
                     tempobj; tempobj = tempobj->next) {
                    if (tempobj->item_number == realcorpseid) {
                        temp = 1;
                    }
                }
                /* Even though the generic and in_room match, there is no corpse
                 * so we need to drop one, and that will be our action this time.
                 * If the corpse is there, then we will take the next step.
                 */
                if(!temp) {
                    corpse = read_object(CORPSEOBJVNUM, VIRTUAL);
                    obj_to_room(corpse, theitem->in_room);
                    currroomnum = mob->in_room;
                    char_from_room(mob);
                    char_to_room(mob, mob->generic);
                    act("An old rotted corpse suddenly appears ten feet "
                        "above the ground and falls to the ground with a "
                        "sickening thud.", FALSE, mob, 0, 0, TO_ROOM);
                    char_from_room(mob);
                    char_to_room(mob, currroomnum);
                    return(TRUE);
                }
                if(theitem->in_room != mob->in_room) {
                act("$n waves $s hands, and a pair of rotted hands "
                    "reaches up through the ground and drags $m under.",
                    TRUE, mob, 0, 0, TO_ROOM);
                char_from_room(mob);
                char_to_room(mob, mob->generic);
                act("The rotting corpse suddenly sits up, it reaches "
                    "down into the ground like it was water, and pulls "
                    "up another being.", FALSE, mob, 0, 0, TO_ROOM);
                }

                if (theitem->in_room == mob->in_room) {
                    act("$n gestures and a bauble disappears from the "
                        "ground.", TRUE, mob, 0, 0, TO_ROOM);
                    obj_from_room(theitem);
                    obj_to_obj(theitem, ventobj);
                    mob->generic = WAITTOGOHOME;
                } else {
                    act("$n looks frustrated by something.", TRUE,
                        mob, 0, 0, TO_ROOM);
                    act("$n seems to biodegrade into nothingness, "
                        "leaving only the stench of decay.", TRUE, mob,
                        0, 0, TO_ROOM);
                    char_from_room(mob);
                    char_to_room(mob, MOBROOMVNUM);
                    
                    act("A smell of death and decay wafts by as $n "
                        "emerges from nothingness.", TRUE, mob, 0, 0,
                        TO_ROOM);
                    mob->generic = 0;
                }
            }
            return (TRUE);
        }
        
        /* 
         * object exists, but not in room, must be 
         * on person or in container in room
         */
        temp = 0;
        for (tempchar = character_list; tempchar;
             tempchar = tempchar->next) {
            if (HasObject(tempchar, PHYLOBJVNUM)) {
                temp = 1;
                break;
            }
        }

        if (temp) {
            /* 
             * a character has it let's go get him/her
             */
            if (real_roomp(tempchar->in_room)->zone == HOMEZONE) {
                /* 
                 * don't go hunting if char is still in zone
                 */
                if (mob->in_room == tempchar->in_room && 
                    mob->generic != INPEACEROOM) {
                    /* 
                     * what to do when you find him
                     */
                    if (!IS_SET(real_roomp(mob->in_room)->room_flags,
                                PEACEFUL)) {
                        act("$n glares at you, and launches to the "
                            "attack!", TRUE, mob, 0, tempchar, TO_VICT);
                        act("$n suddenly launches $mself at $N!",
                            TRUE, mob, 0, tempchar, TO_NOTVICT);
                        MobHit(mob, tempchar, 0);
                    } else {
                        sprintf(buf, "I will wait for you, %s.",
                                GET_NAME(tempchar));
                        do_say(mob, buf, 0);
                        act("$n becomes completely motionless.",
                            TRUE, mob, 0, 0, TO_ROOM);
                        mob->generic = INPEACEROOM;
                    }
                }
                return (FALSE);
            }
            if (mob->in_room == tempchar->in_room &&
                mob->generic != INPEACEROOM) {

                if (!IS_SET(real_roomp(mob->in_room)->room_flags,
                            PEACEFUL)) {
                    act("$n glares at you, and launches to the attack!",
                        TRUE, mob, 0, tempchar, TO_VICT);
                    act("$n suddenly launches $mself at $N!", TRUE, mob,
                        0, tempchar, TO_NOTVICT);
                    MobHit(mob, tempchar, 0);
                    mob->generic = WAITTOGOHOME;
                    return (FALSE);
                }
            }
            if(mob->in_room == tempchar->in_room &&
                mob->generic == INPEACEROOM) {

                if(!number(0,2)) {
                    act("$n glares at you!",
                        TRUE, mob, 0, tempchar, TO_VICT);
                    act("$n glares at $N!", TRUE, mob,
                        0, tempchar, TO_NOTVICT);
                }
                return(FALSE);
            }

            if (mob->generic != tempchar->in_room) {
                /* 
                 * mob has not dropped a corpse in the correct 
                 * room yet
                 */
                mob->generic = tempchar->in_room;
                for (tempobj = real_roomp(tempchar->in_room)->contents;
                     tempobj; tempobj = tempobj->next) {
                    if (tempobj->item_number == realcorpseid) {
                        return (TRUE);
                    }
                }
                corpse = read_object(CORPSEOBJVNUM, VIRTUAL);
                obj_to_room(corpse, mob->generic);
                currroomnum = mob->in_room;
                char_from_room(mob);
                char_to_room(mob, tempchar->in_room);

                act("An old rotted corpse suddenly appears ten feet "
                    "above the ground and falls to the ground with a "
                    "sickening thud.", FALSE, mob, 0, 0, TO_ROOM);
                char_from_room(mob);
                char_to_room(mob, currroomnum);
                return (TRUE);
            } 
            
            if(mob->generic == tempchar->in_room) {
            /* 
             * mob dropped corpse in room, and char is still there 
             * so use corpse to transfer mob, then attack if not 
             * peace room
             */
                temp = 0;
                for (tempobj = real_roomp(tempchar->in_room)->contents;
                     tempobj; tempobj = tempobj->next) {
                    if (tempobj->item_number == realcorpseid) {
                        temp = 1;
                    }
                }
                if(!temp) {
                    mob->generic = -1;
                    return(FALSE);
                }
            act("$n waves $s hands, and a pair of rotted hands reaches "
                "up through the ground and drags $m under.", TRUE, mob,
                0, 0, TO_ROOM);
            char_from_room(mob);
            char_to_room(mob, mob->generic);

            act("The rotting corpse suddenly sits up, it reaches down "
                "into the ground like it was water, and pulls up "
                "another being.", FALSE, mob, 0, 0, TO_ROOM);
            act("$n points at you, that can't be good.",
                TRUE, mob, 0, tempchar, TO_VICT);
            act("$n slowly points at $N, which bodes ill for $N.",
                TRUE, mob, 0, tempchar, TO_NOTVICT);

            if (mob->in_room == tempchar->in_room &&
                mob->generic != INPEACEROOM) {
                if (!IS_SET(real_roomp(mob->in_room)->room_flags,
                            PEACEFUL)) {
                    act("$n glares at you, and launches to the attack!",
                        TRUE, mob, 0, tempchar, TO_VICT);
                    act("$n suddenly launches $mself at $N!", TRUE, mob,
                        0, tempchar, TO_NOTVICT);
                    MobHit(mob, tempchar, 0);
                    mob->generic = WAITTOGOHOME;
                    return (TRUE);
                } 

                sprintf(buf, "I will wait for you, %s.",
                        GET_NAME(tempchar));
                do_say(mob, buf, 0);
                act("$n becomes completely motionless.", TRUE, mob, 0,
                    0, TO_ROOM);
                mob->generic = INPEACEROOM;
                return (TRUE);
            }
            return (FALSE);
        }
        }
        
        /* 
         * in container, in room -> how about
         * moving outermost container to room,
         * then moving some items out of it, until 
         * the item is out
         */
        if (theitem->in_obj != NULL && theitem->in_obj != ventobj) {
            parentobj = theitem->in_obj;
            while (parentobj->in_obj != NULL) {
                parentobj = parentobj->in_obj;
            }
            
            /* 
             * Let's grab the parent object from the
             * floor, move it to the vent
             */
            currroomnum = mob->in_room;
            char_from_room(mob);
            char_to_room(mob, parentobj->in_room);

            act("A pair of skeletal hands reaches up from the earth and"
                " grabs $p, pulling it down into the ground.", TRUE,
                mob, parentobj, 0, TO_ROOM);
            char_from_room(mob);
            char_to_room(mob, currroomnum);
            obj_from_room(parentobj);
            obj_to_obj(parentobj, ventobj);

            while (theitem->in_obj != ventobj) {
                /* 
                 * it's in a container take out items until it's not
                 */
                parentobj = theitem->in_obj;
                while (parentobj->contains) {
                    tempobj = parentobj->contains;
                    obj_from_obj(tempobj);
                    obj_to_obj(tempobj, ventobj);
                }
            }
        }
        return (FALSE);
    } 
    
    if (i > 1) {
        /* 
         * I say randomly remove all but one, then hunt it
         */
        j = number(1, i);
        for (curritem = object_list; curritem;) {
            if (curritem->item_number == GoodItemReal) {
                remobj = curritem;
                curritem = curritem->next;
                if (k != j) {
                    extract_obj(remobj);
                }
                k++;
            } else {
                curritem = curritem->next;
            }
        }
    }

    return (necromancer(mob, cmd, arg, mob, type));
}



/*
 * @Name:           guardianextraction 
 * @description:    A mob function used by the guardians 
 *                  from guardianroom to clean themselves 
 *                  out of the room if there are no PC's 
 *                  there to fight, also resets the trap 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    room(37823) 
 */

int guardianextraction(struct char_data *ch, int cmd, char *arg,
                       struct char_data *mob, int type)
{
    struct char_data *tempchar;
    int             i = 0;
    struct room_data *rp;
    struct char_data *next_v;

    if(cmd) {
        return (FALSE);
    }

    if (mob->in_room != GUARDIANHOMEROOMVNUM) {
        return (FALSE);
    }

    rp = real_roomp(mob->in_room);

    for (tempchar = rp->people; tempchar;
         tempchar = tempchar->next_in_room) {
        if (IS_PC(tempchar) && !IS_IMMORTAL(tempchar)) {
            i++;
        }
    }

    if (i == 0) {
        for (tempchar = rp->people; tempchar; tempchar = next_v) {
            next_v = tempchar->next_in_room;
            if (IS_NPC(tempchar) && 
                !IS_SET(tempchar->specials.act, ACT_POLYSELF) &&
                mob_index[tempchar->nr].virtual == GUARDIANMOBVNUM) {
                extract_char(tempchar);
            }
        }
        rp->special = 0;
    }
    return (FALSE);
}


/*
 * @Name:           confusionmob 
 * @description:    This mob will attempt to make players flee 
 *                  using their 'best' method. Uses ability scores 
 *                  as saves rather than saves vs spells 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37811) 
 */

/*
 * Ideally, I'd like this to force characters to do semi-random 
 * commands instead of fleeing (maybe 3 out of 4 are random, 1 is 
 * user choice), but that would probably mean adding an affect
 */
int confusionmob(struct char_data *ch, int cmd, char *arg,
                 struct char_data *mob, int type)
{
    struct char_data *tempchar;
    int             makethemflee = 1;
    int             currroomnum = 0;

    if(cmd) {
        return(FALSE);
    }

    if ((tempchar = mob->specials.fighting)) {
        if (number(0, 4) > 0) {
            if (dice(3, 7) < GET_CHR(tempchar) &&
                dice(3, 7) < GET_INT(tempchar) &&
                dice(3, 7) < GET_WIS(tempchar)) {
                /* 
                 * three saves to avoid it fail any of the three, and it's 
                 * time to flee, this one is charisma
                 * to show what kind of confidence you have in yourself
                 * two saves to avoid it, this one intelligence to 
                 * understand what it is trying to do
                 * final save to avoid it, this one wisdom, to show 
                 * strength of will to avoid fleeing
                 * 3d7 = 12 average
                 */
                        
                /*
                 * they made 3 saves, I guess they can stay
                 */
                makethemflee = 0;
            }

            if (makethemflee) {
                currroomnum = tempchar->in_room;
                act("You gaze into $n's eyes and suddenly get frightened for no"
                    " reason!", FALSE, mob, 0, tempchar, TO_VICT);
                act("$n stares at $N and they suddenly look very scared!  "
                    "They've even stopped fighting back!", FALSE, mob, 0,
                    tempchar, TO_NOTVICT);

                /* 
                 * mages teleport
                 */
                stop_fighting(tempchar);
                if (HasClass(tempchar, CLASS_MAGIC_USER)) {
                    act("You panic and attempt to teleport to safety!",
                        FALSE, tempchar, 0, 0, TO_CHAR);
                    spell_teleport(GET_LEVEL(tempchar, MAGE_LEVEL_IND),
                                   tempchar, tempchar, 0);
                } else if (HasClass(tempchar, CLASS_CLERIC)) {
                    /* 
                     * clerics word and necromancers bind
                     */
                    act("You panic and attempt to recall to safety!",
                        FALSE, tempchar, 0, 0, TO_CHAR);
                    spell_word_of_recall(GET_LEVEL(tempchar, CLERIC_LEVEL_IND),
                                         tempchar, tempchar, 0);
                } else if (HasClass(tempchar, CLASS_NECROMANCER)) {
                    act("You panic and attempt to step through the shadows to "
                        "safety!", FALSE, tempchar, 0, 0, TO_CHAR);
                    spell_binding(GET_LEVEL(tempchar, NECROMANCER_LEVEL_IND),
                                  tempchar, tempchar, 0);
                } else if (HasClass(tempchar, CLASS_PSI)) {
                    /* 
                     * psis doorway to Zifnab
                     */
                    act("You panic and attempt to doorway to safety!",
                        FALSE, tempchar, 0, 0, TO_CHAR);
                    command_interpreter(tempchar, "doorway zifnab");
                } else {
                    /* 
                     * warriors types, druids, thieves, sorcerers flee
                     */
                    act("You panic and attempt to run away!", FALSE,
                        tempchar, 0, 0, TO_CHAR);
                    do_flee(tempchar, NULL, 0);
                }

                if (tempchar->in_room == currroomnum) {
                    act("You failed to get away, run!  RUN AWAY NOW!",
                        FALSE, tempchar, 0, 0, TO_CHAR);
                    do_flee(tempchar, NULL, 0);
                }
            }
        }
    }
    return (FALSE);
}


/*
 * @Name:           ghastsmell 
 * @description:    A mob proc which will disease people,
 *                  but only if the mob is fighting, and 
 *                  only when a player does a command, 
 *                  so the more you struggle, the worse it 
 *                  gets 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37804) 
 */
/* 
 * ghast smell which makes all in room get 
 * diseased if they are fighting someone
 */
int ghastsmell(struct char_data *ch, int cmd, char *arg,
               struct char_data *mob, int type)
{
    struct char_data *tempchar;
    struct affected_type af;
    struct room_data *rp;

    if(!mob) {
        return (FALSE);
    }

    if (!number(0, 1)) {
        return (FALSE);
    }

    if (mob->specials.fighting) {
        af.type = SPELL_DISEASE;
        af.duration = 6;
        af.modifier = 0;
        af.location = 0;
        af.bitvector = 0;

        rp = real_roomp(mob->in_room);

        for (tempchar = rp->people; tempchar;
             tempchar = tempchar->next_in_room) {
            if (!IS_NPC(tempchar) && !IS_IMMORTAL(tempchar)) {
                act("A wave of stench rolls out from $n, "
                    "if a skunk's spray is like a dagger, \n\rthis is "
                    "like a claymore.", TRUE, mob, 0, tempchar, TO_VICT);
                if (!saves_spell(tempchar, SAVING_BREATH)) {
                    act("You immediately feel incredibly sick.", TRUE, mob, 0,
                        tempchar, TO_VICT);
                    affect_join(tempchar, &af, TRUE, FALSE);
                }
            }
        }
    }

    return (FALSE);
}

/*
 * @Name:           ghoultouch 
 * @description:    A mob proc which will stun people at
 *                  pretty much every opportunity, probably 
 *                  more of an annoyance than truly deadly 
 *                  if you have a group, but potentially 
 *                  killer if you are attempting to solo 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37805) 
 */
/* 
 * ghoul touch which does stunned for a round (GET_POS(ch) == POS_STUNNED, 
 * WAIT_STATE) (make this happen even if bashed)
 */
int ghoultouch(struct char_data *ch, int cmd, char *arg,
               struct char_data *mob, int type)
{
    struct char_data *opponent;

    if( !mob ) {
        return( FALSE );
    }

    opponent = mob->specials.fighting;

    if (opponent && number(0, 1) && !IS_IMMUNE(opponent, IMM_HOLD)) {
        act("$n reaches out and touches you, you reel, losing focus!",
            FALSE, mob, 0, opponent, TO_VICT);
        act("$n reaches out and sends $N reeling, falling to the ground!",
            FALSE, mob, 0, opponent, TO_NOTVICT);
        GET_POS(opponent) = POSITION_STUNNED;
        stop_fighting(opponent);
        WAIT_STATE(opponent, PULSE_VIOLENCE);
    }

    return (FALSE);
}

/*
 * @Name:           shadowtouch 
 * @description:    A mob proc which will weaken, and tire 
 *                  opponents.  Weaken bigtime, down to a 
 *                  nautral 3 str + magical effects, and 
 *                  move get reduced 40% each time 
 * @Author:         Rick Peplinski (Talesian)
 * @Assigned to:    mob(37806) 
 */
/* 
 * shadow touch which does -str and -move
 */
int shadowtouch(struct char_data *ch, int cmd, char *arg,
                struct char_data *mob, int type)
{

    struct char_data *opponent;
    struct affected_type af;
    int             oppstr = 0;
    int             mod = 0;

    if( !mob ) {
        return( FALSE );
    }

    opponent = mob->specials.fighting;

    if (opponent && number(0, 1) && !IS_IMMUNE(opponent, IMM_DRAIN)) {
        act("$n reaches out and touches you, your limbs lose all their "
            "strength!", FALSE, mob, 0, opponent, TO_VICT);
        act("$n reaches out and touches $N, suddenly they look exhausted.",
            FALSE, mob, 0, opponent, TO_NOTVICT);

        if (!affected_by_spell(opponent, SPELL_WEAKNESS)) {
            if(GET_RADD(opponent) > 0) {
                oppstr += GET_RADD(opponent) / 10;
            }
            oppstr += GET_RSTR(opponent);
            /* 
             * basically take their natural strength and 
             * subtract enough to leave them with 3, 
             * anything magical added afterwards is ok
             */
            mod = oppstr - 3;
            af.type = SPELL_WEAKNESS;
            af.duration = 3;
            af.modifier = 0 - mod;
            af.location = APPLY_STR;
            af.bitvector = 0;
            affect_to_char(opponent, &af);
        }
        GET_MOVE(opponent) = (int) GET_MOVE(opponent) * .6;
    }
    return (FALSE);
}

/*
 * @Name:           moldexplosion 
 * @description:    A mob proc which if a player kills the mob, 
 *                  it poisons everybody in the room, no save 
 *                  except IMM_POISON.
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37807) 
 */
/* 
 * mold, which is fine if left alone, and 
 * blows up and poisons everybody
 * in room if it dies : type == EVENT_DEATH
 */
int moldexplosion(struct char_data *ch, int cmd, char *arg,
                  struct char_data *mob, int type)
{

    struct char_data *tempchar;
    struct affected_type af;

    if (type != EVENT_DEATH || cmd) {
        return (FALSE);
    }

    if (type == EVENT_DEATH) {
        af.type = SPELL_POISON;
        af.duration = 3;
        af.modifier = 0;
        af.location = APPLY_NONE;
        af.bitvector = AFF_POISON;

        act("Suddenly, the mold explodes, throwing a huge cloud of spores into"
            " the air!", FALSE, mob, 0, 0, TO_ROOM);

        for (tempchar = real_roomp(mob->in_room)->people; tempchar;
             tempchar = tempchar->next_in_room) {
            if (!IS_IMMORTAL(tempchar) && !IS_IMMUNE(tempchar, IMM_POISON)) {
                affect_to_char(tempchar, &af);
                send_to_char("You feel very sick.\n\r", tempchar);
            }
        }
    }
    return (FALSE);
}

/*
 * @Name:           boneshardbreather 
 * @description:    A mob proc which does a breath weapon 
 *                  for a bone golem. 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    mob(37809) 
 */
/* 
 * bone golem (breathe bone shards!)
 */
int boneshardbreather(struct char_data *ch, int cmd, char *arg,
                      struct char_data *mob, int type)
{
    struct char_data *tempchar;
    int             dam;

    if (cmd) {
        return (FALSE);
    }

    if (mob->specials.fighting && !number(0, 3)) {
        act("The dragon skull embedded in the bone golem's chest opens wide\n\r"
            "and spews out shards of bones that slice like the sharpest of "
            "daggers!", FALSE, mob, 0, 0, TO_ROOM);

        for (tempchar = real_roomp(mob->in_room)->people; tempchar;
             tempchar = tempchar->next_in_room) {
            if (!IS_NPC(tempchar) && !IS_IMMORTAL(tempchar)) {
                dam = dice(25, 10);
                if (saves_spell(tempchar, SAVING_SPELL)) {
                    dam = dam >> 1;
                }
                doroomdamage(tempchar, dam, TYPE_SLASH);
            }
        }

    }
    return (FALSE);
}

/*
 * @Name:           mistgolemtrap 
 * @description:    A mob proc which will 'capture' a PC 
 *                  (transfer him to a no-magic, no exit, 
 *                  silenced room) but everybody will be 
 *                  returned when mist golem dies. 
 * @Author:         Rick Peplinski (Talesian)
 * @Assigned to:    mob(37810) 
 */
/* 
 * mist golem (sucks somebody in, teleports them to a room "The Mist"
 * where can't get out until mist golem dies)
 */
int mistgolemtrap(struct char_data *ch, int cmd, char *arg,
                  struct char_data *mob, int type)
{
    struct char_data *opponent;
    struct char_data *tempchar;
    struct char_data *nextchar;

    if (cmd) {
        return (FALSE);
    }

    if (type == EVENT_DEATH) {
        for (tempchar = real_roomp(MISTROOMVNUM)->people; tempchar;
             tempchar = nextchar) {
            nextchar = tempchar->next_in_room;
            char_from_room(tempchar);
            char_to_room(tempchar, mob->in_room);
            act("Suddenly, the mist disappears and you find yourself somewhere"
                " else!", TRUE, tempchar, 0, 0, TO_CHAR);
            act("Suddenly, $n appears, looking bewildered.", TRUE,
                tempchar, 0, 0, TO_ROOM);
            do_look(tempchar, NULL, 15);
        }
    }

    if (mob->specials.fighting && !number(0, 1)) {
        opponent = mob->specials.fighting;
        act("The mist golem suddenly leans forward and engulfs you!\n\r"
            "You find yourself in another place.", TRUE, opponent, 0, 0,
            TO_CHAR);
        act("The mist golem suddenly leans forward and engulfs $n!\n\r"
            "They disappear into the body of the golem.", TRUE, opponent, 0, 0,
            TO_ROOM);
        stop_fighting(opponent);
        stop_fighting(mob);
        char_from_room(opponent);
        char_to_room(opponent, MISTROOMVNUM);
        do_look(opponent, NULL, 15);
        AttackRandomChar(mob);
    }
    return (FALSE);
}

/*
 * @Name:           mirrorofopposition 
 * @description:    An obj proc which will dopplegang a PC.  
 *                  Copies just about everything I could think 
 *                  of, makes mob hate him, and attack him using 
 *                  his own equipment.  Players will probably really 
 *                  hate being on the receiving end of their +30 hasted
 *                  damrolls. 
 * @Author:         Rick Peplinski (Talesian) 
 * @Assigned to:    obj(37821) 
 */
int mirrorofopposition(struct char_data *ch, int cmd, char *arg,
                       struct obj_data *obj, int type)
{
    struct affected_type af2;
    struct char_data *mob;
    int             i;
    int             maxlevel;
    struct affected_type *af;
    struct obj_file_u st;
    struct obj_data *tempobj;
    int             total_equip_cost;
    char           *buf1;
    char           *buf2;
    FILE           *fl;

    if (obj->in_room == -1 || cmd != 15 || IS_IMMORTAL(ch)) {
        return (FALSE);
    }

    arg = get_argument(arg, &buf1);
    arg = get_argument(arg, &buf2);

    if (!buf1 || !buf2 || (strcmp(buf1, "mirror") && strcmp(buf2, "mirror"))) {
        return (FALSE);
    }

    /* 
     * CreateAMob -> generic mob (naked) -> no follower, no random attack
     */
    mob = CreateAMob(ch, GENERICMOBVNUM, 0,
                     "A figure suddenly steps out of the mirror.");

    /* 
     * start doing pc info:
     * name, title, classes, hitroll = MAX (1,20 - biggestlevel*.5), armor 
     * will be done by what's being worn
     */
    sprintf(buf1, "%s Bizarro", GET_NAME(ch));
    mob->player.name = (char *) strdup(buf1);
    mob->player.short_descr = (char *) strdup(GET_NAME(ch));
    sprintf(buf1, "%s is standing here.", ch->player.title);
    mob->player.long_descr = (char *) strdup(buf1);
    mob->player.class = ch->player.class;

    for (i = 0; i < 12; i++) {
        mob->player.level[i] = ch->player.level[i];
    }

    maxlevel = GetMaxLevel(mob);
    GET_HITROLL(mob) = MAX(1, 20 - (maxlevel >> 1));
    /*
     * max (hp, mana, move) current (stats, race, spell affects)
     */
    mob->points.max_hit = GET_MAX_HIT(ch);
    mob->points.max_mana = GET_MAX_MANA(ch);
    mob->points.max_move = GET_MAX_MOVE(ch);
    mob->points.hit = GET_MAX_HIT(ch);
    mob->points.mana = GET_MAX_MANA(ch);
    mob->points.move = GET_MAX_MOVE(ch);
    mob->mult_att = ch->mult_att;
    GET_ALIGNMENT(mob) = GET_ALIGNMENT(ch);

    for (af = ch->affected; af; af = af->next) {
        affect_to_char(mob, af);
    }

    GET_RACE(mob) = GET_RACE(ch);
    GET_RSTR(mob) = GET_RSTR(ch);
    GET_RADD(mob) = GET_RADD(ch);
    GET_RCON(mob) = GET_RCON(ch);
    GET_RDEX(mob) = GET_RDEX(ch);
    GET_RWIS(mob) = GET_RWIS(ch);
    GET_RINT(mob) = GET_RINT(ch);
    GET_RCHR(mob) = GET_RCHR(ch);

    sprintf(buf1, "reimb/%s", lower(ch->player.name));
    if (!(fl = fopen(buf1, "r+b"))) {
        act("The figure looks around, and promptly disappears!", TRUE, mob,
            0, 0, TO_ROOM);
        char_from_room(mob);
        extract_char(mob);
        return (TRUE);
    }

    rewind(fl);

    if (!ReadObjs(fl, &st)) {
        act("The figure looks around, and promptly disappears!", TRUE, mob,
            0, 0, TO_ROOM);
        char_from_room(mob);
        extract_char(mob);
        return (TRUE);
    }

    for (i = 0; i < MAX_WEAR; i++) {
        if (mob->equipment[i])
            extract_obj(unequip_char(mob, i));
    }
    while (mob->carrying) {
        extract_obj(mob->carrying);
    }

    obj_store_to_char(mob, &st);

    while (mob->carrying) {
        extract_obj(mob->carrying);
    }

    /*
     * set all equipment to useless somehow (anti_everything? anti_gne?
     * norent? anti_sun? Organic_decay?)
     * lennya said maybe that timer could be used for any equipment, not
     * just specific equipment
     */
    total_equip_cost = 0;
    for (i = 0; i < MAX_WEAR; i++) {
        if (mob->equipment[i]) {

            total_equip_cost += mob->equipment[i]->obj_flags.cost;
            mob->equipment[i]->obj_flags.timer = 20;
            if (GET_ITEM_TYPE(mob->equipment[i]) == ITEM_CONTAINER) {
                while ((tempobj = mob->equipment[i]->contains)) {
                    extract_obj(tempobj);
                }
            }
            if(IS_OBJ_STAT(mob->equipment[i],ITEM_ANTI_GOOD)) {
                REMOVE_BIT(mob->equipment[i]->obj_flags.extra_flags, 
                           ITEM_ANTI_GOOD);
            }
            if(IS_OBJ_STAT(mob->equipment[i],ITEM_ANTI_EVIL)) {
                REMOVE_BIT(mob->equipment[i]->obj_flags.extra_flags, 
                           ITEM_ANTI_EVIL);
            }
            if(IS_OBJ_STAT(mob->equipment[i],ITEM_ANTI_NEUTRAL)) {
                REMOVE_BIT(mob->equipment[i]->obj_flags.extra_flags, 
                           ITEM_ANTI_NEUTRAL);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_NECROMANCER)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags, 
                        ITEM_ANTI_NECROMANCER);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_CLERIC)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_CLERIC);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_MAGE)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_MAGE);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_THIEF)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_THIEF);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_FIGHTER)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_FIGHTER);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_MEN)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_MEN);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_WOMEN)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_WOMEN);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_SUN)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_SUN);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_BARBARIAN)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_BARBARIAN);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_RANGER)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_RANGER);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_PALADIN)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_PALADIN);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_PSI)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_PSI);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_MONK)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_MONK);
            }
            if(!IS_OBJ_STAT(mob->equipment[i], ITEM_ANTI_DRUID)) {
                SET_BIT(mob->equipment[i]->obj_flags.extra_flags,
                        ITEM_ANTI_DRUID);
            }
        }
    }

    /* 
     * if has class to have 'normal' spells (sanc, fs, bb), add the affect 
     * (like casting, not by setting perma-flag)
     */
    if (IS_SET(mob->player.class, CLASS_NECROMANCER) &&
        !affected_by_spell(mob, SPELL_CHILLSHIELD) &&
        GET_LEVEL(ch,NECROMANCER_LEVEL_IND) > 39) {
        af2.type = SPELL_CHILLSHIELD;
        af2.duration = 3;
        af2.modifier = 0;
        af2.location = 0;
        af2.bitvector = AFF_CHILLSHIELD;
        affect_to_char(mob, &af2);
    }

    if (IS_SET(mob->player.class, CLASS_MAGIC_USER | CLASS_PSI) && 
        !IS_SET(mob->player.class, CLASS_CLERIC) &&
        !affected_by_spell(mob, SPELL_FIRESHIELD) &&
        (GET_LEVEL(ch,PSI_LEVEL_IND) > 14 ||
        GET_LEVEL(ch,MAGE_LEVEL_IND) > 39)) {
        af2.type = SPELL_FIRESHIELD;
        af2.duration = 3;
        af2.modifier = 0;
        af2.location = 0;
        af2.bitvector = AFF_FIRESHIELD;
        affect_to_char(mob, &af2);
    }

    if (IS_SET(mob->player.class, CLASS_CLERIC)) {
        if (!affected_by_spell(mob, SPELL_SANCTUARY)) {
            af2.type = SPELL_SANCTUARY;
            af2.duration = 3;
            af2.modifier = 0;
            af2.location = APPLY_NONE;
            af2.bitvector = AFF_SANCTUARY;
            affect_to_char(mob, &af2);
        }

        if (!affected_by_spell(mob, SPELL_BLADE_BARRIER) &&
            GET_LEVEL(ch,CLERIC_LEVEL_IND) > 44) {
            af2.type = SPELL_BLADE_BARRIER;
            af2.duration = 3;
            af2.modifier = 0;
            af2.location = APPLY_NONE;
            af2.bitvector = AFF_BLADE_BARRIER;
            affect_to_char(mob, &af2);
        }
    }
    
    /* 
     * Make him act like class he is if possible:
     */
    if (IS_SET(mob->player.class, CLASS_NECROMANCER)) {
        SET_BIT(mob->specials.act, ACT_NECROMANCER);
    }

    if (IS_SET(mob->player.class, CLASS_MAGIC_USER)) {
        SET_BIT(mob->specials.act, ACT_MAGIC_USER);
    }

    if (IS_SET(mob->player.class, CLASS_CLERIC)) {
        SET_BIT(mob->specials.act, ACT_CLERIC);
    }

    if (IS_SET(mob->player.class, CLASS_WARRIOR)) {
        SET_BIT(mob->specials.act, ACT_WARRIOR);
    }

    if (IS_SET(mob->player.class, CLASS_THIEF)) {
        SET_BIT(mob->specials.act, ACT_THIEF);
    }

    if (IS_SET(mob->player.class, CLASS_DRUID)) {
        SET_BIT(mob->specials.act, ACT_DRUID);
    }

    if (IS_SET(mob->player.class, CLASS_MONK)) {
        SET_BIT(mob->specials.act, ACT_MONK);
    }

    if (IS_SET(mob->player.class, CLASS_BARBARIAN)) {
        SET_BIT(mob->specials.act, ACT_BARBARIAN);
    }

    if (IS_SET(mob->player.class, CLASS_PALADIN)) {
        SET_BIT(mob->specials.act, ACT_PALADIN);
    }

    if (IS_SET(mob->player.class, CLASS_RANGER)) {
        SET_BIT(mob->specials.act, ACT_RANGER);
    }

    if (IS_SET(mob->player.class, CLASS_PSI)) {
        SET_BIT(mob->specials.act, ACT_PSI);
    }

    if (IsGiant(mob)) {
        SET_BIT(mob->specials.act, ACT_HUGE);
    }

    /* 
     * set experience, figure .005 of players total exp + equipment cost?
     */
    mob->points.exp = GET_EXP(ch) * .005 + total_equip_cost;
    GET_ALIGNMENT(mob) = -1 * GET_ALIGNMENT(ch);

    /* 
     * have him go to town on character
     */

    MobHit(mob, ch, 0);

    return (TRUE);
}

/*
 * start shopkeeper .. this to make shops easier to build -Lennya 20030731
 */
int shopkeeper(struct char_data *ch, int cmd, char *arg,
               struct char_data *shopkeep, int type)
{
    struct room_data *rp;
    struct obj_data *obj = NULL,
                   *cond_ptr[50],
                   *store_obj = NULL;
    char           *itemname,
                    newarg[100];
    float           modifier = 1.0;
    int             cost = 0,
                    chr = 1,
                    k,
                    i = 0,
                    stop = 0,
                    num = 1,
                    rnum = 0;
    int             tot_cost = 0,
                    cond_top = 0,
                    cond_tot[50],
                    found = FALSE;

    extern struct str_app_type str_app[];

    if (!ch) {
        return (FALSE);
    }
    if (!AWAKE(ch) || IS_NPC(ch)) {
        return (FALSE);
    }
    /*
     * define the shopkeep 
     */
    if (ch->in_room) {
        rp = real_roomp(ch->in_room);
    } else {
        Log("weirdness in shopkeeper, char not in a room");
        return (FALSE);
    }

    if (!rp) {
        Log("weirdness in shopkeeper, char's room does not exist");
        return (FALSE);
    }

    /*
     * let's make sure shopkeepers don't get killed or robbed 
     */
    if (!IS_SET(rp->room_flags, PEACEFUL)) {
        SET_BIT(rp->room_flags, PEACEFUL);
    }
    if (cmd != 59 && cmd != 56 && cmd != 93 && cmd != 57) {
        /* 
         * list  buy  offer  sell 
         */
        return (FALSE);
    }
    shopkeep = FindMobInRoomWithFunction(ch->in_room, shopkeeper);

    if (!shopkeep) {
        Log("weirdness in shopkeeper, shopkeeper assigned but not found");
        return (FALSE);
    }

    if (!IS_NPC(shopkeep)) {
        Log("weirdness in shopkeeper, shopkeeper is not a mob");
        return (FALSE);
    }

    if (!IS_SET(shopkeep->specials.act, ACT_SENTINEL)) {
        SET_BIT(shopkeep->specials.act, ACT_SENTINEL);
    }
    /*
     * players with 14 chr pay avg price 
     */
    chr = GET_CHR(ch);
    if (chr < 1) {
        chr = 1;
    }
    modifier = (float) 14 / chr;

    /*
     * list 
     */
    switch (cmd) {
    case 59:
        oldSendOutput(ch, "This is what %s currently has on store:\n\r\n\r",
                  shopkeep->player.short_descr);
        send_to_char("  Count  Item                                       "
                     "Price\n\r", ch);
        send_to_char("$c0008*---------------------------------------------"
                     "------------*\n\r", ch);
        obj = shopkeep->carrying;
        if (!obj) {
            send_to_char("$c0008|$c0007        Nothing.                    "
                         "                     $c0008|\n\r", ch);
            send_to_char("$c0008*------------------------------------------"
                         "---------------*\n\r", ch);
            break;
        }
        
        for (obj = shopkeep->carrying; obj; obj = obj->next_content) {
            if (CAN_SEE_OBJ(ch, obj)) {
                if (cond_top < 50) {
                    found = FALSE;
                    for (k = 0; (k < cond_top && !found); k++) {
                        if (cond_top > 0 &&
                            obj->item_number == cond_ptr[k]->item_number &&
                            obj->short_description && 
                            cond_ptr[k]->short_description && 
                            !strcmp(obj->short_description, 
                                    cond_ptr[k]->short_description)) {
                            cond_tot[k] += 1;
                            found = TRUE;
                        }
                    }

                    if (!found) {
                        cond_ptr[cond_top] = obj;
                        cond_tot[cond_top] = 1;
                        cond_top += 1;
                    }
                } else {
                    cost = (int) obj->obj_flags.cost * modifier;
                    if (cost < 0) {
                        cost = 0;
                    }
                    cost += 1000;   /* Trader's fee = 1000 */
                    oldSendOutput(ch, "$c0008|$c0007    1   %-41s %6d "
                                  "$c0008|\n\r", obj->short_description,
                              cost);
                }
            }
        }

        if (cond_top) {
            for (k = 0; k < cond_top; k++) {
                cost = (int) cond_ptr[k]->obj_flags.cost * modifier;
                if (cost < 0) {
                    cost = 0;
                }
                cost += 1000;   /* Trader's fee = 1000 */
                oldSendOutput(ch, "$c0008|$c0007 %4d   %-41s %6d $c0008|\n\r",
                          (cond_tot[k] > 1 ?  cond_tot[k] : 1),
                          cond_ptr[k]->short_description, cost);
            }
        }
        send_to_char("$c0008*------------------------------------------------"
                     "---------*\n\r", ch);
        break;
    case 56:
        /*
         * buy 
         */
        arg = get_argument(arg, &itemname);
        if (!itemname) {
            send_to_char("Buy what?\n\r", ch);
            return (TRUE);
        } 
        
        if ((num = getabunch(itemname, newarg)) != FALSE) {
            strcpy(itemname, newarg);
        }

        if (num < 1) {
            num = 1;
        }
        rnum = 0;
        stop = 0;
        i = 1;

        while (i <= num && stop == 0) {
            if ((obj = get_obj_in_list_vis(ch, itemname, shopkeep->carrying))) {
                cost = (int) obj->obj_flags.cost * modifier;
                if (cost < 0) {
                    cost = 0;
                }
                cost += 1000;       /* Trader's Fee is 1000 */

                if (GET_GOLD(ch) < cost) {
                    send_to_char("Alas, you cannot afford that.\n\r", ch);
                    stop = 1;
                } else if ((IS_CARRYING_N(ch) + 1) > (CAN_CARRY_N(ch))) {
                    oldSendOutput(ch, "%s : You can't carry that many items.\n\r",
                              obj->short_description);
                    stop = 1;
                } else
                    if ((IS_CARRYING_W(ch) + (obj->obj_flags.weight)) >
                        CAN_CARRY_W(ch)) {
                    oldSendOutput(ch, "%s : You can't carry that much weight.\n\r",
                              obj->short_description);
                    stop = 1;
                } else if (GET_LEVEL(ch, BARBARIAN_LEVEL_IND) != 0 &&
                           anti_barbarian_stuff(obj) && 
                           !IS_IMMORTAL(ch)) {
                    send_to_char("You sense magic on the object and think "
                                 "better of buying it.\n\r", ch);
                    stop = 1;
                } else {
                    obj_from_char(obj);
                    obj_to_char(obj, ch);
                    GET_GOLD(ch) -= cost;
                    GET_GOLD(shopkeep) += cost;
                    store_obj = obj;
                    i++;
                    tot_cost += cost;
                    rnum++;
                }
            } else if (rnum > 0) {
                oldSendOutput(ch, "Alas, %s only seems to have %d %ss on "
                              "store.\n\r",
                          shopkeep->player.short_descr, rnum, itemname);
                stop = 1;
            } else {
                oldSendOutput(ch, "Alas, %s doesn't seem to stock any %ss..\n\r",
                          shopkeep->player.short_descr, itemname);
                stop = 1;

            }
        }

        if (rnum == 1) {
            oldSendOutput(ch, "You just bought %s for %d coins.\n\r",
                      store_obj->short_description, cost);
            act("$n buys $p from $N.", FALSE, ch, obj, shopkeep, TO_ROOM);
        } else if (rnum > 1) {
            oldSendOutput(ch, "You just bought %d items for %d coins.\n\r",
                      rnum, tot_cost);
            act("$n buys some stuff from $N.", FALSE, ch, obj, shopkeep,
                TO_ROOM);
        }
        break;
    case 57:
    /*
     * sell 
     */
        arg = get_argument(arg, &itemname);
        if (!itemname) {
            send_to_char("Sell what?\n\r", ch);
            return (TRUE);
        }
        
        if ((obj = get_obj_in_list_vis(ch, itemname, ch->carrying))) {
            cost = (int) obj->obj_flags.cost / (3 * modifier);
            /*
             * lets not have shops buying non-rentables
             */
            if (obj->obj_flags.cost_per_day == -1) {
                oldSendOutput(ch, "%s doesn't buy items that cannot be rented.\n\r",
                          shopkeep->player.short_descr);
                return (TRUE);
            }

            if (cost < 400) {
                oldSendOutput(ch, "%s doesn't buy worthless junk like that.\n\r",
                          shopkeep->player.short_descr);
                return (TRUE);
            }
            
            if (GET_GOLD(shopkeep) < cost) {
                oldSendOutput(ch, "Alas, %s cannot afford that right now.\n\r",
                          shopkeep->player.short_descr);
                return (TRUE);
            }
            
            obj_from_char(obj);
            obj_to_char(obj, shopkeep);
            oldSendOutput(ch, "You just sold %s for %d coins.\n\r",
                      obj->short_description, cost);
            act("$n sells $p to $N.", FALSE, ch, obj, shopkeep, TO_ROOM);
            GET_GOLD(ch) += cost;
            GET_GOLD(shopkeep) -= cost;
            return (TRUE);
        }
        
        oldSendOutput(ch, "Alas, you don't seem to have the %s to sell.\n\r",
                  itemname);
        break;
    case 93:
    /*
     * offer 
     */
        arg = get_argument(arg, &itemname);
        if (!itemname) {
            oldSendOutput(ch, "What would you like to offer to %s?\n\r",
                      shopkeep->player.short_descr);
            return (TRUE);
        }
        
        if ((obj = get_obj_in_list_vis(ch, itemname, ch->carrying))) {
            cost = (int) obj->obj_flags.cost / (3 * modifier);
            if (cost < 400) {
                oldSendOutput(ch, "%s doesn't buy worthless junk like that.\n\r",
                          shopkeep->player.short_descr);
                return (TRUE);
            }
            
            oldSendOutput(ch, "%s is willing to pay you %d coins for %s.\n\r",
                      shopkeep->player.short_descr, cost,
                      obj->short_description);
            return (TRUE);
        }
        
        oldSendOutput(ch, "You don't seem to have any %ss.\n\r", itemname);
        break;
    default:
        return (FALSE);
    }
    return (TRUE);
}



int knockproc(struct char_data *ch, int cmd, char *arg,
              struct room_data *rp, int type)
{
    time_t          curr_time;
    struct tm      *t_info;

    Log("Knockproc");
    if (cmd != 429) {
        return (FALSE);
    }
    curr_time = time(0);
    t_info = localtime(&curr_time);

    curr_time = time(NULL);
#if 0
    oldSendOutput(ch,"Well.. look at the time.. its %d ",t_info->tm_hour);
#endif
    if (t_info->tm_hour != THE_HOUR && 
        IS_SET(rp->dir_option[0]->exit_info, EX_LOCKED)) {
        command_interpreter(ch, "zload 188");
        raw_unlock_door(ch, EXIT(ch, 0), 0);
        open_door(ch, 0);
        oldSendOutput(ch, "You knock on the big wooden door and then slowly, it "
                      "opens.\n\r");

        act("$n knocks on the big door and then suddenly, the big door opens "
            "up.", TRUE, ch, 0, 0, TO_ROOM);
        return (TRUE);
    }

    oldSendOutput(ch, "You knock on the big wooden door but nothing seems to "
                  "happen.\n\r");
    return (TRUE);
}


int troll_regen(struct char_data *ch)
{
    assert(ch);

    if (GET_HIT(ch) >= GET_MAX_HIT(ch)) {
        return (FALSE);
    }
    if (number(0, 2)) {
        return( FALSE );
    }
    GET_HIT(ch) += number(1, 3);
    if (GET_HIT(ch) > GET_MAX_HIT(ch)) {
        GET_HIT(ch) = GET_MAX_HIT(ch);
    }
    act("$n's wounds seem to close of their own.", FALSE, ch, 0, 0, TO_ROOM);
    act("Your wounds close of their own accord.", FALSE, ch, 0, 0, TO_CHAR);
    return( TRUE );
}

int disembark_ship(struct char_data *ch, int cmd, char *argument,
                   struct obj_data *obj, int type)
{
    int             x = 0;

    if (cmd == 621) {
        /* 
         * disembark 
         */

        if (oceanmap[GET_XCOORD(obj)][GET_YCOORD(obj)] == '@') {
            if (ch->specials.fighting) {
                send_to_char("You can't while your fighting!", ch);
                return (TRUE);
            }

            /*
             * lets find out where they are going 
             */
            while (map_coords[x].x != -1) {
                if (map_coords[x].x == GET_XCOORD(obj) &&
                    map_coords[x].x == GET_YCOORD(obj)) {
                    char_from_room(ch);
                    char_to_room(ch, map_coords[x].room);
                    do_look(ch, NULL, 0);
                    return (TRUE);
                }

                x++;
            }
            send_to_char("Ahh.. maybe we don't have a place to disembark!\n\r",
                         ch);
        } else {
            send_to_char("There is no place around to disembark your ship!\n\r",
                         ch);
        }
        return (TRUE);
    }
    return (FALSE);
}

int steer_ship(struct char_data *ch, int cmd, char *argument,
               struct obj_data *obj, int type)
{
    static char    *keywords[] = {
        "north",
        "east",
        "south",
        "west",
        "up",
        "down"
    };
    int             keyword_no = 0;
    char           *buf;

    argument = get_argument(argument, &buf);
    if (!buf) {
        /* 
         * No arguments?? so which direction anyway? 
         */
        send_to_char("Sail in which direction?", ch);
        return (TRUE);
    }

    keyword_no = search_block(buf, keywords, FALSE);

    if ((keyword_no == -1)) {
        send_to_char("Sail in which direction?", ch);
        return (TRUE);
    }

    if (!CanSail(obj, keyword_no)) {
        send_to_char("You can't sail that way, you'd be bound to sink the "
                     "ship!!\n\r", ch);
        return (TRUE);
    }

    oldSendOutput(ch, "You sail %sward.\n\r", keywords[keyword_no]);

    switch (keyword_no) {
    case 0:
        /* 
         * North
         */
        GET_XCOORD(obj)--;
        break;
    case 1:
        /*
         * East
         */
        GET_YCOORD(obj)++;
        break;
    case 2:
        /*
         * south
         */
        GET_XCOORD(obj)++;
        break;
    case 3:
        /* 
         * west
         */
        GET_YCOORD(obj)--;
        break;
    default:
        break;
    }

    printmap(ch, GET_XCOORD(obj), GET_YCOORD(obj), 5, 10);
    return (TRUE);
}


/*
 * can they sail in that directioN?? 
 */
int CanSail(struct obj_data *obj, int direction)
{
    int             x = 0,
                    y = 0;

    x = GET_XCOORD(obj);
    y = GET_YCOORD(obj);

    switch (direction) {
    case 0:
        /* 
         * North
         */
        x--;
        break;
    case 1:
        /* 
         * East
         */
        y++;
        break;
    case 2:
        /* 
         * south
         */
        x++;
        break;
    case 3:
        /* 
         * west
         */
        y--;
        break;
    default:
        break;
    }

    if (oceanmap[x][y] == '~') {
        return (TRUE);
    } else if (oceanmap[x][y] == '@') {
        /* 
         * Entered port city.. lets place a ship in the board city
         */
        return (TRUE);
    } else if (oceanmap[x][y] == ':') {
        return (TRUE);
    } else {
        return (FALSE);
    }
}

/*
 * lets print the map to the screen 
 */
void printmap(struct char_data *ch, int x, int y, int sizex, int sizey)
{
    int             loop = 0;
    void            printColors(struct char_data *ch, char *buf);
    char            buf[256];
    char            buf2[256];
#if 0
    printf("Displaying map at coord X%d-Y%d with display size of "
           "%d by %d.\n\r\n\r",x,y,sizex, sizey);
#endif
    oldSendOutput(ch, "Coords: %d-%d.\n\r", x, y);
    sprintf(buf, "\n\r$c000B]$c000W");

    for (loop = 0; loop < sizey * 2 + 1; loop++) {
        sprintf(buf, "%s=", buf);
    }

    sprintf(buf, "%s$c000B[$c000w\n\r", buf);
    send_to_char(buf, ch);

    for (loop = 0; loop < sizex * 2 + 1; loop++) {
        /* 
         * move that row of the ocean map into buf 
         */
        sprintf(buf, "%s", oceanmap[x - sizex + loop]);

        if (loop == sizex) {
            buf[y] = 'X';
        }
        /* 
         * mark of the end of where they should see 
         */
        buf[y + sizey + 1] = '\0';
        /* 
         * move to the start of where they should see on that row 
         * Print that mofo out 
         */
        sprintf(buf2, "|%s|\n\r", &buf[y - sizey]);
#if 0
        send_to_char(buf,ch);
        printmapcolors(buf);
#endif
        printColors(ch, buf2);
    }

    sprintf(buf, "$c000B]$c000W");
    for (loop = 0; loop < sizey * 2 + 1; loop++) {
        sprintf(buf, "%s=", buf);
    }

    sprintf(buf, "%s$c000B[$c000w", buf);
    send_to_char(buf, ch);
#if 0
    printColors(ch, buf);
#endif
}

/*
 * Lets go through and see what terrain needs what color 
 */
void printColors(struct char_data *ch, char *buf)
{
    int             x = 0;
#if 0
    int last=0;
#endif
    char            buffer[2048];
    char            last = ' ';

    buffer[0] = '\0';

    while (buf[x] != '\0') {
        switch (buf[x]) {
        case '~':
            if (last == '~') {
                strcat(buffer, "~");
            } else {
                strcat(buffer, "$c000b~");
                last = '~';
            }
            break;
        case '+':
            if (last == '+') {
                strcat(buffer, "+");
            } else {
                strcat(buffer, "$c000G+");
                last = '+';
            }
            break;
        case '^':
            if (last == '^') {
                strcat(buffer, "^");
            } else {
                strcat(buffer, "$c000Y^");
                last = '^';
            }
            break;
        case '.':
            if (last == '.') {
                strcat(buffer, ".");
            } else {
                strcat(buffer, "$c000y.");
                last = '.';
            }
            break;
        default:
            if (last == buf[x]) {
                sprintf(buffer, "%s%c", buffer, buf[x]);
            } else {
                sprintf(buffer, "%s$c000w%c", buffer, buf[x]);
                last = buf[x];
            }
            break;
        }
        x++;
    }

    send_to_char(buffer, ch);
}

int embark_ship(struct char_data *ch, int cmd, char *arg,
                struct char_data *mob, int type)
{
    int             j;
    char           *buf;
    struct char_data *ship;

    if (cmd != 620) {
        /* 
         * board ship 
         */
        return (FALSE);  
    }

    arg = get_argument(arg, &buf);
    if (buf && !strcasecmp("ship", buf) &&
        (ship = get_char_room("", ch->in_room))) {
        j = mob_index[ship->nr].virtual;

        send_to_char("You enter the ship.\n\r", ch);
        act("$n enters the ship.", FALSE, ch, 0, 0, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, j);

        act("Walks onto the ship.", FALSE, ch, 0, 0, TO_ROOM);
        do_look(ch, NULL, 0);
        return (TRUE);
    }

    return (FALSE);
}

/*
 * @Name:           skillfixer
 * @description:    A mob proc will fix the problem generated
 *                  by the lack of SKILL_KNOWN_?class? bit added
 *                  to the skill[].flags.  This proc will check
 *                  each player, and add the appropriate skill[].flags.
 *                  It will check each player on cmd then once in
 *                  a while it will go through the whole player list
 *                  and fix everybody to verify.
 * @Author:         Rick Peplinski (Talesian)
 * @Assigned to:    mob()
 */

int skillfixer(struct char_data *ch, int cmd, char *arg,
           struct char_data *mob, int type)
{
    int             i;
    int             j;
    int             k;
    int             m;
    int             n;
    char            buf[256];

    if (!ch->skills) {
        return( FALSE );
    }

    if (!(mob = get_char_room("nondescript man skill fixer", ch->in_room))) {
        Log("skill fixer proc is attached to a mob without proper name, "
            "in room %ld", ch->in_room);
        return (FALSE);
    }

    if (cmd != 531 || ch == mob ) {
        return( FALSE );
    }

    for (i = 0; i < MAX_SKILLS; i++) {
        if (!IS_SET(ch->skills[i].flags, SKILL_KNOWN)) {
            continue;
        }

        for (j = 0, m = 1; j < MAX_CLASS; j++, m <<= 1) {
            /*
             * Currently can only do a few classes because of size 
             * restrictions on char_skill_data 
             */
            if (!HasClass(ch, m) ||
                (m != CLASS_CLERIC && m != CLASS_MAGIC_USER &&
                 m != CLASS_SORCERER && m != CLASS_DRUID)) {
                continue;
            }

            for (k = 0; k < classes[j].skillCount; k++) {
                if (classes[j].skills[k].skillnum != i) {
                    continue;
                }

                if (m == CLASS_CLERIC && 
                    !IS_SET(ch->skills[i].flags, SKILL_KNOWN_CLERIC)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_CLERIC);
                } else if (m == CLASS_MAGIC_USER &&
                           !IS_SET(ch->skills[i].flags, SKILL_KNOWN_MAGE)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_MAGE);
                } else if (m == CLASS_SORCERER &&
                           !IS_SET(ch->skills[i].flags, SKILL_KNOWN_SORCERER)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_SORCERER);
                } else if (m == CLASS_DRUID &&
                           !IS_SET(ch->skills[i].flags, SKILL_KNOWN_DRUID)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_DRUID);
                }
            }

            for (n = 0; n < classes[j].mainskillCount; n++) {
                if (classes[j].mainskills[n].skillnum != i) {
                    continue;
                }

                if (m == CLASS_CLERIC && 
                    !IS_SET(ch->skills[i].flags, SKILL_KNOWN_CLERIC)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_CLERIC);
                } else if (m == CLASS_MAGIC_USER &&
                           IS_SET(ch->skills[i].flags, SKILL_KNOWN_MAGE)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_MAGE);
                } else if (m == CLASS_SORCERER &&
                           !IS_SET(ch->skills[i].flags, SKILL_KNOWN_SORCERER)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_SORCERER);
                } else if (m == CLASS_DRUID &&
                           !IS_SET(ch->skills[i].flags, SKILL_KNOWN_DRUID)) {
                    SET_BIT(ch->skills[i].flags, SKILL_KNOWN_DRUID);
                }
            }
        }
    }

    sprintf(buf, "tell %s Your skills have been patched up.", GET_NAME(ch));
    command_interpreter(mob, buf);
    return( TRUE );
}

/* procs for zone 39 */
int mazekeeper(struct char_data *ch, int cmd, char *arg,
               struct char_data *mob)
{
    char    buf[MAX_STRING_LENGTH];
    struct  obj_data *o;    
    int     objnum;
    struct  char_data *i;
    
    if (cmd != 17 || ch == mob || !arg || !ch || !mob) {
            return( FALSE );
    }

    objnum = real_object(6575);
    
    for( o = real_roomp(ch->in_room)->contents; o; o = o->next_content) {
        if( o->item_number == objnum ) {
            return( FALSE );
        }
    }

    if (!strcasecmp(arg, "yes")) {
        for (i = real_roomp(ch->in_room)->people; i;
             i = i->next_in_room) {
            if (IS_FOLLOWING(ch, i) && GetMaxLevel(i) >= 41) {
                strcpy( buf, "say Your group is far too powerfull to enter!");
                command_interpreter(mob, buf);
                return( TRUE );
            }
        }
        strcpy(buf, "say Wonderful!! Let us begin!");
        command_interpreter(mob, buf);
        act("$c000WThe mazekeeper utters strange words and traces arcane"
            " symbols in the air.$c000w", FALSE, mob, 0, 0, TO_ROOM);
        act("$c000WSuddenly a large portal opens!$c000w",
            FALSE, mob, 0, 0, TO_ROOM);
        o = read_object(6575, VIRTUAL);
        obj_to_room(o, ch->in_room);
        return(TRUE);
    } 
    
    if (!strcasecmp(arg, "no")) {
        strcpy(buf, "say Fine, if you don't want to play, you can die now!");
        command_interpreter(mob, buf);
        sprintf(buf, "kill %s", GET_NAME(ch));
        command_interpreter(mob, buf);
        return(TRUE);
    }
    return(FALSE);
}


int mazekeeper_riddle_master(struct char_data *ch, int cmd, char *arg,
                             struct char_data *mob)
{
    char    buf[MAX_STRING_LENGTH];
    char    *argument = NULL,
            *arg1,
            *arg2;
    struct  obj_data *o;
    int     ret = FALSE;
    int     objnum;
    
    if (!ch || !mob || mob == ch || cmd != 83 || !arg) {
        return(FALSE);
    }

    objnum = real_object(6580);

    for( o = real_roomp(ch->in_room)->contents; o; o = o->next_content) {
        if( o->item_number == objnum ) {
            return( FALSE );
        }
    }


    /* Keep the original arg for the caller in case we don't return TRUE */
    argument = strdup(arg);
    arg = argument;

    arg = get_argument(arg, &arg1);
    arg = get_argument(arg, &arg2);
    
    if (arg2 && !strcasecmp(arg2, "tomorrow")) {
        strcpy(buf, "say That is correct!");
        command_interpreter(mob, buf);
        strcpy(buf, "say I must say, I am impressed.");
        command_interpreter(mob, buf);
        strcpy(buf, "say It has been a very long time since the last "
                     "adventurer made it this far.");
        command_interpreter(mob, buf);
        strcpy(buf, "say Your reward is this armor crafted from special "
                     "ore found deep in our mines many years ago.");
        command_interpreter(mob, buf);
        strcpy(buf, "say Use it wisely, friend.  So long, and fare thee "
                     "well");
        command_interpreter(mob, buf);
        
        o = read_object(6593, VIRTUAL);
        obj_to_char(o, ch);
        gain_exp(ch, 150000);
        
        act("The riddle master waves his hand and a shimmering portal "
            "appears.", FALSE, mob, NULL, NULL, TO_ROOM);
        act("Somehow you know that your journey has come to an end, and"
            "the portal is the way home.", FALSE, mob, NULL, NULL, TO_ROOM);

        o = read_object(6580, VIRTUAL);
        obj_to_room(o, ch->in_room);
        ret = TRUE;
    }

    free(argument);
    return(ret);
}

    
int mazekeeper_riddle_common(struct char_data *ch, char *arg,
                             struct char_data *mob, struct riddle_answer *rid,
                             int ridCount, int exp, int portal)
{
    char    buf[MAX_STRING_LENGTH];
    char    *argument = NULL,
            *arg1,
            *arg2;
    struct  obj_data *o;
    int     ret = FALSE;
    int     i;
    
    /* Keep the original arg for the caller in case we don't return TRUE */
    argument = strdup(arg);
    arg = argument;

    arg = get_argument(arg, &arg1);
    arg = get_argument(arg, &arg2);
        
    if (!arg2) {
        free( argument );
        return( FALSE );
    }

    for( i = 0; i < ridCount && !ret; i++ ) {
        if( !strcasecmp(arg2, rid[i].answer ) ) {
            strcpy(buf, "say Excellent, you are correct!");
            command_interpreter(mob, buf);
            sprintf(buf, "say Take this %s as a reward.", rid[i].rewardText);
            command_interpreter(mob, buf);
            strcpy(buf, "say You may now move on to the next challenge, good "
                         "luck!");
            command_interpreter(mob, buf);
            
            o = read_object(rid[i].reward, VIRTUAL);
            obj_to_char(o, ch);
            gain_exp(ch, exp);
            sprintf(buf, "$c000BYou receive $c000W%d $c000Bexperience!$c000w", 
                    exp);
            act(buf, FALSE, ch, 0, 0, TO_CHAR);
            GET_GOLD(ch) = (GET_GOLD(ch) + exp);
            sprintf(buf, "$c000BYou receive $c000W%d $c000Bgold coins!$c000w", 
                    exp);
            act(buf, FALSE, ch, 0, 0, TO_CHAR);

            act("The riddler speaks in a strange language and traces an "
                "arcane symbol in the air.", FALSE, mob, 0, 0, TO_ROOM);
            act("A large portal opens in front of you!",
                FALSE, mob, 0, 0, TO_ROOM);

            o = read_object(portal, VIRTUAL);
            obj_to_room(o, ch->in_room);

            act("The riddler disapears in a puff of smoke!", 
                FALSE, mob, NULL, NULL, TO_ROOM);
            char_from_room(mob);
            extract_char(mob);
            ret = TRUE;
        }
    } 
    
    if( !ret ) {
        strcpy(buf, "say HAH! Wrong answer, now you will die!");
        command_interpreter(mob, buf);
        sprintf(buf, "kill %s", GET_NAME(ch));
        command_interpreter(mob, buf);
        ret = TRUE;
    }

    free( argument );
    return(ret);
}


int mazekeeper_riddle_one(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob)
{
    static struct riddle_answer rid[] = {
        { "doll",   6581, "earring" },
        { "needle", 6582, "earring" },
        { "storm",  6583, "earring" }
    };
    
    if (!ch || !mob || mob == ch || cmd != 83 || !arg) {
        return(FALSE);
    }
    
    return( mazekeeper_riddle_common(ch, arg, mob, rid, NELEMENTS(rid),
                                     10000, 6576) );
}

int mazekeeper_riddle_two(struct char_data *ch, int cmd, char *arg,
                          struct char_data *mob)
{
    static struct riddle_answer rid[] = {
        { "breath", 6584, "ring" },
        { "tongue", 6585, "ring" },
        { "temper", 6586, "ring" }
    };
    
    if (!ch || !mob || mob == ch || cmd != 83 || !arg) {
        return(FALSE);
    }

    return( mazekeeper_riddle_common(ch, arg, mob, rid, NELEMENTS(rid),
                                     25000, 6577) );
}
    
int mazekeeper_riddle_three(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob)
{
    static struct riddle_answer rid[] = {
        { "time",        6587, "necklace" },
        { "temperature", 6588, "necklace" },
        { "pressure",    6589, "necklace" }
    };
    
    if (!ch || !mob || mob == ch || cmd != 83 || !arg) {
        return(FALSE);
    }

    return( mazekeeper_riddle_common(ch, arg, mob, rid, NELEMENTS(rid),
                                     50000, 6578) );
}
 
int mazekeeper_riddle_four(struct char_data *ch, int cmd, char *arg,
                            struct char_data *mob)
{
    static struct riddle_answer rid[] = {
        { "star",  6590, "bracelet" },
        { "sleep", 6591, "bracelet" },
        { "dream", 6592, "bracelet" }
    };
    
    if (!ch || !mob || mob == ch || cmd != 83 || !arg) {
        return(FALSE);
    }

    return( mazekeeper_riddle_common(ch, arg, mob, rid, NELEMENTS(rid),
                                     100000, 6579) );
}


struct dragon_def dragonTable[] = {
    { RACE_DRAGON_RED,   
      { { "$c000RA cone of fire",  spell_fire_breath } }, 1 },
    { RACE_DRAGON_BLACK, 
      { { "$c000GA cone of acid",  spell_acid_breath } }, 1 }, 
    { RACE_DRAGON_GREEN, 
      { { "$c000gA cloud of gas",  spell_gas_breath } }, 1 }, 
    { RACE_DRAGON_WHITE, 
      { { "$c000CA cone of frost", spell_frost_breath } }, 1 }, 
    { RACE_DRAGON_BLUE,
      { { "$c000BA bolt of lightning", spell_lightning_breath } }, 1 }, 
    { RACE_DRAGON_SILVER, 
      { { "$c000CA cone of frost", spell_frost_breath }, 
        { "$c000YA golden ray", spell_paralyze_breath } }, 2 },
    { RACE_DRAGON_GOLD,  
      { { "$c000RA cone of fire", spell_fire_breath }, 
        { "$c000gA cloud of gas", spell_gas_breath } }, 2 },
    { RACE_DRAGON_BRONZE,
      { { "$c000BA bolt of lightning", spell_lightning_breath }, 
        { "$c000yA repulsive cloud", spell_repulsion_breath } }, 2 },
    { RACE_DRAGON_COPPER,
      { { "$c000PA sticky cloud", spell_slow_breath }, 
        { "$c000GA cone of acid", spell_acid_breath } }, 2 },
    { RACE_DRAGON_BRASS,
      { { "$c0008A gloomy cloud", spell_sleep_breath }, 
        { "$c000yA cloud of blistering sand", spell_desertheat_breath } }, 2 },
    { RACE_DRAGON_AMETHYST,
      { { "$c000pA faceted violet lozenge", spell_lozenge_breath } }, 1 },
    { RACE_DRAGON_CRYSTAL,
      { { "$c000YA cone of glowing shards", spell_shard_breath } }, 1 },
    { RACE_DRAGON_EMERALD,
      { { "$c000cA loud keening wail", spell_sound_breath } }, 1 },
    { RACE_DRAGON_SAPPHIRE,
      { { "$c000cA cone of high-pitched, almost inaudible sound", 
         spell_sound_breath } }, 1 },
    { RACE_DRAGON_TOPAZ,
      { { "$c000yA cone of dehydration", spell_dehydration_breath } }, 1 },
    { RACE_DRAGON_BROWN,
      { { "$c000yA cloud of blistering sand", spell_desertheat_breath } }, 1 },
    { RACE_DRAGON_CLOUD,
      { { "$c000CAn icy blast of air", spell_frost_breath } }, 1 },
    { RACE_DRAGON_DEEP,
      { { "$c000gA cone of flesh-corrosive gas", spell_gas_breath } }, 1 },
    { RACE_DRAGON_MERCURY,
      { { "$c000YA beam of bright, yellow light", spell_light_breath } }, 1 },
    { RACE_DRAGON_MIST,
      { { "$c000CA cloud of scalding vapor", spell_vapor_breath } }, 1 },
    { RACE_DRAGON_SHADOW,
      { { "$c0008A cloud of blackness", spell_dark_breath } }, 1 },
    { RACE_DRAGON_STEEL,
      { { "$c000gA cube of toxic gas", spell_gas_breath } }, 1 },
    { RACE_DRAGON_YELLOW,
      { { "$c000YA cloud of blistering sand", spell_desertheat_breath } }, 1 },
    { RACE_DRAGON_TURTLE,
      { { "$c000CA cloud of scalding steam", spell_vapor_breath } }, 1 }
};
int dragonTableCount = NELEMENTS(dragonTable);

int dragon(struct char_data *ch, int cmd, char *arg, 
           struct char_data *mob, int type)
{
   char             buf[MAX_STRING_LENGTH];
   struct   char_data   *tar_char;
   int              i,
                    j,
                    level;
    
    if (cmd || GET_POS(ch) < POSITION_FIGHTING) {
        return( FALSE );
    }

    if (!ch->specials.fighting || !number(0, 2) ) {
        return( FALSE );
    }

    for( i = 0; i < dragonTableCount; i++ ) {
        if( dragonTable[i].race == GET_RACE(ch) ) {
            break;
        }
    }

    if( i == dragonTableCount ) {
        /* Bad dragon, he don't exist! */
        Log( "Dragon %s has an undefined breath type (race %d), defaulting to "
            "fire", GET_NAME(ch), GET_RACE(ch) );
        /* First entry is Red Dragon, breathes fire */
        i = 0;
    }

    j = number(0, dragonTable[i].breathCount - 1);

    act("$c000W$n rears back and inhales!$c000w", FALSE, ch, 0, 0, TO_ROOM);

    sprintf(buf, "%s spews forth from $n's mouth!$c000w",
            dragonTable[i].breath[j].spews );
    act(buf, FALSE, ch, 0, 0, TO_ROOM);
    WAIT_STATE(ch, PULSE_VIOLENCE * 3);
    level = GetMaxLevel(ch);
    
    for (tar_char = real_roomp(ch->in_room)->people; tar_char;
         tar_char = tar_char->next_in_room) {
        if (!in_group(ch, tar_char) && !IS_IMMORTAL(tar_char)) {
            (dragonTable[i].breath[j].func)(level, ch, tar_char, 0);
        }
    }
    return(TRUE);
}
/*
 * vim:ts=4:sw=4:ai:et:si:sts=4
 */
