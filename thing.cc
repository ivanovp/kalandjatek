/*
 * File:        thing.cc
 * Purpose:     CThing class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/01/13
 * Last modify: 2008-08-27 13:24:50 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <algorithm>
#include <iomanip>

#include "thing.h"
#include "debug.h"
#include "lang.h"
#include "keyword.h"
#include "split.h"
#include "trans.h"

const std::string CThing::THING          = "thing"; // for get_type()
const std::string CThing::K_NAME         = "sName";
const std::string CThing::K_NOUN         = "iNoun";         // koznev/tulajdonnev
const std::string CThing::K_DESCR        = "sDescr";
const std::string CThing::K_ALTNAME      = "sAltName";
const std::string CThing::K_RAGOK        = "sRagok";        // hungarian endings
const std::string CThing::K_PLURAL       = "sPlural";       // t�bbes sz�m
const std::string CThing::K_CHILDS       = "sChilds";       // possessions
const std::string CThing::K_BODY         = "iBody";
const std::string CThing::K_STRENGTH     = "iStrength";
const std::string CThing::K_DEXTERITY    = "iDexterity";
const std::string CThing::K_PERCEPTION   = "iPerception";
const std::string CThing::K_INTELLIGENCE = "iIntelligence";
const std::string CThing::K_WILLPOWER    = "iWillpower";
const std::string CThing::K_CHARISMA     = "iCharisma";
const std::string CThing::K_BDY          = K_BODY;
const std::string CThing::K_STR          = K_STRENGTH;
const std::string CThing::K_DEX          = K_DEXTERITY;
const std::string CThing::K_PER          = K_PERCEPTION;
const std::string CThing::K_INT          = K_INTELLIGENCE;
const std::string CThing::K_WIL          = K_WILLPOWER;
const std::string CThing::K_CHA          = K_CHARISMA;
const std::string CThing::K_HP           = "iHP";
const std::string CThing::K_MP           = "iMP";
const std::string CThing::K_ATTACK       = "iAttack";
const std::string CThing::K_DEFENSE      = "iDefense";
const std::string CThing::K_DAMAGE       = "sDamage";
const std::string CThing::K_DAMAGE_RESISTANCE = "sDamageResistance";

signed long CThing::last_sn = 0; ///< Last used serial number.

CThingList CThing::global_thinglist;
CThingList CThing::global_spectator_thinglist;

CStringVector CThing::stat_name_vector (S_STAT_NUMBER);
CStringVector CThing::stat_keyword_vector (S_STAT_NUMBER);

void CThing::init ()
{
    type = THING;
    sn = last_sn++;
    /*std::ostringstream os;
    os << __INFO__ << __FUNCTION__ << " sn: " << get_sn ();
    Log.debug (os.str ());*/
    parent = NULL;
    global_thinglist.push_back (this);
    // initialize only once
    if (sn == 0)
    {
#if (LANG == ENG)
        stat_name_vector[S_BODY]                = "body";
        stat_name_vector[S_STRENGTH]            = "strength";
        stat_name_vector[S_DEXTERITY]           = "dexterity";
        stat_name_vector[S_PERCEPTION]          = "perception";
        stat_name_vector[S_INTELLIGENCE]        = "intelligence";
        stat_name_vector[S_WILLPOWER]           = "willpower";
        stat_name_vector[S_CHARISMA]            = "charisma";
        stat_name_vector[S_CURR_HP]             = "hit points";
        stat_name_vector[S_MAX_HP]              = "max. hit points";
        stat_name_vector[S_CURR_MP]             = "mana points";
        stat_name_vector[S_MAX_MP]              = "max. mana points";
        stat_name_vector[S_ATTACK]              = "attack";
        stat_name_vector[S_DEFENSE]             = "defense";
        stat_name_vector[S_DAMAGE]              = "damage";
        stat_name_vector[S_DAMAGE_RESISTANCE]   = "damage resistance";
#endif
#if (LANG == HUN)
        stat_name_vector[S_BODY]                = "test";
        stat_name_vector[S_STRENGTH]            = "er�";
        stat_name_vector[S_DEXTERITY]           = "�gyess�g";
        stat_name_vector[S_PERCEPTION]          = "�rz�kel�s";
        stat_name_vector[S_INTELLIGENCE]        = "intelligencia";
        stat_name_vector[S_WILLPOWER]           = "akarater�";
        stat_name_vector[S_CHARISMA]            = "karizma";
        //stat_name_vector[S_HP]                  = "�leter� pont m�dos�t�";
        //stat_name_vector[S_MP]                  = "mana pont m�dos�t�";
        stat_name_vector[S_CURR_HP]             = "�leter� pont";
        stat_name_vector[S_MAX_HP]              = "maxim�lis �leter� pont";
        stat_name_vector[S_CURR_MP]             = "mana pont";
        stat_name_vector[S_MAX_MP]              = "maxim�lis mana pont";
        stat_name_vector[S_ATTACK]              = "t�mad�s";
        stat_name_vector[S_DEFENSE]             = "v�dekez�s";
        stat_name_vector[S_DAMAGE]              = "sebz�s";
        stat_name_vector[S_DAMAGE_RESISTANCE]   = "sebz�s ellen�ll�s";
#endif
        stat_keyword_vector[S_BODY]             = K_BODY;
        stat_keyword_vector[S_STRENGTH]         = K_STRENGTH;
        stat_keyword_vector[S_DEXTERITY]        = K_DEXTERITY;
        stat_keyword_vector[S_PERCEPTION]       = K_PERCEPTION;
        stat_keyword_vector[S_INTELLIGENCE]     = K_INTELLIGENCE;
        stat_keyword_vector[S_WILLPOWER]        = K_WILLPOWER;
        stat_keyword_vector[S_CHARISMA]         = K_CHARISMA;
        //stat_keyword_vector[S_HP]               = K_HP;
        //stat_keyword_vector[S_MP]               = K_MP;
        stat_keyword_vector[S_CURR_HP]          = K_HP;
        stat_keyword_vector[S_MAX_HP]           = -1;
        stat_keyword_vector[S_CURR_MP]          = K_MP;
        stat_keyword_vector[S_MAX_MP]           = -1;
        stat_keyword_vector[S_ATTACK]           = K_ATTACK;
        stat_keyword_vector[S_DEFENSE]          = K_DEFENSE;
        stat_keyword_vector[S_DAMAGE]           = K_DAMAGE;
        stat_keyword_vector[S_DAMAGE_RESISTANCE]= -1;
    }
}

CThing::CThing ()
{
    init ();
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
}

CThing::CThing (const std::string& id, const std::string& name, const std::string& descr, CThing* parent)
{
    init ();
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    set_id (id);
    if (!name.empty ())
    {
        set_name (name);
    }
    if (!descr.empty ())
    {
        set_descr (descr);
    }
    this->parent = parent;
    //global_thinglist.push_back (this);
}

CThing::CThing (CThing& thing)
{
    init ();
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " copy constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    set_id (thing.id);
    sparams = thing.sparams;
    iparams = thing.iparams;
    fparams = thing.fparams;
    parent = thing.parent;
    copy_childs_from (thing);
}

CThing& CThing::operator= (CThing& thing)
{
    std::cout << __PRETTY_FUNCTION__ << " called!!!" << std::endl;
    if (this != &thing)
    {
        return *thing.copy ();
    }
    return *this;
}

CThing::~CThing ()
{
    std::ostringstream os;
    /*os << __INFO__ << "destructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //set_spectator (false);
    if (!childs.empty ())
    {
        // deleting all childs
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            delete (*i);
        }
    }
    CThingListIt i = std::find (global_thinglist.begin (), global_thinglist.end (), this);
    if (i != global_thinglist.end ())
    {
        global_thinglist.remove (*i);
    }
    else
    {
        os.str ("");
        os << __INFO__ << "Internal error. Thing not found in global_thinglist (sn: " << get_sn () << ")!";
        Log.warn (os.str ());
    }
}

void CThing::setup ()
{
    std::string childs_str = get_sparam (K_CHILDS);
    if (!childs_str.empty ())
    {
        // split object for splitting string into pieces
        CSplit split;
        // for storing string pieces
        CStringVector sv;
        sv = split.split (K_LISTSEPARATOR, childs_str);
        if (!sv.empty ())
        {
            CRegEx regex ("(\\d+)\\s+(\\S+)");
            for (CStringVectorIt i = sv.begin (); i != sv.end (); i++)
            {
                int n = 1;
                std::string id = *i;
                if (regex.Matches (*i))
                {
                    // getting the number of items
                    n = atoi (regex.GetMatch (*i, 1).c_str ());
                    // getting the id of item
                    id = regex.GetMatch (*i, 2);
                }
                CThing *th;
                // search in main_items
                th = find (id, global_thinglist);
                // FIXME a global_thinglist helyett a main_items-bol kellene masolni!
                //th = find (id, main_items);
                if (th)
                {
                    for (int j = 0; j < n; j++)
                    {
                        // copying thing!
                        CThing *th_copy = th->copy ();
                        th_copy->parent = this;
                        childs.push_back (th_copy);
                    }
                }
                else
                {
                    // FIXME what should we do?
                    std::ostringstream os;
#if (LANG == ENG)
                    os << __INFO__ << "'" << id << "' not found! Identifier: " << get_id () << ".";
#endif
#if (LANG == HUN)
                    os << __INFO__ << "'" << id << "' nem l�tezik! Azonos�t�: " << get_id () << ".";
#endif
                    /*
#if (LANG == ENG)
                    os << __INFO__ << "'" << id << "' not found! File: " << fileid << 
                        " line: " << cfg.get_line_counter () << ".";
#endif
#if (LANG == HUN)
                    os << __INFO__ << "'" << id << "' nem l�tezik! F�jl: " << fileid << 
                        " sor: " << cfg.get_line_counter () << ".";
#endif
                    */
                    Log.warn (os.str ());
                }
            }
        }
    }
}

void CThing::copy_childs_to (CThing& thing)
{
    if (&thing == NULL)
    {
        std::ostringstream os;
        os << __INFO__ << "thing not exists!";
        Log.error (os.str ());
    }
    else if (!childs.empty ())
    {
        // copying all childs
        for (CThingListIt i = childs.begin (); i != childs.end (); i++)
        {
            CThing *th = (*i)->copy ();
            th->parent = &thing;
            thing.childs.push_back (th);
        }
    }
}

void CThing::copy_childs_from (CThing& thing)
{
    if (&thing == NULL)
    {
        std::ostringstream os;
        os << __INFO__ << "thing not exists!";
        Log.error (os.str ());
    }
    else if (!thing.childs.empty ())
    {
        // copying all childs
        for (CThingListIt i = thing.childs.begin (); i != thing.childs.end (); i++)
        {
            CThing *th = (*i)->copy ();
            th->parent = this;
            childs.push_back (th);
        }
    }
}

void CThing::move_to (CThing& thing)
{
    if (parent != NULL)
    {
        parent->childs.remove (this);
    }
    else
    {
        std::ostringstream os;
        os << __INFO__ << "No parent (parent == NULL)!";
        Log.warn (os.str ());
    }
    parent = &thing;
    parent->childs.push_back (this);
}

signed long CThing::get_sn ()
{
    return sn;
}

bool CThing::set_id (const std::string& id)
{
    if (!id.length ())
    {
        std::ostringstream os;
        os << __INFO__ << "ID length is zero!";
        Log.warn (os.str ());
        return false;
    }
    this->id = id;
    return true;
}

std::string CThing::get_id ()
{
    return id;
}

bool CThing::set_name (const std::string& name)
{
    if (!name.length ())
    {
        std::ostringstream os;
        os << __INFO__ << "name length is zero!";
        Log.warn (os.str ());
        return false;
    }
    set_sparam (K_NAME, name);
    return true;
}

std::string CThing::get_name (int mode)
{
    bool article = false;       // definite article
    bool iarticle = false;      // indefinite article
    bool capital_first = false;
    int Mode = mode;
    std::ostringstream s;
    std::string name = get_sparam (K_NAME);
    // Regul�ris kifejez�s annak meg�llap�t�s�ra, hogy
    // a sz�veg mag�nhangz�val kezd�dik-e
    // Regular expression to determine, that the text begins with vowel
    CRegEx mgh ("^[a�e�i�o���u���A�E�I�O���U���]");
    if (Mode & M_ARTICLE)
    {
        article = true;
        Mode -= M_ARTICLE;
    }
    if (Mode & M_IARTICLE)
    {
        iarticle = true;
        Mode -= M_IARTICLE;
    }
    if (Mode & M_CAPITAL_FIRST)
    {
        capital_first = true;
        Mode -= M_CAPITAL_FIRST;
    }
    if (article && iarticle)
    {
        std::ostringstream os;
        os << __INFO__ << "M_ARTICLE and M_IARTICLE defined at same time.";
        Log.error (os.str ());
        return "";
    }
    if (article && get_iparam (K_NOUN) == 0)
    {
#if (LANG == ENG)
        s << "the ";
#endif
#if (LANG == HUN)
        if (mgh.Matches (name))
        {
            s << "az ";
        }
        else
        {
            s << "a ";
        }
#endif
    }
    if (iarticle && get_iparam (K_NOUN) == 0)
    {
#if (LANG == ENG)
        if (mgh.Matches (name))
        {
            s << "an ";
        }
        else
        {
            s << "a ";
        }
#endif
#if (LANG == HUN)
        s << "egy ";
#endif
    }
    if (Mode == 0)
    {
        s << name;
    }
#if (LANG == HUN)
    if (Mode > 0)
    {
        CSplit split;
        CStringVector sv = split (K_LISTSEPARATOR, get_sparam (K_RAGOK));
        if ((unsigned int) Mode - 1 >= sv.size ())
        {
            std::ostringstream os;
            os << __INFO__ << "In " << K_RAGOK << " vectorlist's index " << Mode << 
                " is greater than vector size " << sv.size () << ".";
            Log.error (os.str ());
            return "";
        }
        name = sv[Mode - 1];
        s << name;
        return s.str ();
    }
#endif
    if (capital_first)
    {
        s.str (Upper (s.str ()));
    }
    return s.str ();
}

bool CThing::compare_name (const std::string& name)
{
    CStringVector sv;
    CSplit split;
    if (lower (name) == lower (get_sparam (K_NAME)))
    {
        return true;
    }
#if (LANG == HUN)
    sv = split (K_LISTSEPARATOR, get_sparam (K_RAGOK));
    for (unsigned int i = 0; i < sv.size (); i++)
    {
        if (lower (name) == lower (sv[i]))
        {
            return true;
        }
    }
#endif
    sv = split (K_LISTSEPARATOR, get_sparam (K_ALTNAME));
    for (unsigned int i = 0; i < sv.size (); i++)
    {
        if (lower (name) == lower (sv[i]))
        {
            return true;
        }
    }
    if (lower (name) == lower (get_sparam (K_PLURAL)))
    {
        return true;
    }
    return false;
}

bool CThing::set_descr (const std::string& descr)
{
    if (!descr.length ())
    {
        std::ostringstream os;
        os << __INFO__ << "descr length is zero!";
        Log.warn (os.str ());
        return false;
    }
    set_sparam (K_DESCR, descr);
    return true;
}

std::string CThing::get_descr ()
{
    return get_sparam (K_DESCR);
}

bool CThing::set_sparam (const std::string& variable, const std::string& value)
{
    if (variable[0] != 's')
    {
        std::ostringstream os;
        os << __INFO__ << "First character of variable must be 's'!";
        Log.error (os.str ());
        return false;
    }
    sparams[variable] = value;
    return true;
}

const std::string empty_string = "";

const std::string& CThing::get_sparam (const std::string& variable)
{
    if (sparams.find (variable) != sparams.end ())
    {
        return sparams[variable];
    }
    else
    {
        // XXX Parameter not found! Returning an empty string. 
        // Shall I throw an exception?
        return empty_string;
    }
}

bool CThing::set_iparam (const std::string& variable, const int& value)
{
    if (variable[0] != 'i')
    {
        std::ostringstream os;
        os << __INFO__ << "First character of variable must be 'i'!";
        Log.error (os.str ());
        return false;
    }
    iparams[variable] = value;
    return true;
}

int CThing::inc_iparam (const std::string& variable, const int& value)
{
    if (variable[0] != 'i')
    {
        std::ostringstream os;
        os << __INFO__ << "First character of variable must be 'i'!";
        Log.error (os.str ());
        return 0;
    }
    iparams[variable] += value;
    return iparams[variable];
}

int CThing::dec_iparam (const std::string& variable, const int& value)
{
    if (variable[0] != 'i')
    {
        std::ostringstream os;
        os << __INFO__ << "First character of variable must be 'i'!";
        Log.error (os.str ());
        return 0;
    }
    iparams[variable] -= value;
    return iparams[variable];
}

int CThing::get_iparam (const std::string& variable)
{
    if (iparams.find (variable) != iparams.end ())
    {
        return iparams[variable];
    }
    else
    {
        // XXX Parameter not found! Returning zero. Shall I throw an exception?
        return 0;
    }
}

bool CThing::set_fparam (const std::string& variable, const float& value)
{
    if (variable[0] != 'f')
    {
        std::ostringstream os;
        os << __INFO__ << "First character of variable must be 'f'!";
        Log.error (os.str ());
        return false;
    }
    fparams[variable] = value;
    return true;
}

float CThing::get_fparam (const std::string& variable)
{
    if (fparams.find (variable) != fparams.end ())
    {
        return fparams[variable];
    }
    else
    {
        // XXX Parameter not found! Returning zero. Shall I throw an exception?
        return 0;
    }
}

int CThing::get_stat (int stat_type, bool base)
{
    if (stat_keyword_vector.size () <= static_cast<unsigned int> (stat_type))
    {
        std::ostringstream os;
        os << __INFO__ << __PRETTY_FUNCTION__ << " invalid stat type!!!";
        Log.warn (os.str ());
    }
    return get_iparam (stat_keyword_vector[stat_type]);
}

std::string CThing::get_shape ()
{
    std::ostringstream os;
    os << "T";
    return os.str ();
}

int CThing::get_color ()
{
    return 0;
}

void CThing::do_something ()
{
    std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " calls childs_do_something()";
    Log.warn (os.str ());
    childs_do_something ();
}

void CThing::childs_do_something ()
{
    std::ostringstream os;
    if (!childs.empty ())
    {
        std::vector<CThing*> childs2;
        childs2.reserve (childs.size ());
        // we've got childs, copy the list
        // after calling do_something() the list iterator could be faulty!!!
        // (because the creatures can move to another place!)
        std::copy (childs.begin (), childs.end (), std::back_inserter (childs2));
        for (unsigned int i = 0; i < childs2.size (); i++)
        {
            childs2[i]->do_something ();
        }
    }
}

CThing* CThing::find (const std::string& id, CThingList& thinglist, bool recursive)
{
    for (CThingListIt i = thinglist.begin (); i != thinglist.end (); i++)
    {
        if ((*i)->get_id () == id) 
        {
            return *i;
        }
        if (recursive && !(*i)->childs.empty ()) 
        {
            CThing *thing = find (id, (*i)->childs);
            if (thing != NULL)
            {
                return thing;
            }
        }
    }
    return NULL;
}

CThing* CThing::find (const int sn, CThingList& thinglist, bool recursive)
{
    for (CThingListIt i = thinglist.begin (); i != thinglist.end (); i++)
    {
        if ((*i)->get_sn () == sn) 
        {
            return *i;
        }
        if (recursive && !(*i)->childs.empty ()) 
        {
            CThing *thing = find (sn, (*i)->childs);
            if (thing != NULL)
            {
                return thing;
            }
        }
    }
    return NULL;
}

std::string CThing::info (int verbose_level)
{
    std::ostringstream os;
    if (verbose_level == 0)
    {
        os << "type: " << std::left << std::setw (8) << get_type () << " ";
        os << "sn: " << std::left << std::setw (5) << get_sn () << " ";
        os << "id: [" << get_id () << "]";
        os << std::endl;
    }
    else
    {
        os << "---[ CThing info ]------------------------------------------------------------" << std::endl;
        os << "type: " << get_type () << std::endl;
        os << "sn: " << get_sn () << std::endl;
        os << "id: [" << get_id () << "]" << std::endl;
        if (verbose_level <= 1) return os.str ();
        os << "name: [" << get_name () << "]" << std::endl;
        os << "descr: [" << get_descr () << "]" << std::endl;
        if (verbose_level <= 2) return os.str ();
        if (parent)
        {
            os << "parent: " << parent->get_id () << " (sn: " << parent->get_sn () << ")" << std::endl;
        }
        else
        {
            os << "parent: NONE" << std::endl;
        }
        os << "childs: " << childs.size () << " thing(s)" << std::endl;
        if (verbose_level <= 3) return os.str ();
        if (verbose_level >= 4)
        {
            for (CThingListIt i = childs.begin (); i != childs.end (); i++)
            {
                os << "\t" << (*i)->get_id () << " (sn: " << (*i)->get_sn () << ")" << std::endl;
            }
        }
        os << "sparams: " << sparams.size () << " variable(s)" << std::endl;
        if (verbose_level >= 5)
        {
            for (CThingSParamIt i = sparams.begin (); i != sparams.end (); i++)
            {
                os << "\t" << i->first << " = [" << i->second << "]" << std::endl;
            }
        }
        os << "iparams: " << iparams.size () << " variable(s)" << std::endl;
        if (verbose_level >= 5)
        {
            for (CThingIParamIt i = iparams.begin (); i != iparams.end (); i++)
            {
                os << "\t" << i->first << " = " << i->second << std::endl;
            }
        }
        os << "fparams: " << fparams.size () << " variable(s)" << std::endl;
        if (verbose_level >= 5)
        {
            for (CThingFParamIt i = fparams.begin (); i != fparams.end (); i++)
            {
                os.precision (3);
                os.setf (std::ios_base::fixed, std::ios_base::floatfield);
                os << "\t" << i->first << " = " << i->second << std::endl;
            }
        }
    }
    return os.str ();
}

std::ostream& operator<< (std::ostream &os, CThing &thing)
{
    os << thing.info ();
    return os;
}

