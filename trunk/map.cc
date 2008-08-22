/*
 * File:        map.cc
 * Purpose:     CMap class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 11:40:14 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <string>
#include "map.h"
#include "debug.h"

const std::string CMap::K_EXITS = "sExits";        // kijáratok
CThingList CMap::global_maplist;

void CMap::init ()
{
    global_maplist.push_back (this);
}

CMap::CMap () : CThing::CThing ()
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    init ();
}

CMap::CMap (const std::string& id, const std::string& name, const std::string& descr, CThing* parent) :
    CThing::CThing (id, name, descr, parent)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CMap::CMap ();
    init ();
}

CMap::CMap (CThing& thing) : CThing::CThing (thing)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " copy constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CMap::CMap ();
    init ();
}

CMap::CMap (CMap& thing) : CThing::CThing (thing)
{
    /*std::ostringstream os;
    os << __INFO__ << __PRETTY_FUNCTION__ << " copy constructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    //CMap::CMap ();
    init ();
}

CMap::~CMap ()
{
    /*std::ostringstream os;
    os << __INFO__ << "destructor, sn: " << get_sn ();
    Log.debug (os.str ());*/
    CThingListIt i = std::find (global_maplist.begin (), global_maplist.end (), this);
    if (i != global_maplist.end ())
    {
        global_maplist.remove (*i);
    }
    else
    {
        std::ostringstream os;
        os << __INFO__ << "Internal error. Map not found in global_maplist (sn: " << get_sn () << ")!";
        Log.warn (os.str ());
    }
}

void CMap::draw ()
{
    std::ostringstream os;
    os << __INFO__ << "Soon.";
    Log.warn (os.str ());
}

void CMap::do_something ()
{
    /*std::ostringstream os;
    os << __INFO__ << "I can't do.";
    Log.warn (os.str ());*/
    childs_do_something ();
}

