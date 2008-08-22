/*
 * File:        split.cc
 * Purpose:     CSplit class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005-04-14
 * Last modify: 2008-08-19 11:03:34 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "split.h"
#include "debug.h"

CSplit split;

CSplit::CSplit()
{
    regex = new CRegEx;
}

CSplit::~CSplit()
{
    delete regex;
}

CStringVector CSplit::split (const std::string& pattern, const std::string& expr, int limit, int flags)
{
    CStringVector stringvector;
    std::string Expr = expr;
    if (stringvector.size () != 0)
        stringvector.clear ();
    regex->Compile ("(" + pattern + ")", flags);
    if (!regex->Matches (Expr))
    {
        stringvector.push_back (Expr);
        return stringvector;
    }
    int i = 0;
    while (regex->Matches (Expr) && (limit == 0 || i < limit))
    {
        size_t start, len; 
        if (regex->GetMatch (&start, &len, 1))
        {
            /*std::ostringstream os;
            os << __INFO__ << "start = " << start << " len = " << len;
            Log.debug (os.str ());*/
            stringvector.push_back (Expr.substr (0, start));
            Expr = Expr.substr (start + len);
        }
        i++;
    }
    stringvector.push_back (Expr);
    return stringvector;
}

