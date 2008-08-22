/*
 * File:        regex.h
 * Purpose:     CRegEx class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005-01-13
 * Last modify: 2006-05-19 11:03:24 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file regex.h
 * \brief CRegEx class header.
 * Perl compatible regular expression class.
 */

#ifndef __INCLUDE_REGEX_H
#define __INCLUDE_REGEX_H

#include <pcre.h>
#include <iostream>
#include <string>
#include "debug.h"

/// Options for Compile ()
enum
{
    /**
     * \brief Extended regex syntax (default).
     * If a pattern is compiled with the PCRE_EXTENDED option, whitespace 
     * in the pattern (other than in a character class) and characters 
     * between a "#" outside a character class and the next newline character
     * are ignored. An escaping backslash can be used to include a 
     * whitespace or "#" character as part of the pattern.
     */
    RE_EXTENDED = PCRE_EXTENDED,     // wxCRegEx compatible (Perl's /x option)
    /// Use basic RE syntax.
    RE_BASIC    = 0,

    /// Ignore case in match.
    RE_ICASE    = PCRE_CASELESS,     // wxCRegEx compabitible

    /// Only check match, don't set back references.
    RE_NOSUB    = 0,

    /// If not set, treat '&#92;n' as an ordinary character, otherwise it is
    /// special: it is not matched by '.' and '^' and '$' always match
    /// after/before it regardless of the setting of wxRE_NOT[BE]OL.
    RE_NEWLINE  = 0,

    /// Default flags.
    RE_DEFAULT  = RE_EXTENDED,       // wxCRegEx compatible
    
    RE_ANCHORED = PCRE_ANCHORED,
    RE_CASELESS = PCRE_CASELESS,
    /** 
     * If PCRE_DOLLAR_ENDONLY is set and PCRE_MULTILINE is not
     * set, the $ meta-character matches only at the very end of
     * the string.
     * The  PCRE_DOLLAR_ENDONLY option is ignored if PCRE_MULTILINE is set.
     */
    RE_DOLLAR_ENDONLY = PCRE_DOLLAR_ENDONLY,
    /**
     * Perl's /s option. If the PCRE_DOTALL option is set, then dots match newlines as well.
     */
    RE_DOTALL = PCRE_DOTALL,
    /**
     * If PCRE_EXTRA is set, a backslash followed by a letter with no special meaning is faulted.
     */
    RE_EXTRA = PCRE_EXTRA,
    RE_MULTILINE = PCRE_MULTILINE,
    /**
     * If PCRE_UNGREEDY is set, the greediness of the repetition quantifiers 
     * is inverted, that is, by default they are not greedy, but if followed 
     * by a question mark they are.
     */
    RE_UNGREEDY = PCRE_UNGREEDY,
    RE_UTF8 = PCRE_UTF8              // TODO
};

/// Options for Matches ()
enum
{
    RE_NORMAL = 0,

    /// '^' doesn't match at the start of line
    RE_NOTBOL = PCRE_NOTBOL,         // wxCRegEx compatible

    /// '$' doesn't match at the end of line
    RE_NOTEOL = PCRE_NOTEOL,         // wxCRegEx compatible

    /// special PCRE flag
    RE_NOTEMPTY = PCRE_NOTEMPTY
};

#define OVECTOR_SIZE 90    ///< Should be a multiple of 3.

/**
 * Perl compatible regular expression class.
 *
 * More info: <code>man pcre</code> or <a href="http://www.pcre.org/pcre.txt">http://www.pcre.org/pcre.txt</a>.
 *
 * Example:
 * <pre>
CRegEx regex ("^\\s*(\\S+)\\s+(.*)$");
std::string input = "  say  hello world!";
if (regex.Matches (input))
{
     std::string cmd = regex.GetMatch (input, 1),
                 params = regex.GetMatch (input, 2);
     std::cout << "cmd: " << cmd << std::endl;
     std::cout << "params: " << params << std::endl;
}
else
{
     std::cout << "Invalid input." << std::endl;
}
 * </pre>
 */
class CRegEx
{
public:
    CRegEx ();
    /**
     * Initialize object and compiles a regular expression.
     * \see CRegEx::Compile
     */
    CRegEx (const std::string& expr, int flags = RE_DEFAULT);
    ~CRegEx ();

    /**
     * Compiles a regular expression.
     * \param pattern Regular expression. Example: "^Pelda[k]$"
     * \param flags Options.
     * \return True if compilation successful.
     */
    bool Compile (const std::string& pattern, int flags = RE_DEFAULT);

    /**
     * After compilation of a regular expression you can
     * check matching of string.
     * \param text A string, example: <code>"Peldak"</code> (matches to <code>"^Pelda[k]$"</code>)
     * \param flags RE_NOTBOL or RE_NOTEOL.
     * \return True if text matches to pattern.
     */
    bool Matches (const std::string& text, int flags = RE_NORMAL);

    /**
     * Check for valid regular expression pattern.
     * \return True if regular expression found.
     */
    bool IsValid() const { return this->re != NULL; }
    
    /**
     * Get position of matched string.
     * \param start Position of matched text.
     * \param len Length of matched text.
     * \param index Index of selected text. 0 means the whole text.
     */
    bool GetMatch(size_t *start, size_t *len, size_t index) const;

    /**
     * Get substring of matched string.
     * \param text Matched text.
     * \param index Index of selected text. 0 means the whole text.
     */
    std::string GetMatch (const std::string& text, size_t index = 0) const;

private:
    const char *error_msg;
    int error_offset;
    pcre *re;
    pcre_extra *pe;
    int rc;
    int ovector[OVECTOR_SIZE];
};

extern CRegEx regex;

#endif // __INCLUDE_REGEX_H

