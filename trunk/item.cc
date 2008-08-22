/*
 * File:        item.cc
 * Purpose:     CItem class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 09:45:50 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <string>
#include "item.h"
#include "debug.h"
#include "lang.h"
#include "keyword.h"

const char* CItem::K_PRICE    = "fPrice";       // ár
const char* CItem::K_WEIGHT   = "fWeight";      // tömeg
const char* CItem::K_MOVABLE  = "iMovable";     // mozgatható/felvehetõ tárgy
const char* CItem::K_VISIBLE  = "iVisible";     // ,,néz'' paranccsal látható
const char* CItem::K_WEAREDON = "sWearedOn";    // min viseli a tárgyat (nem a konfig fájlban használatos)

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
        os << silver << " ez�st";
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
        os << copper << " r�z";
#endif
    }
    if (price == 0)
    {
#if (LANG == ENG)
        os << "valueless";
#endif
#if (LANG == HUN)
        os << "�rt�ktelen";
#endif
    }
#ifdef __DEBUG__
    os << " (" << price << ")"; // debug
#endif
    return os.str ();
}

