/*
 * File:        trans.cc
 * Purpose:     CTrans class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/18
 * Last modify: 2008-08-22 12:40:34 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "trans.h"
#include "debug.h"

CTrans::CTrans ()
{
    l1 = l2 = "";
    lists_ok = false;
}

CTrans::CTrans (const std::string& _l1, const std::string& _l2)
{
    CTrans ();
    set_lists (_l1, _l2);
}

CTrans::~CTrans ()
{
}

std::string CTrans::compile_list (const std::string& list)
{
    std::string clist; // compiled list
    for (unsigned int i = 0; i < list.size (); i++)
    {
        if (list[i] == '\\')
        {
            // escape sequences: '\-' or '\\'
            if (list[i + 1] == '\\' || list[i + 1] == '-')
            {
                clist += list[++i];
            }
            else
            {
                std::ostringstream os;
                os << __INFO__ << "Invalid char (" << list[i + 1] << ") after '\\' at position " << i + 1 << "!";
                Log.error (os.str ());
                lists_ok = false;
                return "";
            }
        }
        else if (list[i + 1] == '-')
        {
            // range: a-z
            if (list.size () > i + 2)
            {
                if (list[i] < list[i + 2])
                {
                    for (int j = list[i]; j <= list[i + 2]; j++)
                        clist += j;
                }
                else
                {
                    for (int j = list[i]; j >= list[i + 2]; j--)
                        clist += j;
                }
                i += 2;
            }
            else
            {
                std::ostringstream os;
                os << __INFO__ << "Invalid dash character (-) at end of string: " << list;
                Log.error (os.str ());
                lists_ok = false;
                return "";
            }
        }
        else
        {
            // normal character
            clist += list[i];
        }
    }
    return clist;
}

bool CTrans::set_lists (const std::string& _l1, const std::string& _l2)
{
    lists_ok = true;

    l1 = compile_list (_l1);
    l2 = compile_list (_l2);

    return lists_ok;
}
   
int CTrans::tr (std::string& text, flags_t flags)
{
    s = "";
    int c; // counting replaced characters
    if (lists_ok)
    {
        c = 0;
        if (text.size () > 0 && l1.size () > 0)
        {
            for (unsigned int i = 0; i < text.size (); i++)
            {
                int pos = l1.find (text[i]);
                if (pos != -1)
                {
                    if (flags != TR_DELETE)
                    {
                        s += l2[(unsigned int) pos >= l2.size () ? l2.size () - 1 : pos];
                        c++;
                    }
                    if (flags == TR_DELETE && (unsigned int) pos < l2.size ())
                    {
                        s += l2[pos];
                        c++;
                    }
                }
                else
                {
                    s += text[i];
                }
            }
            text = s;
        }
    }
    else
    {
        c = -1;
        std::ostringstream os;
        os << __INFO__ << "Lists are invalid!";
        Log.error (os.str ());
    }
    return c;
}

int CTrans::tr (const std::string& l1, const std::string& l2, std::string& text, flags_t flags)
{
    set_lists (l1, l2);
    return tr (text);
}

