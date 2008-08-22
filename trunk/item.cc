/*
 * File:        item.cc
 * Purpose:     CItem class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 11:38:35 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <string>
#include "item.h"
#include "debug.h"
#include "lang.h"
#include "keyword.h"

const std::string CItem::K_PRICE      = "fPrice";       // ar
const std::string CItem::K_WEIGHT     = "fWeight";      // tomeg
const std::string CItem::K_MOVABLE    = "iMovable";     // mozgathato/felveheto targy (creature can move it)
const std::string CItem::K_VISIBLE    = "iVisible";     // ,,nez'' paranccsal lathato (can be seen with command "look")
const std::string CItem::K_WEAREDON   = "sWearedOn";    // min viseli a tagyat (nem a konfig fajlban hasznalatos)

// CItem keywords
const std::string CItem::K_TYPE       = "sType";

const std::string CItem::K_RING       = "ring";         // gyuru
const std::string CItem::K_BRACELET   = "bracelet";     // karkoto
const std::string CItem::K_AMULET     = "amulet";       // nyaklanc
const std::string CItem::K_CLOAK      = "cloak";        // kopeny/csuha
const std::string CItem::K_PANTS      = "pants";        // nadrag
const std::string CItem::K_FOOTWEAR   = "footwear";     // labbeli
const std::string CItem::K_GLOVE      = "glove";        // kesztyu
const std::string CItem::K_CAP        = "cap";          // kalap/sapka
const std::string CItem::K_TORSO      = "torso";        // torzs/torzo (mellvert)
const std::string CItem::K_ARMGUARDS  = "armguards";    // alkarvedo
const std::string CItem::K_SHINGUARDS = "shinguards";   // labszarvedo
const std::string CItem::K_ONE_HANDED = "one handed";   // egykezes (fegyver)
const std::string CItem::K_TWO_HANDED = "two handed";   // ketkezes (fegyver)
const std::string CItem::K_CURRENCY   = "currency";     // fizetoeszkoz
const std::string CItem::K_KEY        = "key";          // kulcs

CThingList CItem::global_itemlist;

void CItem::init ()
{
    global_itemlist.push_back (this);
}

CItem::CItem () : CThing::CThing ()
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    init ();
}

CItem::CItem (const std::string& id, const std::string& name, const std::string& descr, CThing* parent) :
    CThing::CThing (id, name, descr, parent)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CItem::CItem ();
    init ();
}

CItem::CItem (CThing& thing) : CThing::CThing (thing)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " copy constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CItem::CItem ();
    init ();
}

CItem::CItem (CItem& thing) : CThing::CThing (thing)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " copy constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CItem::CItem ();
    init ();
}

CItem::~CItem ()
{
    /*std::ostringstream os;
    os << __INFO__ << "destructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    CThingListIt i = std::find (global_itemlist.begin (), global_itemlist.end (), this);
    if (i != global_itemlist.end ())
    {
        global_itemlist.remove (*i);
    }
    else
    {
        std::ostringstream os;
        os << __INFO__ << "Internal error. Item not found in global_itemlist (sn: " << get_sn () << ")!";
        Log.warn (os.str ());
    }
}

std::string CItem::get_shape ()
{
    std::ostringstream os;
    os << "I";
    return os.str ();
}

int CItem::get_color ()
{
    return 0;
}

void CItem::do_something ()
{
    /*std::ostringstream os;
    os << __INFO__ << "I can't do anything.";
    Log.warn (os.str ());*/
    childs_do_something ();
}

std::string CItem::get_price_str (float Price)
{
    std::ostringstream os;
    bool first = true;
    float price = (Price == -1) ? get_fparam (K_PRICE) : Price;
    int gold = static_cast<int> (price); // rounding
    if (gold > 0)
    {
        first = false;
#if (LANG == ENG)
        os << gold << " gold";
#endif
#if (LANG == HUN)
        os << gold << " arany";
#endif
    }
    float fraction = ((float) price - (int) price);
    float silver = 10.0 * fraction;
    if ((int) silver > 0)
    {
        if (!first) 
        {
            os << " ";
        }
        first = false;
#if (LANG == ENG)
        os << silver << " silver";
#endif
#if (LANG == HUN)
        os << silver << " ezüst";
#endif
    }
    int copper = (static_cast <int> ((10.0 * silver + 0.5)) % 10);
    if (copper > 0)
    {
        if (!first) 
        {
            os << " ";
        }
        first = false;
#if (LANG == ENG)
        os << copper << " copper";
#endif
#if (LANG == HUN)
        os << copper << " réz";
#endif
    }
    if (price == 0)
    {
#if (LANG == ENG)
        os << "valueless";
#endif
#if (LANG == HUN)
        os << "értéktelen";
#endif
    }
#ifdef __DEBUG__
    os << " (" << price << ")"; // debug
#endif
    return os.str ();
}

