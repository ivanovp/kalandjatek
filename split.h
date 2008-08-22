/*
 * File:        split.h
 * Purpose:     CSplit class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005-04-14
 * Last modify: 2008-08-19 11:03:56 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file split.h
 * \brief CSplit class header.
 * Perl compatible split class.
 */

#ifndef __INCLUDE_SPLIT_H
#define __INCLUDE_SPLIT_H

#include <string>
#include <vector>
#include "regex.h"
#include "debug.h"

typedef std::vector<std::string> CStringVector;           ///< std::string list type
typedef CStringVector::iterator CStringVectorIt;            ///< std::string list iterator type

/**
 * Perl compatible split class.
 *
 * You can split string with regular expressions. Example:
 * <pre>
CSplit split;
CStringVector sl;
sl = split (":", "root:x:0:0:root:/root:/bin/bash");
 * </pre>
 */
class CSplit
{
private:
    /// Regular expression pattern for splitting 'expr'.
    CRegEx *regex;

public:
    CSplit ();
    ~CSplit ();

    /**
     * Splits the 'expr' string into pieces where pattern matches.
     * \param pattern Regular expression for pattern matching.
     * \param expr String to be split.
     * \param limit Maximum number of pieces. Zero means unlimited.
     * \param flags Flags for regular expression.
     * \return Splitted strings.
     */
    CStringVector split (const std::string& pattern, const std::string& expr, int limit = 0, int flags = RE_DEFAULT);

    /**
     * Splits the 'expr' string into pieces where pattern matches.
     * \param pattern Regular expression for pattern matching.
     * \param expr String to be split.
     * \param limit Maximum number of pieces. Zero means unlimited.
     * \param flags Flags for regular expression.
     * \return Splitted strings.
     */
    CStringVector operator() (const std::string& pattern, const std::string& expr, int limit = 0, int flags = RE_DEFAULT)
    {
        return split (pattern, expr, limit, flags);
    };
};

extern CSplit split;

#endif // __INCLUDE_SPLIT_H

