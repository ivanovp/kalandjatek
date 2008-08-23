/*
 * File:        inv.cc
 * Purpose:     CInventory class
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/26
 * Last modify: 2008-08-23 10:14:49 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "inv.h"
#include "debug.h"

CInventory::CInventory (std::ostream *os)
{
    set_stream (os);
}

CInventory::CInventory ()
{
    os = NULL;
}

CInventory::~CInventory ()
{
}

void CInventory::set_stream (std::ostream *os)
{
    this->os = os;
}

std::string CInventory::write ()
{
    bool mine = false;
    if (os == NULL)
    {
        os = new std::ostringstream;
        mine = true;
    }
    bool first = true,
         last = false;
    CInventoryMapIt i;
    unsigned int j;
    for (i = inventory_map.begin (), j = 0; i != inventory_map.end (); i++, j++)
    {
        std::string name = i->first;
        int counter = i->second.counter;
        int noun = i->second.noun;
        std::string plural = i->second.plural;
        
        if (j == inventory_map.size () - 1)
        {
            last = true;
        }
        if (!first && !last)
        {
            (*os) << ", ";
        }
        if (!first && last)
        {
#if (LANG == ENG)
            (*os) << " and ";
#endif
#if (LANG == HUN)
            (*os) << " és ";
#endif
        }
        first = false;
        if (counter > 1)
        {
#if (LANG == ENG)
            if (!plural.empty ())
            {
                name = plural;
            }
            else
            {
                name += "s";
            }
#endif
            (*os) << counter << " " << name;
        }
        else
        {
            if (!noun)
            {
#if (LANG == ENG)
                (*os) << "one ";
#endif
#if (LANG == HUN)
                (*os) << "egy ";
#endif
            }
            (*os) << name;
        }
    }
    std::string s;
    if (mine)
    {
        std::ostringstream *os2 = dynamic_cast<std::ostringstream*> (os);
        if (os2)
        {
            s = os2->str ();
        }
        delete os;
    }
    return s;
}
