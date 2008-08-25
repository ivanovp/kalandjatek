/*
 * File:        creature.cc
 * Purpose:     CCreature class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-25 14:31:18 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstdio>  // snprintf
#include <assert.h>

#include "debug.h"
#include "item.h"
#include "creature.h"
#include "map.h"
#include "keyword.h"
#include "regex.h"
#include "split.h"
#include "trans.h" // uppercase
#include "inv.h"
#include "colors.h"
#include "app.h" // ABOUT
#include "dice.h"

/// Unit of weight
#define U_WEIGHT        "kg"
const std::string CCreature::CREATURE = "creature";

CThingList CCreature::global_creaturelist;
NullStream CCreature::nullstream;

#if (LANG == ENG)
const std::string CCreature::CMD_SAY         = "say";
const std::string CCreature::CMD_LOOK        = "look";
const std::string CCreature::CMD_INVENTORY   = "inventory";
const std::string CCreature::CMD_NORTH       = "n";
const std::string CCreature::CMD_SOUTH       = "s";
const std::string CCreature::CMD_EAST        = "e";
const std::string CCreature::CMD_WEST        = "w";
const std::string CCreature::CMD_NORTHEAST   = "ne";
const std::string CCreature::CMD_SOUTHEAST   = "se";
const std::string CCreature::CMD_NORTHWEST   = "nw";
const std::string CCreature::CMD_SOUTHWEST   = "sw";
const std::string CCreature::CMD_IN          = "in";
const std::string CCreature::CMD_OUT         = "out";
const std::string CCreature::CMD_UP          = "up";
const std::string CCreature::CMD_DOWN        = "down";
const std::string CCreature::CMD_PICKUP      = "pickup";
const std::string CCreature::CMD_DROP        = "drop";
const std::string CCreature::CMD_ALIAS       = "alias";
const std::string CCreature::CMD_BRINGOUT    = "bringout";
const std::string CCreature::CMD_PUTAWAY     = "putaway";
const std::string CCreature::CMD_POINTS      = "points";
const std::string CCreature::CMD_ATTACK      = "attack";
const std::string CCreature::CMD_ABOUT       = "about";
const std::string CCreature::CMD_HELP        = "help";
const std::string CCreature::CMD_HELP2       = "?";
const std::string CCreature::CMD_INFO        = "info";
// parameters
const std::string CCreature::CMD_SELF        = "myself";
const std::string CCreature::CMD_ALL         = "all";
const std::string CCreature::CMD_BRIEF       = "-brief";
const std::string CCreature::CMD_VERBOSE     = "-verbose";
#endif // (LANG == ENG)
#if (LANG == HUN)
const std::string CCreature::CMD_SAY         = "mond";
const std::string CCreature::CMD_LOOK        = "néz";
const std::string CCreature::CMD_INVENTORY   = "leltár";
const std::string CCreature::CMD_NORTH       = "é";
const std::string CCreature::CMD_SOUTH       = "d";
const std::string CCreature::CMD_EAST        = "k";
const std::string CCreature::CMD_WEST        = "ny";
const std::string CCreature::CMD_NORTHEAST   = "ék";
const std::string CCreature::CMD_SOUTHEAST   = "dk";
const std::string CCreature::CMD_NORTHWEST   = "ény";
const std::string CCreature::CMD_SOUTHWEST   = "dny";
const std::string CCreature::CMD_IN          = "be";
const std::string CCreature::CMD_OUT         = "ki";
const std::string CCreature::CMD_UP          = "fel";
const std::string CCreature::CMD_DOWN        = "le";
const std::string CCreature::CMD_PICKUP      = "felvesz";
const std::string CCreature::CMD_DROP        = "eldob";
const std::string CCreature::CMD_ALIAS       = "álnév";
const std::string CCreature::CMD_BRINGOUT    = "elõvesz";
const std::string CCreature::CMD_PUTAWAY     = "eltesz";
const std::string CCreature::CMD_POINTS      = "pontok";
const std::string CCreature::CMD_ATTACK      = "támad";
const std::string CCreature::CMD_ABOUT       = "névjegy";
const std::string CCreature::CMD_HELP        = "súgó";
const std::string CCreature::CMD_HELP2       = "?";
const std::string CCreature::CMD_INFO        = "info";
// parameters;
const std::string CCreature::CMD_SELF        = "magam";
const std::string CCreature::CMD_ALL         = "mindent";
const std::string CCreature::CMD_BRIEF       = "-rövid";
const std::string CCreature::CMD_VERBOSE     = "-teljes";
#endif // (LANG == HUN)

const std::string CCreature::K_SEX           = "iSex";
const std::string CCreature::K_RANDOMMOVE    = "iRandomMove";
// CCreature keywords
const std::string CCreature::K_WEAR          = "sWear";         // viselt dolgok
const std::string CCreature::K_RACE          = "sRace";         // faj (inkabb maskepp kellene megoldani)
const std::string CCreature::K_RANDOMSAY     = "sRandomSay";    // veletlenszeruen mondott szovegek
// wearing types
const std::string CCreature::K_LEFT_HAND     = "left hand";     // bal kez
const std::string CCreature::K_RIGHT_HAND    = "right hand";    // jobb kez
const std::string CCreature::K_TWO_HANDS     = "two hands";     // mindket kez

const std::string CCreature::K_ATTACKED      = "iAttacked";         // currently attacked (not for use in configuration files)
const std::string CCreature::K_DEAD          = "iDead";
const std::string CCreature::K_HP_REGEN_COUNT = "iHPRegenCount";    // not for use in configuration files
const std::string CCreature::K_MP_REGEN_COUNT = "iMPRegenCount";    // not for use in configuration files

void CCreature::init ()
{
    type = CREATURE;
    global_creaturelist.push_back (this);
    spectator = false;
    // Default otream works only under *nix systems.
    //ostream = new std::ofstream ("/dev/null");
    // ostream is necessary because of characters' random event: random say, move, etc.
    ostream = &nullstream;
    // setting default parser_map
    parser_map[CMD_SAY] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_say);
    parser_map[CMD_LOOK] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_look);
    parser_map[CMD_INVENTORY] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_inventory);
    parser_map[CMD_NORTH] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_SOUTH] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_EAST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_WEST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_NORTHEAST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_SOUTHEAST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_NORTHWEST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_SOUTHWEST] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_IN] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_OUT] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_UP] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_DOWN] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_move);
    parser_map[CMD_PICKUP] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_pickup);
    parser_map[CMD_DROP] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_drop);
    parser_map[CMD_ALIAS] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_alias);
    parser_map[CMD_BRINGOUT] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_bringout);
    parser_map[CMD_PUTAWAY] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_putaway);
    parser_map[CMD_POINTS] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_points);
    parser_map[CMD_ATTACK] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_attack);

    parser_map[CMD_ABOUT] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_about);
    parser_map[CMD_HELP] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_help);
    parser_map[CMD_HELP2] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_help);
    // FIXME 'info' command should be disabled by default. If iDebug = 1 in kf.cfg, can be registered...
    parser_map[CMD_INFO] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_info);
}

CCreature::CCreature () : CThing::CThing ()
{
    init (); 
}

CCreature::CCreature (const std::string& id, const std::string& name, const std::string& descr, CThing* parent) :
    CThing::CThing (id, name, descr, parent)
{
    init ();
}

CCreature::CCreature (CThing& thing) : CThing::CThing (thing)
{
    init ();
}

CCreature::CCreature (CCreature& thing) : CThing::CThing (thing)
{
    init ();
}

void CCreature::setup ()
{
    CThing::setup ();
    std::string wears_str = get_sparam (K_WEAR);
    if (!wears_str.empty ())
    {
        // split object for splitting string into pieces
        CSplit split;
        // for storing string pieces
        CStringVector sv;
        sv = split.split (K_LISTSEPARATOR, wears_str);
        if (!sv.empty ())
        {
            for (CStringVectorIt i = sv.begin (); i != sv.end (); i++)
            {
                CStringVector sv2;
                sv2 = split.split (K_LISTSEPARATOR2, *i);
                std::string type, id;
                CStringVectorIt j = sv2.begin ();
                type = *j;
                id = *(++j);
                CThing *th;
                // searching in childs
                th = find (id, childs);
                if (th)
                {
                    // Marking thing as weared, wielded, etc.
                    th->set_sparam (CItem::K_WEAREDON, type);
                }
                else
                {
                    // FIXME what should we do?
                    std::ostringstream os;
#if (LANG == ENG)
                    os << __INFO__ << "'" << id << "' not found! Identifier: " << get_id () << ".";
#endif
#if (LANG == HUN)
                    os << __INFO__ << "'" << id << "' nem létezik! Azonosító: " << get_id () << ".";
#endif
                    Log.warn (os.str ());
                }
            }
        }
    }
    set_iparam (K_HP, get_stat (S_MAX_HP));
    set_iparam (K_MP, get_stat (S_MAX_MP));
}

CCreature::~CCreature ()
{
    set_spectator (false);
    CThingListIt i = std::find (global_creaturelist.begin (), global_creaturelist.end (), this);
    if (i != global_creaturelist.end ())
    {
        global_creaturelist.remove (*i);
    }
    else
    {
        std::ostringstream os;
        os << __INFO__ << "Internal error. Creature not found in global_creaturelist (sn: " << get_sn () << ")!";
        Log.warn (os.str ());
    }
    // Deleting functors.
    if (!parser_map.empty ())
    {
        for (CFunctorMapIt i = parser_map.begin (); i != parser_map.end (); i++)
        {
            delete i->second;
        }
    }
}

void CCreature::set_ostream (std::ostream *ostream)
{
    this->ostream = ostream;
}

void CCreature::set_spectator (bool mode)
{
    this->spectator = mode;
    if (mode)
    {
        // ENABLE spectator mode
        CThingListIt i = std::find (global_spectator_thinglist.begin (), global_spectator_thinglist.end (), this);
        if (i != global_spectator_thinglist.end ())
        {
            std::ostringstream os;
            os << __INFO__ << "This thing was already added to global_spectator_thinglist (sn: " << get_sn () << ").";
            Log.debug (os.str ());
        }
        else
        {
            global_spectator_thinglist.push_back (this);
        }
    }
    else
    {
        // DISABLE spectator mode 
        CThingListIt i = std::find (global_spectator_thinglist.begin (), global_spectator_thinglist.end (), this);
        if (i != global_spectator_thinglist.end ())
        {
            std::ostringstream os;
            os << __INFO__ << "Erasing thing from global_spectator_thinglist (sn: " << get_sn () << ").";
            Log.debug (os.str ());
            global_spectator_thinglist.remove (this);
        }
    }
}

bool CCreature::get_spectator ()
{
    return spectator;
}

void CCreature::set_aliases (std::string aliases)
{
    CStringVector sv;
    CSplit split;

    if (!ostream)
    {
        std::ostringstream os;
        os << __INFO__ << "Output stream has not initialized yet!";
        Log.error (os.str ());
    }
    else
    {
        sv = split (K_LISTSEPARATOR, aliases);
        for (CStringVectorIt i = sv.begin (); i != sv.end (); i++)
        {
            CRegEx regex ("^(.*)" K_LISTSEPARATOR2 "(.*)$");
            if (regex.Matches (*i))
            {
                std::string alias = regex.GetMatch (*i, 1),
                    cmd = regex.GetMatch (*i, 2);
                cmd_alias (CMD_ALIAS, alias + " " + cmd);
            }
            else
            {
                std::ostringstream os;
                os << __INFO__ << "Invalid alias: " << *i;
                Log.error (os.str ());
            }
        }
    }
}

std::string CCreature::get_aliases ()
{
    std::string aliases;
    bool first = true;
    if (!alias_map.empty ())
    {
        for (CStringMapIt i = alias_map.begin (); i != alias_map.end (); i++)
        {
            if (!first)
            {
                aliases += K_LISTSEPARATOR;
            }
            first = false;
            aliases += i->first + K_LISTSEPARATOR2 + i->second;
        }
    }
    return aliases;
}

std::string CCreature::get_shape ()
{
    std::ostringstream os;
    os << "C";
    return os.str ();
}

int CCreature::get_color ()
{
    return 0;
}

void CCreature::combat ()
{
    if (get_iparam (K_ATTACKED))
    {
        CThing *th = find (get_iparam (K_ATTACKED), global_creaturelist);
        if (th)
        {
            CCreature *opponent = dynamic_cast<CCreature*> (th);
            if (!opponent)
            {
                std::ostringstream os;
                os << __INFO__ << "Internal error. Dynamic cast failed. The item is not a creature in the global_creaturelist.";
                Log.error (os.str ());
                return;
            }
            std::ostringstream os;
            int mode; // mode of get_name ()
#if (LANG == ENG)
            mode = M_ARTICLE | M_NORMAL;
#endif // (LANG == ENG)
#if (LANG == HUN)
            mode = M_ARTICLE | M_RAG_T;
#endif // (LANG == HUN)
            int attack = get_stat (S_ATTACK);
            int d = dice ("1-10");
            int defense = opponent->get_stat (S_DEFENSE);
            (*ostream) << std::left << std::setw (16) << get_name () << " Attack: " << std::setw (3) << attack << " + " << std::setw (2) << d 
                << " Defense: " << std::setw (3) << defense;
            os << std::left << std::setw (16) << get_name () << " Attack: " << std::setw (3) << attack << " + " << std::setw (2) << d 
                << " Defense: " << std::setw (3) << defense;
            if (attack + d > defense)
            {
                int hp = opponent->get_stat (S_CURR_HP);
                int damage = dice (get_damage ());
                // FIXME substract damage_resistance from damage!
                hp -= damage;
                opponent->set_iparam (K_HP, hp);
                (*ostream) << " Damage: " << std::setw (3) << damage << " New HP: " << std::setw (3) << hp << std::endl;
                os << " Damage: " << std::setw (3) << damage << " New HP: " << std::setw (3) << hp << std::endl;
                if (hp <= 0)
                {
#if (LANG == ENG)
                    (*ostream) << C_DO << "You killed " << opponent->get_name (mode) << "!" << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " has killed " << opponent->get_name (mode) << "!" << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
                    (*ostream) << C_DO << "Megölted " << opponent->get_name (mode) << "!" << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megölte " << opponent->get_name (mode) << "!" << C_RST << std::endl;
#endif // (LANG == HUN)
                    // stop attack
                    set_iparam (K_ATTACKED, 0);
                    opponent->set_iparam (K_ATTACKED, 0);
                    opponent->set_iparam (K_DEAD, 1); // the opponent has perished...
                    // save original name as altname
                    if (!opponent->get_sparam (K_ALTNAME).empty ())
                    {
                        opponent->set_sparam (K_ALTNAME, opponent->get_sparam (K_ALTNAME) + "/" + opponent->get_name ());
                    }
                    else
                    {
                        opponent->set_sparam (K_ALTNAME, opponent->get_name ());
                    }
                    // change name of the dead
#if (LANG == ENG)
                    opponent->set_name ("body of " + opponent->get_name ());
#endif // (LANG == ENG)
#if (LANG == HUN)
                    opponent->set_name (opponent->get_name () + " holtteste");
#endif // (LANG == HUN)
                    // put all possessions to the parent (all items will be dropped)
                    std::vector<CThing*> childs2;
                    childs2.reserve (opponent->childs.size ());
                    // we've got childs, copy the list
                    // after calling move_to() the list iterator will be faulty!!!
                    std::copy (opponent->childs.begin (), opponent->childs.end (), std::back_inserter (childs2));
                    for (unsigned int i = 0; i < childs2.size (); i++)
                    {
                        if (childs2[i]->get_type () == CItem::ITEM ||
                                childs2[i]->get_type () == CCreature::CREATURE)
                        {
                            childs2[i]->move_to (*(opponent->parent));
                        }
                    }
                }
                else
                {
#if (LANG == ENG)
                    (*ostream) << C_DO << "You have wounded " << opponent->get_name (mode) << "." << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " has wounded " << opponent->get_name (mode) << "." << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
                    (*ostream) << C_DO << "Megsebezted " << opponent->get_name (mode) << "." << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megsebesítette " << opponent->get_name (mode) << "." << C_RST << std::endl;
#endif // (LANG == HUN)
                }
                // the attacked opponent will attack ourselves!!!
                // FIXME check sHabit!!!
                opponent->set_iparam (K_ATTACKED, get_sn ());
            }
            else
            {
                (*ostream) << " No damage" << std::endl;
                os << " No damage" << std::endl;
            }
            write_to_spectators (os.str (), *this);
        }
        else
        {
            // XXX Az eloleny valoszinuleg elpusztult... nem! azt az iDead valtozoval kellene jelezni
            std::ostringstream os;
            os << __INFO__ << "Internal error. The opponent is not found in the global_creaturelist.";
            Log.error (os.str ());
        }
    }
}

void CCreature::regenerate ()
{
    const int hpRegenRate = 200;
    const int mpRegenRate = 200;
    if (!get_iparam (K_ATTACKED))
    {
        if (get_stat (S_CURR_HP) < get_stat (S_MAX_HP))
        {
            if (inc_iparam (K_HP_REGEN_COUNT, get_stat (S_BODY)) >= hpRegenRate)
            {
                dec_iparam (K_HP_REGEN_COUNT, hpRegenRate);
                inc_iparam (K_HP, 1);
                if (get_stat (S_CURR_HP) == get_stat (S_MAX_HP))
                {
#if (LANG == ENG)
                    (*ostream) << C_DO << "You are healhty." << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
                    (*ostream) << C_DO << "Meggyógyultál." << C_RST << std::endl;
#endif // (LANG == HUN)
                }
            }
        }
        if (get_stat (S_CURR_MP) < get_stat (S_MAX_MP))
        {
            if (inc_iparam (K_MP_REGEN_COUNT, get_stat (S_WILLPOWER)) >= mpRegenRate)
            {
                dec_iparam (K_MP_REGEN_COUNT, mpRegenRate);
                inc_iparam (K_MP, 1);
                if (get_stat (S_CURR_MP) == get_stat (S_MAX_MP))
                {
#if (LANG == ENG)
                    (*ostream) << C_DO << "You are refreshed." << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
                    (*ostream) << C_DO << "Felfrissültél." << C_RST << std::endl;
#endif // (LANG == HUN)
                }
            }
        }
    }
}

void CCreature::random_look ()
{
    // look something or someone
    CThingList tl;
    int rn = static_cast<int> (3.0 * rand () / (RAND_MAX + 1.0));
    switch (rn)
    {
        case 0:
            // look something in inventory
            tl = childs;
            break;
        case 1:
            // look something in map
            tl = parent->childs;
            tl.remove (this);
            break;
        case 2:
            // look around
            parser (CMD_LOOK);
            return;
    }
    CThing *th = NULL;
    if (!tl.empty ())
    {
        int rn = static_cast<int> (static_cast<float> (tl.size ()) * rand () / (RAND_MAX + 1.0));
        int j = 0;
        for (CThingListIt i = tl.begin (); i != tl.end (); i++, j++)
        {
            if (j == rn)
            {
                th = *i;
                break;
            }
        }
    }
    if (th)
    {
        parser (std::string (CMD_LOOK) + " " + th->get_name ());
    }
}

void CCreature::random_say ()
{
    std::string s = get_sparam (K_RANDOMSAY);
    if (!s.empty ())
    {
        CStringVector sv;
        CSplit split;
        sv = split (K_LISTSEPARATOR, s);
        int rn = static_cast<int> (static_cast<float> (sv.size ()) * rand () / (RAND_MAX + 1.0));
        int j = 0;
        for (CStringVectorIt i = sv.begin (); i != sv.end (); i++, j++)
        {
            if (j == rn)
            {
                parser (std::string (CMD_SAY) + " " + *i);
                break;
            }
        }
    }
}

void CCreature::random_move ()
{
    std::ostringstream os;
    if (doesRandomMove ())
    {
        CSplit split;
        if (parent == NULL)
        {
            std::ostringstream os;
            os << __INFO__ << "No parent (parent == NULL)!";
            Log.debug (os.str ());
            return;
        }
        CStringVector sv = split (K_LISTSEPARATOR, parent->get_sparam (CMap::K_EXITS));
        if (!sv.empty ())
        {
            int rn = static_cast<int> (static_cast<float> (sv.size ()) * rand () / (RAND_MAX + 1.0));
            int j = 0;
            for (CStringVectorIt i = sv.begin (); i != sv.end (); i++, j++)
            {
                if (j != rn) continue;
                CRegEx regex ("^(.*)" K_LISTSEPARATOR2 "(.*)$");
                if (regex.Matches (*i))
                {
                    std::string exit = regex.GetMatch (*i, 1),
                                id = regex.GetMatch (*i, 2);
                    CThing *thing = find (id, CMap::global_maplist);
                    if (thing != NULL)
                    {
                        std::ostringstream os;
                        parser (exit + " " + thing->get_name ());
                        break;
                    }
                    else
                    {
                        std::ostringstream os;
                        os << __INFO__ << "Invalid exit identifier: " << *i;
                        Log.warn (os.str ());
                    }
                }
                else
                {
                    std::ostringstream os;
                    os << __INFO__ << "Invalid exit: " << *i;
                    Log.warn (os.str ());
                }
            }
        }
    }
}

void CCreature::do_something ()
{
    if (isAlive ())
    {
        combat ();
        regenerate ();
        childs_do_something ();
        // If we are not a player do something funny
        if (!get_spectator () && !isInCombat ())
        {
            const float CHANCE = 100.0;
            // random number
            int rn = static_cast<int> (CHANCE * rand () / (RAND_MAX + 1.0));
            if (rn == 0)
            {
                std::ostringstream os;
                os.str ("");
                os << get_name () << " (sn: " << get_sn () << ") does a random event.";
                Log.debug (os.str ());
                int rn = static_cast<int> (3.0 * rand () / (RAND_MAX + 1.0));
                switch (rn)
                {
                    case 0:
                        random_look ();
                        break;
                    case 1:
                        random_say ();
                        break;
                    case 2:
                        random_move ();
                        break;
                    case 3:
                        //random_attack (); // TODO write random attack! take into account the habit
                        break;
                    default:
                        os.str ("");
                        os << __INFO__ << "Internal error! Unknown random event.";
                        Log.debug (os.str ());
                        break;
                }
            }
        }
    }
}

bool CCreature::parser (const std::string& text, signed long from_serial_number)
{
    std::ostringstream os;
    os << __INFO__ << __FUNCTION__ << " " << get_name () << ": [" << text << "]";
    Log.debug (os.str ());
    if (ostream == NULL)
    {
        os.str ("");
        os << __INFO__ << "Output stream not initialized (ostream == NULL)!";
        Log.error (os.str ());
        return false;
    }
    
    if (parent == NULL)
    {
        os.str ("");
        os << __INFO__ << "No parent (parent == NULL)!";
        Log.error (os.str ());
        return false;
    }
   
    std::string Text = text;
    CRegEx regex ("^(.*\\S+)\\s+$");
    if (regex.Matches (Text))
    {
        // Cutting out whitespaces at end of 'Text'
        Text = regex.GetMatch (Text, 1);
    }
    regex.Compile ("^\\s*(\\S+)\\s*(.*)\\s*$");
    if (regex.Matches (Text))
    {
        bool perm = false; // permission to give command
        std::string cmd, params;
        cmd = regex.GetMatch (Text, 1);
        params = regex.GetMatch (Text, 2);
        
        /*std::ostringstream os;
        os << __INFO__ << __FUNCTION__ << " cmd: [" << cmd << "] params: [" << params << "]";
        Log.debug (os.str ());*/
        
        // command given by owner
        if (from_serial_number == -1 || from_serial_number == get_sn ())
        {
            perm = true;
        }
        else
        {
            // \todo TODO implement permission checking!
#warning "TODO: implement permission checking!"
            std::ostringstream os;
            os << __INFO__ << "Permission checking not implemented.";
            Log.error (os.str ());
        }
        if (perm)
        {
            CFunctorMapIt i = parser_map.find (cmd);
            if (i != parser_map.end ())
            {
                CFunctor *functor = parser_map[cmd];
                (*functor) (cmd, params);
                return true;
            }
            else
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "Unknown command: " << C_CMD << Text << "." << C_RST << " See help: ?" << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Ismeretlen parancs: " << C_CMD << Text << "." << C_RST << " Nézd meg a súgót: ?" << std::endl;
#endif
                return false;
            }
        }
    }
#if (LANG == ENG)
    (*ostream) << C_ERR << "Invalid input: " << C_CMD << text << C_RST << std::endl;
#endif
#if (LANG == HUN)
    (*ostream) << C_ERR << "Érvénytelen parancs: " << C_CMD << text << C_RST << std::endl;
#endif
    return false;
}

void CCreature::write_to_spectators (const std::string& text, CThing& except_thing, bool only_this_place)
{
    // TODO rewrite write_to_spectators!
    CThingList except_thinglist;
    except_thinglist.push_back (&except_thing);
    write_to_spectators (text, except_thinglist, only_this_place);
}

void CCreature::write_to_spectators (const std::string& text, CThingList& except_thinglist, bool only_this_place)
{
    for (CThingListIt i = global_spectator_thinglist.begin (); i != global_spectator_thinglist.end (); i++)
    {
        bool ok = true;
        CThingListIt j = std::find (except_thinglist.begin (), except_thinglist.end (), *i);
        if (j != except_thinglist.end ())
        {
            ok = false;
        }
        if (ok && (!only_this_place || (only_this_place && (*i)->parent == parent)))
        {
            CCreature *creature;
            creature = dynamic_cast<CCreature*> (*i);
            if (creature)
            {
                assert (creature->ostream != NULL);
                *(creature->ostream) << text;
            }
            else
            {
                std::ostringstream os;
                os << __INFO__ << "Internal error. Dynamic cast failed. The item is not a creature in the global_spectator_thinglist.";
                Log.error (os.str ());
            }
        }
    }
}

void CCreature::cmd_say (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Say something.";
#endif
#if (LANG == HUN)
            (*ostream) << "Mondani valamit.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Say something." << std::endl;
            (*ostream) << "Syntax: " C_CMD << CMD_SAY << " <text>" C_RST << std::endl;
            (*ostream) << "Example: " C_CMD << CMD_SAY << " Hello!" C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Mondani valamit." << std::endl;
            (*ostream) << "Szintaktika: " C_CMD << CMD_SAY << " <szöveg>" C_RST << std::endl;
            (*ostream) << "Például: " C_CMD << CMD_SAY << " Helló!" C_RST << std::endl;
#endif
        }
        return;
    }
    // checking whitespaces
    regex.Compile ("^\\s*$");
    if (regex.Matches (params))
    {
        // there are only whitespaces in parameter string
#if (LANG == ENG)
        (*ostream) << C_ERR << "No parameter." << C_RST << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << C_ERR << "Nincs paraméter." << C_RST << std::endl;
#endif
    }
    else
    {
#if 0
        if (isDead ())
        {
#if (LANG == ENG)
            (*ostream) << C_DO << "You can't speak. You are dead." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_DO << "Nem tudsz beszélni. Meghaltál." << C_RST << std::endl;
#endif
            return;
        }
#endif
        std::ostringstream os;
#warning "FIXME: int < 5: nem erti amit mondanak"
        if (get_stat (S_INT) < 5)
        {
#if (LANG == ENG)
            (*ostream) << C_DO << "You can't speak. You make only unarticulated voices." << C_RST << std::endl;
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " makes unarticulated voices." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_DO << "Nem tudsz beszélni, csak artikulálatlan hangok jönnek belõled." << C_RST << std::endl;
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " artikulálatlan hangokat ad." << C_RST << std::endl;
#endif
        }
        else
        {
#if (LANG == ENG)
            (*ostream) << C_DO << "You say: " << C_RST << params << std::endl;
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " say: " << C_RST << params << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_DO << "Azt mondod: " << C_RST << params << std::endl;
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " azt mondja: " << C_RST << params << std::endl;
#endif
        }
        write_to_spectators (os.str (), *this);
    }
}

void CCreature::cmd_look (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Look around or look something.";
#endif
#if (LANG == HUN)
            (*ostream) << "Körülnézni vagy megnézni valamit.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Look around or look something." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_LOOK << " [something]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_LOOK << " sword" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Körülnézni vagy megnézni valamit." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_LOOK << " [valami]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_LOOK << " kardot" << C_RST << std::endl;
#endif
            (*ostream) << "         " << C_CMD << CMD_LOOK << " " << CMD_SELF << C_RST << std::endl;
        }
        return;
    }
    CThingList Childs = parent->childs;
    Childs.remove (this);
    regex.Compile ("^\\s*$");
    if (regex.Matches (params))
    {
        std::ostringstream os;
        // There are only whitespaces in parameter string
        // The creature looks around in this place.
#if (LANG == ENG)
        (*ostream) << C_DO << "You look around." << C_RST << std::endl;
        os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " looks around." << C_RST << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << C_DO << "Körülnézel." << C_RST << std::endl;
        os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " körülnéz." << C_RST << std::endl;
#endif
        (*ostream) << std::endl;
        (*ostream) << parent->get_name ();
        (*ostream) << std::endl << std::endl;
        (*ostream) << parent->get_descr ();
        (*ostream) << std::endl << std::endl;
        // print items and creatures of the area
        if (!Childs.empty ())
        {
            CInventoryMap inventory_map;
            for (CThingListIt i = Childs.begin (); i != Childs.end (); i++)
            {
                CThing *th = *i;
                std::string name;
                name = th->get_name ();
                if ((th->get_type () == CItem::ITEM && th->get_iparam (CItem::K_VISIBLE)) ||
                        th->get_type () == CCreature::CREATURE)
                {
                    if (inventory_map.find (name) == inventory_map.end ())
                    {
                        inventory_map[name].counter = 1;
                        inventory_map[name].noun = th->get_iparam (K_NOUN);
                        inventory_map[name].plural = th->get_sparam (K_PLURAL);
                    }
                    else
                    {
                        inventory_map[name].counter++;
                    }
                }
            }
            if (!inventory_map.empty ())
            {
#if (LANG == ENG)
                (*ostream) << "You see here ";
#endif
#if (LANG == HUN)
                (*ostream) << "Itt van ";
#endif
                CInventory inv (ostream);
                inv.inventory_map = inventory_map;
                inv.write ();
                (*ostream) << "." << std::endl;
            }
            else
            {
#if (LANG == ENG)
                (*ostream) << "You see nothing here." << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << "Nincs itt semmi." << std::endl;
#endif
            }
            (*ostream) << std::endl;
        }
        CSplit split;
        if (parent == NULL)
        {
            std::ostringstream os;
            os << __INFO__ << "No parent (parent == NULL)!";
            Log.debug (os.str ());
            return;
        }
        // print possible exits of the area
        CStringVector sv = split (K_LISTSEPARATOR, parent->get_sparam (CMap::K_EXITS));
        if (!sv.empty ())
        {
            //(*ostream) << std::endl;
            for (CStringVectorIt i = sv.begin (); i != sv.end (); i++)
            {
                CRegEx regex ("^(.*)" K_LISTSEPARATOR2 "(.*)$");
                if (regex.Matches (*i))
                {
                    std::string exit = regex.GetMatch (*i, 1),
                                id = regex.GetMatch (*i, 2);
                    (*ostream) << exit << ": ";
                    CThing *thing = find (id, CMap::global_maplist);
                    if (thing != NULL)
                    {
                        (*ostream) << thing->get_name () << std::endl;
                    }
                    else
                    {
                        std::ostringstream os;
                        os << __INFO__ << "Invalid exit identifier: " << *i;
                        Log.warn (os.str ());
                    }
                }
                else
                {
                    std::ostringstream os;
                    os << __INFO__ << "Invalid exit: " << *i;
                    Log.warn (os.str ());
                }
            }
        }
        write_to_spectators (os.str (), *this);
    }
    else
    {
        // the creature looks at something/someone
        CThing *thing = NULL;
        std::ostringstream os;
        CThingList except;
        except.push_back (this);
        if (params == CMD_SELF)
        {
            thing = this;
        }
        else
        {
            // look something in this place
            for (CThingListIt i = Childs.begin (); i != Childs.end (); i++)
            {
                if (((*i)->get_type () == CItem::ITEM || (*i)->get_type () == CCreature::CREATURE) && (*i)->compare_name (params))
                {
                    thing = *i;
                    break;
                }
            }
            // look something in inventory
            for (CThingListIt i = childs.begin (); i != childs.end (); i++)
            {
                if ((*i)->get_type () == CItem::ITEM && (*i)->compare_name (params))
                {
                    thing = *i;
                    break;
                }
            }
        }
#ifdef __DEBUG__
        CRegEx regex ("(\\d+)");
        if (regex.Matches (params))
        {
            long sn = atol (regex.GetMatch (params, 1).c_str ());
            for (CThingListIt i = global_thinglist.begin (); i != global_thinglist.end (); i++)
            {
                if ((*i)->get_sn () == sn)
                {
                    thing = *i;
                    break;
                }
            }
        }
#endif
        if (thing != NULL)
        {
            except.push_back (thing);
            if (thing == this)
            {
#if (LANG == ENG)
                (*ostream) << C_DO << "You look yourself." << C_RST << std::endl;
                (*ostream) << C_DO << "You are " << get_name () << "." << C_RST << std::endl;
                os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " looks self." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_DO << "Megnézed magad." << C_RST << std::endl;
                (*ostream) << C_DO << get_name () << " vagy." << C_RST << std::endl;
                os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megnézi magát." << C_RST << std::endl;
#endif
            }
            else
            {
#if (LANG == ENG)
                (*ostream) << C_DO << "You look " << thing->get_name () << "." << C_RST << std::endl;
                os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " looks " << thing->get_name (M_ARTICLE) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_DO << "Megnézed " << thing->get_name (M_ARTICLE | M_RAG_T) << "." << C_RST << std::endl;
                os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megnézi " << thing->get_name (M_ARTICLE | M_RAG_T) << "." << C_RST << std::endl;
#endif
                CCreature *creature;
                creature = dynamic_cast<CCreature*> (thing);
                if (creature != NULL && creature->get_spectator ())
                {
#if (LANG == ENG)
                    (*creature->ostream) << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " looks you." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                    (*creature->ostream) << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megnéz téged." << C_RST << std::endl;
#endif
                }
            }
            (*ostream) << std::endl;
            (*ostream) << thing->get_descr () << std::endl;

            CCreature *creature;
            creature = dynamic_cast<CCreature*> (thing);
            if (creature)
            {
                CStringMap types_map;
#if (LANG == ENG)
                types_map[K_LEFT_HAND]          = "wields %s in the left hand.";
                types_map[K_RIGHT_HAND]         = "wields %s in the right hand.";
                types_map[K_TWO_HANDS]          = "wields %s in the hands.";
                types_map[CItem::K_RING]        = "has %s on a finger.";
                types_map[CItem::K_BRACELET]    = "has %s on a wrist.";
                types_map[CItem::K_AMULET]      = "wears %s on the neck.";
                types_map[CItem::K_CLOAK]       = "wears %s.";
                types_map[CItem::K_PANTS]       = "wears %s.";
                types_map[CItem::K_FOOTWEAR]    = "wears %s on the feet.";
                types_map[CItem::K_GLOVE]       = "wears %s on the hands.";
                types_map[CItem::K_CAP]         = "wears %s on the head.";
                types_map[CItem::K_TORSO]       = "wears %s on the torso.";
                types_map[CItem::K_ARMGUARDS]   = "wears %s on the arm.";
                types_map[CItem::K_SHINGUARDS]  = "wears %s on the shin.";
#endif
#if (LANG == HUN)
                types_map[K_LEFT_HAND]          = "Egy %s fog a bal kezében.";
                types_map[K_RIGHT_HAND]         = "Egy %s fog a jobb kezében.";
                types_map[K_TWO_HANDS]          = "Egy %s fog a két kezében.";
                types_map[CItem::K_RING]        = "Egy %s visel az egyik ujján.";
                types_map[CItem::K_BRACELET]    = "Egy %s visel a csuklóján.";
                types_map[CItem::K_AMULET]      = "Egy %s visel a nyaka körül.";
                types_map[CItem::K_CLOAK]       = "Egy %s visel.";
                types_map[CItem::K_PANTS]       = "Egy %s visel.";
                types_map[CItem::K_FOOTWEAR]    = "Egy %s visel a lábán.";
                types_map[CItem::K_GLOVE]       = "Egy %s visel a kezén.";
                types_map[CItem::K_CAP]         = "Egy %s visel a fején.";
                types_map[CItem::K_TORSO]       = "Egy %s visel a törzsén.";
                types_map[CItem::K_ARMGUARDS]   = "Egy %s visel az alkarján";
                types_map[CItem::K_SHINGUARDS]  = "Egy %s visel a lábszárán.";
#endif
                if (!creature->childs.empty ())
                {
                    (*ostream) << std::endl;
                    CInventoryMap inventory_map;
                    for (CThingListIt i = creature->childs.begin (); i != creature->childs.end (); i++)
                    {
                        CThing *th = *i;
                        if (!th->get_sparam (CItem::K_WEAREDON).empty ())
                        {
                            char s[256];
                            const char *p;
                            if (types_map.find (th->get_sparam (CItem::K_WEAREDON)) != types_map.end ())
                            {
                                p = types_map[th->get_sparam (CItem::K_WEAREDON)].c_str ();
#if (LANG == ENG)
                                if (creature->isMale ())
                                {
                                    (*ostream) << "He ";
                                }
                                else
                                {
                                    (*ostream) << "She ";
                                }
                                snprintf (s, sizeof (s), p, th->get_name (M_IARTICLE).c_str ());
                                (*ostream) << s << std::endl;
#endif
#if (LANG == HUN)
                                snprintf (s, sizeof (s), p, th->get_name (M_RAG_T).c_str ());
                                (*ostream) << Upper (s) << std::endl;
#endif
                            }
                            else
                            {
                                std::ostringstream os;
                                os << __INFO__ << "Internal error. Object weared on invalid part." << std::endl;
                                Log.error (os.str ());
                            }
                        }
                    }
                }
            }

            write_to_spectators (os.str (), except);
            if (thing == this)
            {
                (*ostream) << std::endl;
                cmd_inventory (CMD_INVENTORY, "");
            }
#ifdef __DEBUG__
            (*ostream) << std::endl;
            (*ostream) << thing->info (4);
            (*ostream) << std::endl;
#endif
        }
        else
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "You can't look." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nem tudod megnézni." << C_RST << std::endl;
#endif
        }
    }
}

void CCreature::cmd_inventory (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    std::ostringstream os;
    CStringMap types_map;
#if (LANG == ENG)
    types_map[K_LEFT_HAND]          = "You wield %s in left hand.";
    types_map[K_RIGHT_HAND]         = "You wield %s in right hand.";
    types_map[K_TWO_HANDS]          = "You wield %s in hands.";
    types_map[CItem::K_RING]        = "You have %s on either finger.";
    types_map[CItem::K_BRACELET]    = "You have %s on either wrist.";
    types_map[CItem::K_AMULET]      = "You wear %s on neck.";
    types_map[CItem::K_CLOAK]       = "You wear %s.";
    types_map[CItem::K_PANTS]       = "You wear %s.";
    types_map[CItem::K_FOOTWEAR]    = "You wear %s on feet.";
    types_map[CItem::K_GLOVE]       = "You wear %s on hands.";
    types_map[CItem::K_CAP]         = "You wear %s on head.";
    types_map[CItem::K_TORSO]       = "You wear %s on torso.";
    types_map[CItem::K_ARMGUARDS]   = "You wear %s on arm.";
    types_map[CItem::K_SHINGUARDS]  = "You wear %s on shin.";
#endif
#if (LANG == HUN)
    types_map[K_LEFT_HAND]          = "Egy %s fogsz a bal kezedben.";
    types_map[K_RIGHT_HAND]         = "Egy %s fogsz a jobb kezedben.";
    types_map[K_TWO_HANDS]          = "Egy %s fogsz a két kezedben.";
    types_map[CItem::K_RING]        = "Egy %s viselsz az egyik ujjadon.";
    types_map[CItem::K_BRACELET]    = "Egy %s viselsz a csuklódon.";
    types_map[CItem::K_AMULET]      = "Egy %s viselsz a nyakad körül.";
    types_map[CItem::K_CLOAK]       = "Egy %s viselsz.";
    types_map[CItem::K_PANTS]       = "Egy %s viselsz.";
    types_map[CItem::K_FOOTWEAR]    = "Egy %s viselsz a lábadon.";
    types_map[CItem::K_GLOVE]       = "Egy %s viselsz a kezeden.";
    types_map[CItem::K_CAP]         = "Egy %s viselsz a fejeden.";
    types_map[CItem::K_TORSO]       = "Egy %s viselsz a törzseden.";
    types_map[CItem::K_ARMGUARDS]   = "Egy %s viselsz az alkarodon";
    types_map[CItem::K_SHINGUARDS]  = "Egy %s viselsz a sípcsontodon.";
#endif
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Draw up an inventory.";
#endif
#if (LANG == HUN)
            (*ostream) << "Leltározás.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Drawing up an inventory." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_INVENTORY << " [-l]" << C_RST << std::endl;
            (*ostream) << "Switches: " << std::endl;
            (*ostream) << " -l: make a table" << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_INVENTORY << " -l" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Leltározás." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_INVENTORY << " [-l]" << C_RST << std::endl;
            (*ostream) << "Kapcsolók: " << std::endl;
            (*ostream) << " -l: táblázatot készít" << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_INVENTORY << " -l" << C_RST << std::endl;
#endif
        }
        return;
    }
    bool list = false;
    if (!params.empty ())
    {
        if (params == "-l")
        {
            list = true;
        }
        else
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "Invalid parameter. See help: " << C_CMD <<  CMD_HELP << " " << CMD_INVENTORY << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
            (*ostream) << C_ERR << "Ismeretlen parametér. Nézd meg a súgót: " << C_CMD <<  CMD_HELP << " " << CMD_INVENTORY << C_RST << std::endl;
#endif // (LANG == HUN)
            return;
        }
    }
    os.str ("");
#if (LANG == ENG)
    (*ostream) << C_DO << "You draw up an inventory: " << C_RST << std::endl;
    os << C_DO << get_name (M_CAPITAL_FIRST) << " draws up an inventory." << C_RST << std::endl;
#endif
#if (LANG == HUN)
    (*ostream) << C_DO << "Leltározol: " << C_RST << std::endl;
    os << C_DO << get_name (M_CAPITAL_FIRST) << " leltározik." << C_RST << std::endl;
#endif
    write_to_spectators (os.str (), *this);
    if (!childs.empty ())
    {
        CInventoryMap inventory_map;
        float load = 0;
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            if (th->get_type () == CItem::ITEM 
                    || th->get_type () == CCreature::CREATURE) // dead body... 
            {
                std::string name = th->get_name ();
                load += th->get_fparam (CItem::K_WEIGHT);
                if (inventory_map.find (name) == inventory_map.end ())
                {
                    inventory_map[name].counter = 1;
                    inventory_map[name].noun = th->get_iparam (K_NOUN);
                    inventory_map[name].price = th->get_fparam (CItem::K_PRICE);
                    inventory_map[name].weight = th->get_fparam (CItem::K_WEIGHT);
                    inventory_map[name].plural = th->get_sparam (K_PLURAL);
                    inventory_map[name].wearedon = th->get_sparam (CItem::K_WEAREDON);
                }
                else
                {
                    inventory_map[name].counter++;
                    // az alabbi specialis eset
                    // ket egyforma targyat (rez gyuru) visel az illeto, az egyiket
                    // elteszi tehat a masodikat viseli -> nem jelenik meg a csillag
                    // a leltarban a nev utan, ha nem kezeljuk az alant lathato modon
                    if (inventory_map[name].wearedon.empty ())
                    {
                        inventory_map[name].wearedon = th->get_sparam (CItem::K_WEAREDON);
                    }
                }
                if (!list && !th->get_sparam (CItem::K_WEAREDON).empty ())
                {
                    char s[256];
                    const char *p;
                    if (types_map.find (th->get_sparam (CItem::K_WEAREDON)) != types_map.end ())
                    {
                        p = types_map[th->get_sparam (CItem::K_WEAREDON)].c_str ();
#if (LANG == ENG)
                        snprintf (s, sizeof (s), p, th->get_name (M_IARTICLE).c_str ());
#endif
#if (LANG == HUN)
                        snprintf (s, sizeof (s), p, th->get_name (M_RAG_T).c_str ());
#endif
                        (*ostream) << Upper (s) << std::endl;
                    }
                    else
                    {
                        std::ostringstream os;
                        os << __INFO__ << "Internal error. Object weared on invalid part." << std::endl;
                        Log.error (os.str ());
                    }
                }
            }
        }
        if (list) // -l
        {
            // creating a table of items
            CItem *th = new CItem;
            (*ostream) << C_HL;
            int w_name = 25;
#if (LANG == ENG)
            int w_weight = 6;
            int w_price = 5;
            (*ostream) << "  # " << std::left << std::setw (w_name) << "Name" << " " 
                << std::setw (w_weight) << "Weight" << " " 
                << std::setw (w_price) << "Price"
                << std::endl;
#endif
#if (LANG == HUN)
            int w_weight = 5;
            int w_price = 5;
            (*ostream) << "Db. " << std::left << std::setw (w_name) << "Név" << " " 
                << std::setw (w_weight) << "Tömeg" << " " 
                << std::setw (w_price) << "Érték"
                << std::endl;
#endif
            (*ostream) << C_RST;
            for (CInventoryMapIt i = inventory_map.begin (); i != inventory_map.end (); i++)
            {
                std::string name = i->first;
                if (!i->second.wearedon.empty ())
                {
                    name += "*";
                }
                int counter = i->second.counter;
                float weight = i->second.weight,
                      price = i->second.price;
     
                (*ostream) << std::right << std::setw (3) << counter << " "
                    << std::left << std::setw (w_name) << name << " " 
                    << std::setw (w_weight) << static_cast<float> (counter) * weight << " " 
                    << std::setw (w_price) << th->get_price_str (static_cast<float> (counter) * price) 
                    << std::endl;
            }
            delete th;
            (*ostream) << std::endl;
#if (LANG == ENG)
            (*ostream) << "*: one or more items are weared." << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "*: egy vagy több tárgyat viselsz." << std::endl;
#endif
        }
        else
        {
            // without -l, normal mode
#if (LANG == ENG)
            (*ostream) << "You have ";
#endif
#if (LANG == HUN)
            (*ostream) << "Van nálad ";
#endif
            CInventory inv (ostream);
            inv.inventory_map = inventory_map;
            inv.write ();
            (*ostream) << "." << std::endl;
        }
        (*ostream) << std::endl;
#if (LANG == ENG)
        (*ostream) << "Load: " << load << " " << U_WEIGHT << " (" << std::setprecision (4) << 100.0 * load / get_loadability () << "%)" << std::endl;
        (*ostream) << "Loadability: " << get_loadability () << " " << U_WEIGHT << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << "Teher: " << load << " " << U_WEIGHT << " (" << std::setprecision (4) << 100.0 * load / get_loadability () << "%)" << std::endl;
        (*ostream) << "Terhelhetõség: " << get_loadability () << " " << U_WEIGHT << std::endl;
#endif
    }
    else
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "You have nothing.";
#endif
#if (LANG == HUN)
        (*ostream) << C_ERR << "Nincs semmid.";
#endif
        (*ostream) << C_RST << std::endl;
    }
}

void CCreature::cmd_move (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    std::ostringstream os;
    os << __INFO__ << __FUNCTION__  << " "  << get_name () << " " << cmd << " " << params;
    Log.debug (os.str ());
    CStringMap exits_map;
#if (LANG == ENG)
    exits_map[CMD_NORTH]     = "to north";
    exits_map[CMD_SOUTH]     = "to south";
    exits_map[CMD_EAST]      = "to east";
    exits_map[CMD_WEST]      = "to west";
    exits_map[CMD_NORTHEAST] = "to northeast";
    exits_map[CMD_SOUTHEAST] = "to southeast";
    exits_map[CMD_NORTHWEST] = "to northwest";
    exits_map[CMD_SOUTHWEST] = "to southwest";
    exits_map[CMD_IN]        = "in";
    exits_map[CMD_OUT]       = "out";
    exits_map[CMD_UP]        = "up";
    exits_map[CMD_DOWN]      = "down";
#endif
#if (LANG == HUN)
    exits_map[CMD_NORTH]     = "észak felé ";
    exits_map[CMD_SOUTH]     = "dél felé ";
    exits_map[CMD_EAST]      = "kelet felé ";
    exits_map[CMD_WEST]      = "nyugat felé ";
    exits_map[CMD_NORTHEAST] = "északkelet felé ";
    exits_map[CMD_SOUTHEAST] = "délkelet felé ";
    exits_map[CMD_NORTHWEST] = "északnyugat felé ";
    exits_map[CMD_SOUTHWEST] = "délnyugat felé ";
    exits_map[CMD_IN]        = "be";
    exits_map[CMD_OUT]       = "ki";
    exits_map[CMD_UP]        = "fel";
    exits_map[CMD_DOWN]      = "le";
#endif
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Move to given direction.";
#endif
#if (LANG == HUN)
            (*ostream) << "Mozgás a megadott irányba.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Move to given direction." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << "<direction> [place]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD "sw mountain" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Mozgás a megadott irányba." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << "<irány> [hely]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << "be boltba" << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
            for (CStringMapIt i = exits_map.begin (); i != exits_map.end (); i++)
            {
                (*ostream) << std::setw (10) << std::left;
#if (LANG == ENG)
                (*ostream) << i->first << " Moving " << i->second << "." << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << i->first << " " << Upper (i->second) << "megy." << std::endl;
#endif
            }
            (*ostream) << std::endl;
        }
        return;
    }
    CMap *map = dynamic_cast<CMap*> (parent);
    if (map == NULL)
    {
        std::ostringstream os;
        os << __INFO__ << "Internal error. Dynamic cast failed. Parent is not map." << std::endl;
        Log.error (os.str ());
        return;
    }
    CSplit split;
    CStringVector sv = split (K_LISTSEPARATOR, map->get_sparam (CMap::K_EXITS));
    if (!sv.empty ())
    {
        bool ok = false;
        std::string cmd_exit = cmd,
                    cmd_name = params;
        for (CStringVectorIt i = sv.begin (); i != sv.end (); i++)
        {
            regex.Compile ("^(.*):(.*)$");
            if (regex.Matches (*i))
            {
                std::string exit = regex.GetMatch (*i, 1),
                            id = regex.GetMatch (*i, 2);
                if (cmd_exit == exit)
                {
                    CThing *thing = find (id, CMap::global_maplist);
                    if (thing != NULL)
                    {
                        if (cmd_name == "" || thing->compare_name (cmd_name))
                        {
                            os.str ("");
#if (LANG == ENG)
                            (*ostream) << C_DO << "You go " << exits_map[cmd_exit] << "." << C_RST << std::endl;
                            os << C_DO << get_name (M_CAPITAL_FIRST) << " go " << exits_map[cmd_exit] << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                            (*ostream) << C_DO << Upper (exits_map[cmd_exit]) << "mész." << C_RST << std::endl;
                            os << C_DO << get_name (M_CAPITAL_FIRST) << " " << exits_map[cmd_exit] << "megy." << C_RST << std::endl;
#endif
                            write_to_spectators (os.str (), *this);
                            
                            move_to (*thing);

                            os.str ("");
#if (LANG == ENG)
                            os << C_DO << get_name (M_IARTICLE | M_CAPITAL_FIRST) << " arrived." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                            os << C_DO << get_name (M_IARTICLE | M_CAPITAL_FIRST) << " érkezik." << C_RST << std::endl;
#endif
                            write_to_spectators (os.str (), *this);

                            cmd_look (CMD_LOOK, "");
                            ok = true;
                            break;
                        }
                    }
                    else
                    {
                        os.str ("");
                        os << __INFO__ << "Exit identifier not found: " << *i;
                        Log.warn (os.str ());
                    }
                }
            }
            else
            {
                os.str ("");
                os << __INFO__ << "Invalid exit: " << *i;
                Log.warn (os.str ());
            }
        }
        if (!ok)
        {
            os.str ("");
#if (LANG == ENG)
            (*ostream) << C_ERR << "You can't go to this direction." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nem tudsz abba az irányba menni." << C_RST << std::endl;
#endif
        }
    }
    else
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "You can't see exit." << C_RST << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << C_ERR << "Nem látsz kijáratot." << C_RST << std::endl;
#endif
    }
}

void CCreature::cmd_pickup (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    std::ostringstream os;
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Pick up one or more item.";
#endif
#if (LANG == HUN)
            (*ostream) << "Felvesz egy vagy több tárgyat.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Pick up one or more item." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_PICKUP << " [quantity] <item>[, [quantity] <item>]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_PICKUP << " 2 sword" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Felvesz egy vagy több tárgyat." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_PICKUP << " [darab] <tárgy>[, [darab] <tárgy>]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_PICKUP << " 2 kardot" << C_RST << std::endl;
#endif
            (*ostream) << "         " << C_CMD << CMD_PICKUP << " " << CMD_ALL << C_RST << std::endl;
            (*ostream) << std::endl;
        }
        return;
    }
    CThingList Childs = parent->childs;
    Childs.remove (this);
    if (!Childs.empty ())
    {
        CInventoryMap inventory_map;
        int mode; // mode of get_name ()
#if (LANG == ENG)
        mode = M_NORMAL;
#endif
#if (LANG == HUN)
        mode = M_RAG_T;
#endif
        CStringVector sv;
        CSplit split;
        sv = split ("\\s*,\\s*", params);
        for (unsigned int j = 0; j < sv.size (); j++)
        {
            bool found = false;
            /*os.str ("");
            os << "sv[" << j << "] = " << sv[j] << std::endl;
            Log.debug (os.str ());*/
            CRegEx regex ("^(\\d+)\\s+(.*)$");
            int qty = 1;
            std::string name = sv[j];
            if (regex.Matches (sv[j]))
            {
                qty = std::atoi (regex.GetMatch (sv[j], 1).c_str ());
                name = regex.GetMatch (sv[j], 2);
            }
            int counter = 0;
            float weight = 0;
            for (CThingListIt i = Childs.begin (); i != Childs.end (); i++)
            {
                CThing *th = *i;
                if (name == CMD_ALL || th->compare_name (name))
                {
                    found = true;
                    if ((th->get_iparam (CItem::K_MOVABLE) && th->get_type () == CItem::ITEM) 
                            || (th->get_iparam (CCreature::K_DEAD) && th->get_type () == CCreature::CREATURE))
                    {
                        if (get_load_weight () + th->get_fparam (CItem::K_WEIGHT) <= get_loadability ())
                        {
                            std::string name2 = th->get_name (mode);
                            if (inventory_map.find (name2) == inventory_map.end ())
                            {
                                inventory_map[name2].counter = 1;
                                inventory_map[name2].noun = th->get_iparam (K_NOUN);
                                inventory_map[name2].weight = th->get_fparam (CItem::K_WEIGHT);
                                inventory_map[name2].plural = th->get_sparam (K_PLURAL);
                            }
                            else
                            {
                                inventory_map[name2].counter++;
                            }
                            counter++;
                            weight += th->get_fparam (CItem::K_WEIGHT);
                            th->move_to (*this);
                            if (counter >= qty && name != CMD_ALL)
                            {
                                break;
                            }
                        }
                        else
                        {
#if (LANG == ENG)
                        (*ostream) << C_ERR << "You can't pick up " << th->get_name (M_ARTICLE) << ". It is too heavy." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                        (*ostream) << C_ERR << "Nem tudod felvenni " << th->get_name (M_ARTICLE + M_RAG_T) << ". Túl nehéz." << C_RST << std::endl;
#endif
                        }
                    }
                    else
                    {
#if (LANG == ENG)
                        (*ostream) << C_ERR << "You can't pick up " << th->get_name (M_ARTICLE) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                        (*ostream) << C_ERR << "Nem tudod felvenni " << th->get_name (M_ARTICLE + M_RAG_T) << "." << C_RST << std::endl;
#endif
                    }
                }
            }
            if (!found)
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "There isn't " << name << " item." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Itt nincs " << name << " tárgy." << C_RST << std::endl;
#endif
            }
        }
        if (!inventory_map.empty ())
        {
            os.str ("");
#if (LANG == ENG)
            (*ostream) << C_DO << "You pick up ";
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " picks up ";
#endif
#if (LANG == HUN)
            (*ostream) << C_DO << "Felveszel ";
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " felvesz ";
#endif
            CInventory inv;
            inv.inventory_map = inventory_map;
            std::string s = inv.write ();
            (*ostream) << s << "." << C_RST << std::endl;
            os << s << "." << C_RST << std::endl;
            write_to_spectators (os.str (), *this);
        }
    }
    else
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "There is nothing." << C_RST << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << C_ERR << "Itt nincs semmi." << C_RST << std::endl;
#endif
    }
}

void CCreature::cmd_drop (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    std::ostringstream os;
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Drop one or more item.";
#endif
#if (LANG == HUN)
            (*ostream) << "Eldob egy vagy több tárgyat.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Drop one or more item." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_DROP << " [quantity] <item>[, [quantity] <item>]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_DROP << " 2 sword" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Eldob egy vagy több tárgyat." << std::endl;
            (*ostream) << "Szintaktika: " << CMD_DROP << " [darab] <tárgy>[, [darab] <tárgy>]" << std::endl;
            (*ostream) << "Például: " << CMD_DROP << " 2 kardot" << std::endl;
#endif
            (*ostream) << "         " << C_CMD << CMD_DROP << " " << CMD_ALL << C_RST << std::endl;
            (*ostream) << std::endl;
        }
        return;
    }
    CThingList Childs = this->childs;
    if (!Childs.empty ())
    {
        CInventoryMap inventory_map;
        int mode; // mode of get_name ()
#if (LANG == ENG)
        mode = M_NORMAL;
#endif
#if (LANG == HUN)
        mode = M_RAG_T;
#endif
        CStringVector sv;
        CSplit split;
        sv = split ("\\s*,\\s*", params);
        for (unsigned int j = 0; j < sv.size (); j++)
        {
            bool found = false;
            CRegEx regex ("^(\\d+)\\s+(.*)$");
            int qty = 1;
            std::string name = sv[j];
            if (regex.Matches (sv[j]))
            {
                qty = std::atoi (regex.GetMatch (sv[j], 1).c_str ());
                name = regex.GetMatch (sv[j], 2);
            }
            int counter = 0;
            for (CThingListIt i = Childs.begin (); i != Childs.end (); i++)
            {
                CThing *th = *i;
                if (name == CMD_ALL || th->compare_name (name))
                {
                    if (th->get_iparam (CItem::K_MOVABLE) && th->get_type () == CItem::ITEM)
                    {
                        found = true;
                        if (inventory_map.find (th->get_name (mode)) == inventory_map.end ())
                        {
                            inventory_map[th->get_name (mode)].counter = 1;
                            inventory_map[th->get_name (mode)].noun = th->get_iparam (K_NOUN);
                            inventory_map[th->get_name (mode)].plural = th->get_sparam (K_PLURAL);
                        }
                        else
                        {
                            inventory_map[th->get_name (mode)].counter++;
                        }
                        counter++;
                        th->set_sparam (CItem::K_WEAREDON, "");
                        th->move_to (*parent);
                        if (counter >= qty && name != CMD_ALL)
                            break;
                    }
                    else
                    {
#if (LANG == ENG)
                        (*ostream) << C_ERR << "You can't drop " << th->get_name (M_ARTICLE) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                        (*ostream) << C_ERR << "Nem tudod eldobni " << th->get_name (M_ARTICLE + M_RAG_T) << "." << C_RST << std::endl;
#endif
                    }
                }
            }
            if (!found)
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "You haven't " << name << " item." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Nincs " << name << " nevû tárgyad." << C_RST << std::endl;
#endif
            }
        }
        if (!inventory_map.empty ())
        {
            os.str ("");
#if (LANG == ENG)
            (*ostream) << C_DO << "You drop ";
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " drop ";
#endif
#if (LANG == HUN)
            (*ostream) << C_DO << "Eldobsz ";
            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " eldob ";
#endif
            CInventory inv;
            inv.inventory_map = inventory_map;
            std::string s = inv.write ();
            (*ostream) << s << "." << C_RST << std::endl;
            os << s << "." << C_RST << std::endl;
            write_to_spectators (os.str (), *this);
        }
    }
    else
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "You have nothing." << C_RST << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << C_ERR << "Nincs semmid." << C_RST << std::endl;
#endif
    }
}

void CCreature::cmd_alias (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
            if (help_cmd == CMD_ALIAS)
            {
#if (LANG == ENG)
                (*ostream) << "Creating and deleting command aliases.";
#endif
#if (LANG == HUN)
                (*ostream) << "Parancsálnevek létrehozása és törlése.";
#endif
            }
            else
            {
#if (LANG == ENG)
                (*ostream) << "Command alias. ";
#endif
#if (LANG == HUN)
                (*ostream) << "Parancsálnév. ";
#endif
                (*ostream) << C_HL << help_cmd << C_RST << " = " << C_CMD << alias_map[help_cmd] << C_RST;
            }
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Creating and deleting command aliases." << std::endl;
            (*ostream) << "Syntax: " << std::endl;
            (*ostream) << "Create alias: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << " <alias_name> <command>" << C_RST << std::endl;
            (*ostream) << "Delete alias: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << " <alias_name>" << C_RST << std::endl;
            (*ostream) << "List aliases: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_ALIAS << " i inventory -l" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Parancsálnevek létrehozása és törlése." << std::endl;
            (*ostream) << "Szintaktika: " << std::endl;
            (*ostream) << "Létrehozás: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << " <álnév> <parancs>" << C_RST << std::endl;
            (*ostream) << "Törlés: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << " <álnév>" << C_RST << std::endl;
            (*ostream) << "Listázás: " << std::endl;
            (*ostream) << C_CMD << CMD_ALIAS << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_ALIAS << " l leltár -l" << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }
    regex.Compile ("^(\\S+)\\s*(.*)$");
    if (cmd == CMD_ALIAS)
    {
        if (params.empty ())
        {
            // list aliases
#if (LANG == ENG)
            (*ostream) << "Aliases:" << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Álnevek:" << std::endl;
#endif
            if (!alias_map.empty ())
            {
                for (CStringMapIt i = alias_map.begin (); i != alias_map.end (); i++)
                {
                    (*ostream) << C_HL << i->first << C_RST << " = " << C_CMD << i->second << C_RST << std::endl;
                }
            }
            else
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "No aliases." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Nincsenek álnevek." << C_RST << std::endl;
#endif
            }
        }
        else if (regex.Matches (params))
        {
            std::string alias_name = regex.GetMatch (params, 1),
                        alias_cmd = regex.GetMatch (params, 2);
            // finding illegal characters in parameter string
            if ((int) params.find (K_LISTSEPARATOR) != -1 || (int) params.find (K_LISTSEPARATOR2) != -1 ||
                    (int) params.find ('\"') != -1)
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "Illegal character in alias (" << K_LISTSEPARATOR <<
                    K_LISTSEPARATOR2 << "\")!" << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Érvénytelen karakter az álnévben (" << K_LISTSEPARATOR <<
                    K_LISTSEPARATOR2 << "\")!" << C_RST << std::endl;
#endif
                return;
            }
            if (!alias_cmd.empty ())
            {
                // create/overwrite alias
                if (parser_map.find (alias_name) == parser_map.end () ||
                        alias_map.find (alias_name) != alias_map.end ())
                {
#if (LANG == ENG)
                    (*ostream) << C_DO << "Creating alias: " << C_HL << alias_name << C_RST << " = " << C_CMD << alias_cmd << C_RST << "." << std::endl;
#endif
#if (LANG == HUN)
                    (*ostream) << C_DO << "Álnév létrehozása: " << C_HL << alias_name << C_RST << " = " << C_CMD << alias_cmd << C_RST << "." << std::endl;
#endif
                    // If the functor not exists then we create one...
                    if (!parser_map[alias_name])
                    {
                        parser_map[alias_name] = new CCmdFunctor<CCreature> (this, &CCreature::cmd_alias);
                    }
                    alias_map[alias_name] = alias_cmd;
                }
                else
                {
#if (LANG == ENG)
                    (*ostream) << C_ERR << "There is already command '" << alias_name << "'." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                    (*ostream) << C_ERR << "Már van ,," << alias_name << "'' parancs." << C_RST << std::endl;
#endif
                }
            }
            else
            {
                // delete alias
                if (alias_map.find (alias_name) != alias_map.end ())
                {
                    alias_map.erase (alias_name);
                    parser_map.erase (alias_name);
#if (LANG == ENG)
                    (*ostream) << C_DO << "Alias '" << alias_name << "' has deleted." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                    (*ostream) << C_DO << ",," << alias_name << "'' álnév törölve." << C_RST << std::endl;
#endif
                }
                else
                {
#if (LANG == ENG)
                    (*ostream) << C_ERR << "Alias '" << alias_name << "' doesn't exists." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                    (*ostream) << C_ERR << "Nem létezik ,," << alias_name << "'' álnév." << C_RST << std::endl;
#endif
                }
            }
        }
        else
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "Invalid alias parameter. See help: " << C_CMD <<  CMD_HELP << " " << CMD_ALIAS << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Érvénytelen álnév paraméter. Nézd meg a súgót: " << C_CMD << CMD_HELP << " " << CMD_ALIAS << C_RST << std::endl;
#endif
        }
    }
    else
    {
        // processing alias
        std::string text = alias_map[cmd];
        if (!params.empty ())
        {
            text += " " + params;
        }
        parser (text);
    }
}

void CCreature::cmd_bringout (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    CStringMap types_map, types_map2;
#if (LANG == ENG)
    types_map[CItem::K_ONE_HANDED]     = "You have no more free hand.";
    types_map[CItem::K_TWO_HANDED]     = "You have already %s in hands.";
    types_map[CItem::K_RING]           = "You have already %s on finger.";
    types_map[CItem::K_BRACELET]       = "You wear already %s on wrist.";
    types_map[CItem::K_AMULET]         = "You wear already %s on neck.";
    types_map[CItem::K_CLOAK]          = "You wear already %s.";
    types_map[CItem::K_PANTS]          = "You wear already %s.";
    types_map[CItem::K_FOOTWEAR]       = "You wear already %s on feet.";
    types_map[CItem::K_GLOVE]          = "You wear already %s on hands.";
    types_map[CItem::K_CAP]            = "You wear already %s on head.";
    types_map[CItem::K_TORSO]          = "You wear already %s.";
    types_map[CItem::K_ARMGUARDS]      = "You wear already %s on arms.";
    types_map[CItem::K_SHINGUARDS]     = "You wear already %s on shins.";
    
    types_map2[K_LEFT_HAND]            = "You bring out %s and wield in left hand.";
    types_map2[K_RIGHT_HAND]           = "You bring out %s and wield in rightt hand.";
    types_map2[K_TWO_HANDS]            = "You bring out %s and wield in both hands.";
    types_map2[CItem::K_RING]          = "You bring out %s and put on one of fingers.";
    types_map2[CItem::K_BRACELET]      = "You bring out %s and put on one of wrist.";
    types_map2[CItem::K_AMULET]        = "You bring out %s and put on neck.";
    types_map2[CItem::K_CLOAK]         = "You bring out %s and wear.";
    types_map2[CItem::K_PANTS]         = "You bring out %s and wear.";
    types_map2[CItem::K_FOOTWEAR]      = "You bring out %s and wear on feet.";
    types_map2[CItem::K_GLOVE]         = "You bring out %s and wear on hands.";
    types_map2[CItem::K_CAP]           = "You bring out %s and wear on head.";
    types_map2[CItem::K_TORSO]         = "You bring out %s and wear.";
    types_map2[CItem::K_ARMGUARDS]     = "You bring out %s and wear on arms.";
    types_map2[CItem::K_SHINGUARDS]    = "You bring out %s and wear on shins.";
#endif
#if (LANG == HUN)
    types_map[CItem::K_ONE_HANDED]     = "Már nincs szabad kezed.";
    types_map[CItem::K_TWO_HANDED]     = "Már van egy %s a kezedben.";
    types_map[CItem::K_RING]           = "Már van egy %s az egyik ujjadon.";
    types_map[CItem::K_BRACELET]       = "Már van egy %s a csuklódon.";
    types_map[CItem::K_AMULET]         = "Már van egy %s a nyakad körül.";
    types_map[CItem::K_CLOAK]          = "Már van egy %s rajtad.";
    types_map[CItem::K_PANTS]          = "Már van egy %s rajtad.";
    types_map[CItem::K_FOOTWEAR]       = "Már van egy %s a lábadon.";
    types_map[CItem::K_GLOVE]          = "Már van egy %s a kezeden.";
    types_map[CItem::K_CAP]            = "Már van egy %s a fejeden.";
    types_map[CItem::K_TORSO]          = "Már van egy %s a törzseden.";
    types_map[CItem::K_ARMGUARDS]      = "Már van egy %s az alkarodon.";
    types_map[CItem::K_SHINGUARDS]     = "Már van egy %s a lábszáradon.";

    types_map2[K_LEFT_HAND]            = "Elõveszed %s és a bal kezedben megfogod.";
    types_map2[K_RIGHT_HAND]           = "Elõveszed %s és a jobb kezedben megfogod.";
    types_map2[K_TWO_HANDS]            = "Elõveszed %s és a kezedben megfogod.";
    types_map2[CItem::K_RING]          = "Elõveszed %s és az egyik ujjadra húzod.";
    types_map2[CItem::K_BRACELET]      = "Elõveszed %s és a csuklódra teszed.";
    types_map2[CItem::K_AMULET]        = "Elõveszed %s és felteszed a nyakadra.";
    types_map2[CItem::K_CLOAK]         = "Elõveszed %s és magadra teríted.";
    types_map2[CItem::K_PANTS]         = "Elõveszed %s és felveszed.";
    types_map2[CItem::K_FOOTWEAR]      = "Elõveszed %s és a lábadra húzod.";
    types_map2[CItem::K_GLOVE]         = "Elõveszed %s és a kezedre húzod.";
    types_map2[CItem::K_CAP]           = "Elõveszed %s és a fejedre teszed.";
    types_map2[CItem::K_TORSO]         = "Elõveszed %s és felveszed.";
    types_map2[CItem::K_ARMGUARDS]     = "Elõveszed %s és az alkarodra húzod";
    types_map2[CItem::K_SHINGUARDS]    = "Elõveszed %s és a lábszáradra húzod.";
#endif
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Wear armours and clothes. Put on jewellery. Wield items.";
#endif
#if (LANG == HUN)
            (*ostream) << "Páncélok, ruhák, ékszerek viselése, fegyverek kézbevétele.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Wear armours and clothes. Put on jewellery. Wield items." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_BRINGOUT << " <item>[, <item>]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_BRINGOUT << " cloak" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Páncélok, ruhák, ékszerek viselése, fegyverek kézbevétele." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_BRINGOUT << " <tárgy>[, <tárgy>]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_BRINGOUT << " köpeny" << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }
    CStringVector sv;
    CSplit split;
    sv = split ("\\s*,\\s*", params);
    for (unsigned int j = 0; j < sv.size (); j++)
    {
        std::string name = sv[j];
        std::string name_found;
        bool found = false;
        bool ok = false;
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            bool left_hand = false,
                 right_hand = false;
            char ring = 0;
            char bracelet = 0;
            if (th->compare_name (name) && 
                    th->get_sparam (CItem::K_WEAREDON).empty ()) // not weared
            {
                found = true;
#if (LANG == ENG)
                name_found = th->get_name (M_ARTICLE);
#endif
#if (LANG == HUN)
                name_found = th->get_name (M_ARTICLE | M_RAG_T);
#endif
                std::string type = th->get_sparam (CItem::K_TYPE);
                if (!type.empty () && 
                        (type == CItem::K_RING || type == CItem::K_BRACELET || type == CItem::K_AMULET ||
                        type == CItem::K_CLOAK || type == CItem::K_PANTS || type == CItem::K_FOOTWEAR ||
                        type == CItem::K_GLOVE || type == CItem::K_CAP || type == CItem::K_TORSO ||
                        type == CItem::K_ARMGUARDS || type == CItem::K_SHINGUARDS || 
                        type == CItem::K_ONE_HANDED || type == CItem::K_TWO_HANDED))
                {
                    ok = true;
                    for (CThingListIt j = childs.begin (); j != childs.end (); j++)
                    {
                        CThing *th2 = *j;
                        std::string wearedon = th2->get_sparam (CItem::K_WEAREDON);
                        if (wearedon == CItem::K_RING) ring++;
                        if (wearedon == CItem::K_BRACELET) bracelet++;
                        if (wearedon == K_LEFT_HAND) left_hand = true;
                        if (wearedon == K_RIGHT_HAND) right_hand = true;
                        if (wearedon == type && ((type == CItem::K_RING && ring > 1) ||
                                    ((type == CItem::K_BRACELET && bracelet > 1))) ||
                                (type == CItem::K_ONE_HANDED && left_hand && right_hand) ||
                                (type == CItem::K_TWO_HANDED && (left_hand || right_hand)) ||
                                (type == CItem::K_TWO_HANDED && wearedon == K_TWO_HANDS))
                        {
                            // Mar visel egy targyat
                            // You have already wear an item
                            char s[256];
                            snprintf (s, sizeof (s), types_map[type].c_str (), th2->get_name ().c_str ());
                            (*ostream) << s << std::endl;
                            ok = false;
                            if (type != CItem::K_BRACELET && type != CItem::K_RING)
                            {
                                break;
                            }
                        }
                    }
                    if (ok)
                    {
                        if (type == CItem::K_ONE_HANDED)
                        {
                            if (!right_hand)
                            {
                                th->set_sparam (CItem::K_WEAREDON, K_RIGHT_HAND);
                            }
                            else if (!left_hand)
                            {
                                th->set_sparam (CItem::K_WEAREDON, K_LEFT_HAND);
                            }
                            else
                            {
                                std::ostringstream os;
                                os << __INFO__ << "Internal error. You have no more hands.";
                                Log.debug (os.str ());
                            }
                        }
                        else if (type == CItem::K_TWO_HANDED)
                        {
                            th->set_sparam (CItem::K_WEAREDON, K_TWO_HANDS);
                        }
                        else
                        {
                            th->set_sparam (CItem::K_WEAREDON, type);
                        }
                        std::ostringstream os;
                        char s[256];
#if (LANG == ENG)
                        snprintf (s, sizeof (s), types_map2[th->get_sparam (CItem::K_WEAREDON)].c_str (), th->get_name ().c_str ());
                        os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " bring out " << th->get_name (M_ARTICLE) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                        snprintf (s, sizeof (s), types_map2[th->get_sparam (CItem::K_WEAREDON)].c_str (), th->get_name (M_ARTICLE | M_RAG_T).c_str ());
                        os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " elõveszi " << th->get_name (M_ARTICLE | M_RAG_T) << "." << C_RST << std::endl;
#endif
                        (*ostream) << C_DO << s << C_RST << std::endl;
    
                        CThingList except;
                        except.push_back (this);
                        write_to_spectators (os.str (), except, true);
                    }
                    break;
                }
            }
        }
        if (found && !ok)
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "You cannot wear " << name_found << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nem tudod viselni " << name_found << "." << C_RST << std::endl;
#endif
        }
        if (!found)
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "You haven't " << name << " item." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nincs " << name << " nevû tárgyad." << C_RST << std::endl;
#endif
        }
    }
}

void CCreature::cmd_putaway (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Take off armours and clothes. Remove jewellery. Take away weapons.";
#endif
#if (LANG == HUN)
            (*ostream) << "Viselt páncélokat, ruhákat, ékszereket leveszi, fegyvereket elteszi.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Take off armours and clothes. Remove jewellery. Take away weapons." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_PUTAWAY << " <item>[, <item>]" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_PUTAWAY << " cloak" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Viselt páncélokat, ruhákat, ékszereket leveszi, fegyvereket elteszi." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_PUTAWAY << " <tárgy>[, <tárgy>]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_PUTAWAY << " köpeny" << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }
    CStringVector sv;
    CSplit split;
    sv = split ("\\s*,\\s*", params);
    for (unsigned int j = 0; j < sv.size (); j++)
    {
        std::string name = sv[j];
        bool found = false;
        bool ok = false;
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            if (th->compare_name (name))
            {
                found = true;
                std::string type = th->get_sparam (CItem::K_TYPE);
                if (!type.empty () && !th->get_sparam (CItem::K_WEAREDON).empty () &&
                        (type == CItem::K_RING || type == CItem::K_BRACELET || type == CItem::K_AMULET ||
                        type == CItem::K_CLOAK || type == CItem::K_PANTS || type == CItem::K_FOOTWEAR ||
                        type == CItem::K_GLOVE || type == CItem::K_CAP || type == CItem::K_TORSO ||
                        type == CItem::K_ARMGUARDS || type == CItem::K_SHINGUARDS || 
                        type == CItem::K_ONE_HANDED || type == CItem::K_TWO_HANDED))
                {
                    th->set_sparam (CItem::K_WEAREDON, "");
                    std::ostringstream os;
#if (LANG == ENG)
                    (*ostream) << C_DO << "You take away " << th->get_name (M_ARTICLE) << "." << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " takes away " << th->get_name (M_ARTICLE) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                    (*ostream) << C_DO << "Elteszed " << th->get_name (M_ARTICLE | M_RAG_T) << "." << C_RST << std::endl;
                    os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " elteszi " << th->get_name (M_ARTICLE | M_RAG_T) << "." << C_RST << std::endl;
#endif
                    CThingList except;
                    except.push_back (this);
                    write_to_spectators (os.str (), except, true);
                    ok = true;
                    break;
                }
            }
        }
        if (!found)
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "You haven't " << name << " item." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nincs " << name << " nevû tárgyad." << C_RST << std::endl;
#endif
        }
        if (found && !ok)
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "You can't take away this item." << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Nem tudod eltenni a tárgyat." << C_RST << std::endl;
#endif
        }
    }
}

void CCreature::cmd_points (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Show statistics: strength, hit points, etc.";
#endif
#if (LANG == HUN)
            (*ostream) << "Kiírja az erõt, életerõ pontokat, stb.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Show statistics: strength, hit points, etc." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_POINTS << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_POINTS << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Kiírja az erõt, életerõ pontokat, stb." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_POINTS << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_POINTS << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }

#if (LANG == ENG)
    (*ostream) << "You are " << get_name () << "." << std::endl;
#endif
#if (LANG == HUN)
    (*ostream) << get_name (M_CAPITAL_FIRST) << " vagy." << std::endl;
#endif
    (*ostream) << std::endl;
    
    for (unsigned int i = 0; i < stat_name_vector.size (); i++)
    {
        if (i < S_DAMAGE)
        {
            (*ostream) << std::left << std::setw (25) << Upper (stat_name_vector[i]) 
                << std::left << std::setw (3) << get_stat (i, false) << " (" << get_stat (i, true) << ")" << std::endl;
        }
        else
        {
            if (i == S_DAMAGE)
            {
                (*ostream) << std::left << std::setw (25) << Upper (stat_name_vector[i]) 
                    << std::left << std::setw (3) << get_damage () << std::endl;
            }
            else
            {
                (*ostream) << std::left << std::setw (25) << Upper (stat_name_vector[i]) 
                    << std::left << std::setw (3) << get_damage_resistance () << std::endl;
            }
        }
        if (i == 6 || i == 10)
        {
            (*ostream) << std::endl;
        }
    }
#ifdef __DEBUG__
    (*ostream) << std::endl;
    (*ostream) << info (4);
    (*ostream) << std::endl;
#endif
}

void CCreature::cmd_attack (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    std::ostringstream os;
    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Attack a creature.";
#endif
#if (LANG == HUN)
            (*ostream) << "Megtámad egy élõlényt.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Show debug information about the program." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_ATTACK << " <creature>" << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_ATTACK << " wolf" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Megtámad egy élõlényt.";
            (*ostream) << "Szintaktika: " << C_CMD << CMD_ATTACK << " <élõlény>" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_ATTACK << " farkas" << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }
    if (params.empty ())
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "No parameter. See help: " << C_CMD <<  CMD_HELP << " " << CMD_ATTACK << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
        (*ostream) << C_ERR << "Nincs parametér. Nézd meg a súgót: " << C_CMD <<  CMD_HELP << " " << CMD_ATTACK << C_RST << std::endl;
#endif // (LANG == HUN)
    }
    else
    {
        CThingList Childs = parent->childs;
        //Childs.remove (this);
        if (!Childs.empty ())
        {
            int mode; // mode of get_name ()
#if (LANG == ENG)
            mode = M_ARTICLE | M_NORMAL;
#endif
#if (LANG == HUN)
            mode = M_ARTICLE | M_RAG_T;
#endif
            CThingList except;
            except.push_back (this);
            CStringVector sv;
            CSplit split;
            sv = split ("\\s*,\\s*", params);
            bool found = false;
            for (unsigned int j = 0; j < sv.size (); j++)
            {
                std::string name = sv[j];
                for (CThingListIt i = Childs.begin (); i != Childs.end (); i++)
                {
                    CThing *th = *i;
                    CCreature *opponent = dynamic_cast<CCreature*> (th);
                    if (opponent && opponent->compare_name (name) && opponent->isAlive ())
                    {
                        found = true;
                        if (opponent == this)
                        {
#if (LANG == ENG)
                            (*ostream) << C_DO << "You cannot attack yourself." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                            (*ostream) << C_DO << "Nem tudod megtámadni magad." << C_RST << std::endl;
#endif
                        }
                        else
                        {
                            // Mark the creature as attacked. Function combat() will handle the situation.
                            set_iparam (K_ATTACKED, th->get_sn ());
                            os.str ("");
#if (LANG == ENG)
                            (*ostream) << C_DO << "You have attacked " << th->get_name (mode) << "." << C_RST << std::endl;
                            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " has attacked " << th->get_name (mode) << "." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                            (*ostream) << C_DO << "Megtámadtad " << th->get_name (mode) << "." << C_RST << std::endl;
                            os << C_DO << get_name (M_ARTICLE | M_CAPITAL_FIRST) << " megtámadta " << th->get_name (mode) << "." << C_RST << std::endl;
#endif
                            write_to_spectators (os.str (), except);
                        }
                        break;
                    }
                }
            }
            if (!found)
            {
#if (LANG == ENG)
                (*ostream) << C_ERR << "You cannot attack." << C_RST << std::endl;
#endif
#if (LANG == HUN)
                (*ostream) << C_ERR << "Nem tudod megtámadni." << C_RST << std::endl;
#endif
            }
        }
    }
}

void CCreature::cmd_about (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Show information about author and program";
#endif
#if (LANG == HUN)
            (*ostream) << "Kiírja a szerzõ és a program adatait";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Show information about author and program." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_ABOUT << C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_ABOUT << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Kiírja a szerzõ és a program adatait." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_ABOUT << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_ABOUT << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }

    (*ostream) << CApp::ABOUT << std::endl;
}

void CCreature::cmd_help (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Help system.";
#endif
#if (LANG == HUN)
            (*ostream) << "Súgó rendszer.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Help system. You can get help for commands." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_HELP << " [section]"<< C_RST << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_HELP << " " << CMD_LOOK << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Súgó rendszer. Segítséget kérhetsz a parancsokról." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_HELP << " [téma]" << C_RST << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_HELP << " " << CMD_LOOK << C_RST << std::endl;
#endif
            (*ostream) << std::endl;
        }
        return;
    }
    if (params == "")
    {
#if (LANG == ENG)
        (*ostream) << "Known commands: " << std::endl;
#endif
#if (LANG == HUN)
        (*ostream) << "Ismert parancsok: " << std::endl;
#endif
        for (CFunctorMapIt i = parser_map.begin (); i != parser_map.end (); i++)
        {
            (*ostream) << C_CMD << std::setw (10) << std::left << i->first << C_RST << " ";
            std::string help_params = i->first + " " + CMD_BRIEF;
            (*i->second) (CMD_HELP, help_params);
            (*ostream) << std::endl;
        }
    }
    else
    {
        CFunctorMapIt i = parser_map.find (params);
        if (i != parser_map.end ())
        {
            CFunctor *functor = parser_map[params];
            std::string help_params = i->first + " " + CMD_VERBOSE;
            (*functor) (CMD_HELP, help_params);
        }
        else
        {
#if (LANG == ENG)
            (*ostream) << C_ERR << "Unknown command: " << C_CMD << params << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << C_ERR << "Ismeretlen parancs: " << C_CMD << params << C_RST << std::endl;
#endif
        }
    }
}

void CCreature::cmd_info (const std::string& cmd, const std::string& params)
{
    assert (ostream != NULL);

    CRegEx regex ("(\\S+)\\s+(\\S+)");
    int verbose_level = 0;
    if (cmd == CMD_HELP && regex.Matches (params))
    {
        std::string help_cmd = regex.GetMatch (params, 1),
                    help_params = regex.GetMatch (params, 2);
        if (help_params == CMD_BRIEF)
        {
#if (LANG == ENG)
            (*ostream) << "Show debug information about the program.";
#endif
#if (LANG == HUN)
            (*ostream) << "Hibajavításhoz hasznos adatokat ír ki.";
#endif
        }
        else if (help_params == CMD_VERBOSE)
        {
#if (LANG == ENG)
            (*ostream) << "Show debug information about the program." << std::endl;
            (*ostream) << "Syntax: " << C_CMD << CMD_INFO
                << " [-v <verbose_level>] [-a|-all] [-m|-maps] [-c|-creatures] [-i|-items] [serial number] [a-b] [name]" 
                << C_RST << std::endl;
            (*ostream) << "Switches: " << std::endl;
            (*ostream) << " -v: set verbose level (0-5). Default: " << verbose_level << "." << std::endl;
            (*ostream) << " -a or -all: search in all things." << std::endl;
            (*ostream) << " -m or -maps: search in maps." << std::endl;
            (*ostream) << " -c or -creatures: search in creatures." << std::endl;
            (*ostream) << " -i or -items: search in items." << std::endl;
            (*ostream) << " serial number: search serial number." << std::endl;
            (*ostream) << " a-b: search range of serial numbers." << std::endl;
            (*ostream) << " name: search for a name." << std::endl;
            (*ostream) << "Example: " << C_CMD << CMD_INFO << " -v 5 player" << C_RST << std::endl;
#endif
#if (LANG == HUN)
            (*ostream) << "Hibajavításhoz hasznos adatokat ír ki." << std::endl;
            (*ostream) << "Szintaktika: " << C_CMD << CMD_INFO
                << " [-v <verbose_level>] [-a|-all] [-m|-maps] [-c|-creatures] [-i|-items] [serial number] [a-b] [name]" 
                << C_RST << std::endl;
            (*ostream) << "Kapcsolók: " << std::endl;
            (*ostream) << " -v: beszédesség szintje (0-5). Alap: " << verbose_level << "." << std::endl;
            (*ostream) << " -a vagy -all: mindenben keres." << std::endl;
            (*ostream) << " -m vagy -maps: térképek közt keres." << std::endl;
            (*ostream) << " -c vagy -creatures: élõlények között keres." << std::endl;
            (*ostream) << " -i vagy -items: tárgyak közt keres." << std::endl;
            (*ostream) << " serial number: sorszámot keres." << std::endl;
            (*ostream) << " a-b: sorszám tartományt keres." << std::endl;
            (*ostream) << " name: nevet keres" << std::endl;
            (*ostream) << "Például: " << C_CMD << CMD_INFO << " -v 5 player" << C_RST << std::endl;
#endif
            (*ostream) << "         " << C_CMD << CMD_INFO << " 122 130-135 player" << C_RST << std::endl;
            (*ostream) << "         " << C_CMD << CMD_INFO << " -i" << C_RST << std::endl;
            (*ostream) << std::endl;
        }
        return;
    }
    if (params.empty ())
    {
        verbose_level = 2;
        (*ostream) << info (verbose_level) << std::endl;
        (*ostream) << parent->info (verbose_level) << std::endl;
    }
    else
    {
        bool ok = false;
        CStringVector sv;
        CSplit split;
        sv = split ("\\s+", params);
        CThingListIt begin = global_thinglist.begin (),
                     end = global_thinglist.end ();
        CRegEx regexNum ("^(\\d+)$");
        CRegEx regexRange ("^(\\d+)-(\\d+)$");
        for (unsigned int j = 0; j < sv.size (); j++)
        {
            if (sv[j] == "-all" || sv[j] == "-a")
            {
                begin = global_thinglist.begin (); 
                end = global_thinglist.end ();
            }
            else if (sv[j] == "-maps" || sv[j] == "-m")
            {
                begin = CMap::global_maplist.begin (); 
                end = CMap::global_maplist.end ();
            }
            else if (sv[j] == "-creatures" || sv[j] == "-c")
            {
                begin = CCreature::global_creaturelist.begin (); 
                end = CCreature::global_creaturelist.end ();
            }
            else if (sv[j] == "-items" || sv[j] == "-i")
            {
                begin = CItem::global_itemlist.begin (); 
                end = CItem::global_itemlist.end ();
            }
            else if (sv[j] == "-v")
            {
                if (sv.size () >= j)
                {
                    std::string s = sv[j + 1];
                    verbose_level = std::strtol (s.c_str (), NULL, 10);
                    j++;
                }
            }
            else if (regexNum.Matches (sv[j]))
            {
                // searching serial number
                std::string s = regexNum.GetMatch (sv[j], 1);
                int num = std::strtol (s.c_str (), NULL, 10);
                for (CThingListIt i = begin; i != end; i++)
                {
                    if ((*i)->get_sn () == num)
                    {
                        (*ostream) << (*i)->info (verbose_level);
                        break;
                    }
                }
                ok = true;
            }
            else if (regexRange.Matches (sv[j]))
            {
                // search range of serial numbers
                std::string s = regexRange.GetMatch (sv[j], 1);
                int min = std::strtol (s.c_str (), NULL, 10);
                s = regexRange.GetMatch (sv[j], 2);
                int max = std::strtol (s.c_str (), NULL, 10);
                for (CThingListIt i = begin; i != end; i++)
                {
                    if ((*i)->get_sn () >= min && (*i)->get_sn () <= max)
                    {
                        (*ostream) << (*i)->info (verbose_level);
                    }
                }
                ok = true;
            }
            else
            {
                for (CThingListIt i = begin; i != end; i++)
                {
                    if ((*i)->get_id ().find (sv[j]) != std::string::npos ||
                            (*i)->compare_name (sv[j]))
                    {
                        (*ostream) << (*i)->info (verbose_level);
                    }
                }
                ok = true;
            }
        }
        if (!ok)
        {
            for (CThingListIt i = begin; i != end; i++)
            {
                (*ostream) << (*i)->info (0);
            }
        }
    }
#if 0
    else
    {
#if (LANG == ENG)
        (*ostream) << C_ERR << "Invalid parameter. See help: " << C_CMD <<  CMD_HELP << " " << CMD_INFO << C_RST << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
        (*ostream) << C_ERR << "Ismeretlen parametér. Nézd meg a súgót: " << C_CMD <<  CMD_HELP << " " << CMD_INFO << C_RST << std::endl;
#endif // (LANG == HUN)
        return;
    }
#endif
}

int CCreature::get_weight ()
{
    return get_stat (S_BDY) * 8;
}

float CCreature::get_load_weight ()
{
    float load = 0;
    if (!childs.empty ())
    {
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            if (th->get_type () == CItem::ITEM)
            {
                load += th->get_fparam (CItem::K_WEIGHT);
            }
        }
    }
    return load;
}

float CCreature::get_loadability ()
{
    return (get_stat (S_BDY) + get_stat (S_STR)) * 2;
}

std::string CCreature::get_damage ()
{
    std::string damage;
    bool first = true;
    if (!get_sparam (K_DAMAGE).empty ())
    {
        // the creature can wound with bare hands, claws or teeth
        first = false;
        damage += get_sparam (K_DAMAGE);
    }
    // testing creature's weared posessions to calculate the damage
    if (!childs.empty ())
    {
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            if (th->get_type () == CItem::ITEM &&
                    !th->get_sparam (CItem::K_WEAREDON).empty () &&
                    !th->get_sparam (K_DAMAGE).empty ())
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    damage += ",";
                }
                damage += th->get_sparam (K_DAMAGE);
            }
        }
    }
    return damage;
}

std::string CCreature::get_damage_resistance ()
{
    std::string damage_resistance;
    bool first = true;
    if (!get_sparam (K_DAMAGE_RESISTANCE).empty ())
    {
        // the creature can wound with bare hands, claws or teeth
        first = false;
        damage_resistance += get_sparam (K_DAMAGE_RESISTANCE);
    }
    if (!childs.empty ())
    {
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            if (th->get_type () == CItem::ITEM &&
                    !th->get_sparam (CItem::K_WEAREDON).empty () &&
                    !th->get_sparam (K_DAMAGE_RESISTANCE).empty ())
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    damage_resistance += ",";
                }
                damage_resistance += th->get_sparam (K_DAMAGE_RESISTANCE);
            }
        }
    }
    return damage_resistance;
}

int CCreature::get_stat (int stat_type, bool base)
{
    int stat;
    switch (stat_type)
    {
        case S_MAX_HP:
            stat = (get_stat (S_BDY) + get_stat (S_WIL)) * 5;
            break;
        case S_MAX_MP:
            stat = (get_stat (S_INT) + get_stat (S_WIL)) * 5;
            break;
        case S_ATTACK:
            stat = (get_stat (S_STR) + get_stat (S_DEX)); // + képzettség szintje * 2
            break;
        case S_DEFENSE:
            stat = (get_stat (S_BDY) + get_stat (S_DEX)); // + képzettség szintje * 2
            break;
        case S_DAMAGE:
            stat = -1;
            //stat = dice (get_sparam (K_DAMAGE));
        default:
            stat = CThing::get_stat (stat_type);
            break;
    }
    if (!base && !childs.empty ())
    {
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = *i;
            // TODO not only items can change statistics (curses)!!!
            if (th->get_type () == CItem::ITEM &&
                    !th->get_sparam (CItem::K_WEAREDON).empty ())
            {
                stat += th->get_stat (stat_type);
            }
        }
    }
    return stat;
}

// :set encoding=iso8869-2:
