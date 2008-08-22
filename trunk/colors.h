/*
 * File:        colors.h
 * Purpose:     Default colors
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/30
 * Last modify: 2008-08-21 10:40:22 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file colors.h
 * \brief Default colors.
 */

#ifndef __INCLUDE_COLORS_H
#define __INCLUDE_COLORS_H

#include "ansi.h"

#ifndef GTKMM

/// Resetting to default setting (white on black)
#define C_RST           ANSI_NORMAL
/// Color of commands in help system
#define C_CMD           ANSI_CYAN
/// Color of errors
#define C_ERR           (std::string) ANSI_RED + ANSI_BOLD 
/// If someone do something
#define C_DO            (std::string) ANSI_CYAN + ANSI_BOLD
/// Common highlight
#define C_HL            ANSI_YELLOW
/// Prompt
#define C_PROMPT        ANSI_YELLOW

#else // GTKMM

/// Escape character (for CApp::insert ()).
#define C_ESCAPE        '\x1B'
/// End character (for CApp::insert ()).
#define C_END           '}'
#define C_RESET         "reset"

/// Resetting to default setting (black on white).
#define C_RST           "\x1B{reset}"
// Color of commands in help system
#define C_CMD           "\x1B{command}"
// Color of errors
#define C_ERR           "\x1B{error}"
// Color if someone do something
#define C_DO            "\x1B{do}"
// Color of common highlight
#define C_HL            "\x1B{highlight}"
// Color of prompt
#define C_PROMPT        "\x1B{prompt}"

#endif // GKTMM

#endif // __INCLUDE_COLORS_H
