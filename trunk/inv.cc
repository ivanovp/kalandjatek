/*
 * File:        inv.cc
 * Purpose:     CInventory class
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/26
 * Last modify: 2008-08-25 16:15:48 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "inv.h"
#include "debug.h"

CInventory::CInventory (CInventoryMap &inventory_map)
{
    os = new std::ostringstream;
    set_inventory_map (inventory_map);
}

CInventory::~CInventory ()
{
    delete os;
}

void CInventory::set_inventory_map (CInventoryMap &inventory_map)
{
    this->inventory_map = &inventory_map;
}

std::string CInventory::write ()
{
    bool first = true,
         last = false;
    CInventoryMapIt i;
    unsigned int j;
    os->str ("");
    for (i = inventory_map->begin (), j = 0; i != inventory_map->end (); i++, j++)
    {
        std::string name = i->first;
        int counter = i->second.counter;
        int noun = i->second.noun;
        std::string plural = i->second.plural;
        
        if (j == inventory_map->size () - 1)
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
            const std::string number[9] =
#if (LANG == ENG)
            {"two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
#endif
#if (LANG == HUN)
            {"két", "három", "négy", "öt", "hat", "hét", "nyolc", "kilenc", "tíz"};
#endif
            if (counter <= 10)
            {
                (*os) << number[counter - 2] << " " << name;
            }
            else
            {
                (*os) << counter << " " << name;
            }
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
    return os->str ();
}

std::ostream& operator<< (std::ostream &os, CInventory &inv)
{
    os << inv.write ();
    return os;
}
