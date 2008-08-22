/*
 * File:        trans.h
 * Purpose:     CTrans class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/18
 * Last modify: 2008-08-22 12:44:43 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file trans.h
 * \brief CTrans class header.
 * Perl compatible translator (tr) class.
 */

#ifndef __INCLUDE_TRANS_H
#define __INCLUDE_TRANS_H

#include <string>
#include <iostream>
#include <fstream>
#include <pcre.h>
#include "regex.h"

// TODO Implement TR_COMPLEMENT and TR_SQUASH options!!!
/**
 * Perl compatible translator (tr). Except Perl's /c and /s options.
 * \todo Implement TR_COMPLEMENT (/c) and TR_SQUASH (/s) options!!!
 *
 * Translates text using search and replacement list.
 * If search list is "abcd" and replacement list is "ABCD" then all 'a' replaced to 'A' and so on. 
 * A-D means ABCD.
 * 
 * Example:
 * <pre>
CTrans tr;
std::string s = "abcdefg";
tr.tr ("a-z", "A-Z", s);
// counting stars in sky
std::string sky = "***the*sky**";
std::cout << "stars in sky: " << tr ("*", "*", sky) << std::endl;
 * </pre>
 */
class CTrans
{
private:
    std::string l1;             ///< Search list.
    std::string l2;             ///< Replacement list.
    std::string s;              ///< Temporary string for tr ()
    bool lists_ok;              ///< Lists are compiled and valid.

    std::string compile_list (const std::string& list);
    
public:
    typedef enum
    {
        /// Default mode.
        TR_NORMAL = 0,

        /**
         * <b>Not implemented</b>.
         * If the TR_COMPLEMENT modifier is specified, the SEARCHLIST character set is complemented.
         */
        TR_COMPLEMENT = 1,

        /**
         * If the TR_DELETE modifier is specified, any characters specified by SEARCHLIST not found in 
         * REPLACEMENTLIST are deleted. 
         */
        TR_DELETE = 2,

        /**
         * <b>Not implemented</b>.
         * If the TR_SQUASH modifier is specified, sequences of characters that were transliterated to 
         * the same character are squashed down to a single instance of the character.
         */
        TR_SQUASH = 4
    } flags_t;

    /// Constructor.
    CTrans ();
    
    /**
     * Constructor with set search and replacement lists.
     * \param l1 Search list.
     * \param l2 Replacement list.
     */
    CTrans (const std::string& l1, const std::string& l2);
    
    /// Destructor.
    ~CTrans ();

    /**
     * Set search and replacement lists.
     * \param l1 Search list.
     * \param l2 Replacement list.
     * \return True: if lists are compiled and valid.
     */
    bool set_lists (const std::string& l1, const std::string& l2);
    
    /**
     * Translates text using search and replacement lists.
     * \param text Text to be translating.
     * \param flags @see flags_t
     * \return If lists are valid: number of replaced characters. If lists are invalid: -1
     */
    int tr (std::string& text, flags_t flags = TR_NORMAL);

    /**
     * Translates text using search and replacement lists.
     * \param l1 Search list.
     * \param l2 Replacement list.
     * \param text Text to be translating.
     * \param flags @ee flags_t.
     * \return If lists are valid: number of replaced characters. If lists are invalid: -1
     */
    int tr (const std::string& l1, const std::string& l2, std::string& text, flags_t flags = TR_NORMAL);

    /**
     * Translates text using search and replacement lists.
     * \param text Text to be translating.
     * \param flags @see flags_t
     * \return If lists are valid: number of replaced characters. If lists are invalid: -1
     */
    int operator() (std::string& text, flags_t flags = TR_NORMAL)
    { 
        return tr (text, flags);
    };
    
    /**
     * Translates text using search and replacement lists.
     * \param l1 Search list.
     * \param l2 Replacement list.
     * \param text Text to be translating.
     * \param flags @see flags_t
     * \return If lists are valid: number of replaced characters. If lists are invalid: -1
     */
    int operator() (const std::string& l1, const std::string& l2, std::string& text, flags_t flags = TR_NORMAL)
    { 
        return tr (l1, l2, text, flags);
    };
};

#endif // __INCLUDE_TRANS_H

