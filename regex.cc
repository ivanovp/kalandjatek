/*
 * File:        regex.cc
 * Purpose:     CRegEx class
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005-01-13
 * Last modify: 2006-05-19 11:03:22 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "regex.h"
#include "debug.h"

CRegEx regex;

CRegEx::CRegEx()
{
    re = NULL;
    rc = 0;
    error_msg = NULL;
}

CRegEx::CRegEx(const std::string& pattern, int flags)
{
    CRegEx ();
    Compile (pattern, flags);
}

CRegEx::~CRegEx()
{
}

bool CRegEx::Compile(const std::string& pattern, int flags)
{
    // TODO Check unrecognized flags.
    /// \todo Check unrecognized flags.

    //wxASSERT_MSG( !(flags &
    //                    ~(wxPCRE_BASIC | wxPCRE_ICASE | wxPCRE_NOSUB | wxPCRE_NEWLINE)),
    //              _T("unrecognized flags in wxCRegEx::Compile") );

    re = pcre_compile(
        pattern.c_str (),   // the pattern
        flags,              // default options
        &error_msg,         // for error message
        &error_offset,      // for error offset
        NULL);              // use default character tables

    if (re == NULL)
    {
        //wxLogError(_T("Invalid regular expression '%s' at offset %i: %s"),
        //    pattern.c_str(), this->error_offset, this->error_msg);
        std::ostringstream os;
        os << __INFO__ << "Invalid regular expression '" << 
            pattern.c_str () << "' at offset " << error_offset << ":" << 
            error_msg;
        Log.error (os.str ());
        return false;
    }
    
    pe = pcre_study(
      re,                   // result of pcre_compile()
      0,                    // no options exist
      &error_msg);          // set to NULL or points to a message
    if (this->error_msg != NULL)
    {
        //wxLogError(_T("Can't study regular expression '%s': %s"),
        //    pattern.c_str(), this->error_msg);
        std::ostringstream os;
        os << __INFO__ << "Can't study regular expression '" << 
            pattern.c_str () << "':" << error_msg;
        Log.error (os.str ());
        return false;
    }

    return true;
}

bool CRegEx::Matches (const std::string& text, int flags) //const
{
    // TODO Check unrecognized flags.
    /// \todo Check unrecognized flags.

    //wxCHECK_MSG( IsValid(), FALSE, _T("must successfully Compile() first") );
    if (!IsValid ())
    {
        std::ostringstream os;
        os << __INFO__ << "Must successfully Compile() first";
        Log.error (os.str ());
        return false;
    }

    std::string t = text;
    bool match = false;
    
    rc = pcre_exec(
        re,                // result of pcre_compile()
        NULL,              // we didn't study the pattern
        t.c_str (),        // the subject string
        t.length (),       // the length of the subject string
        0,                 // start at offset 0 in the subject
        flags,             // default options
        ovector,           // vector for substring information
        OVECTOR_SIZE);     // number of elements in the vector
    if (rc < 0)
    {
        if (rc != PCRE_ERROR_NOMATCH)
        {
            //wxLogError (_T("Matching error %d"), this->rc);
            std::ostringstream os;
            os << __INFO__ << "Matching error " << rc;
            Log.error (os.str ());
        }
    }
    else
        match = true;
    return match;
}

bool CRegEx::GetMatch (size_t *start, size_t *len, size_t index) const
{
    if (!IsValid ())
    {
        std::ostringstream os;
        os << __INFO__ << "Must succesfully Compile() first!";
        Log.error (os.str ());
        return false;
    }
    if ((int)index >= rc)
    {
        std::ostringstream os;
        os << __INFO__ << "Invalid match index: " << index << " (max: " << rc - 1 << ")!";
        Log.error (os.str ());
        return false;
    }

    *start = ovector[2 * index];
    *len = ovector[2 * index + 1] - ovector[2 * index];

    return true;
}

std::string CRegEx::GetMatch (const std::string& text, size_t index) const
{
    size_t start, len;
    if (!GetMatch (&start, &len, index))
        return "";

    return text.substr (start, len);
}
