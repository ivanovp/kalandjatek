/*
 * File:        ansi.h
 * Purpose:     ANSI color codes
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/30
 * Last modify: 2008-08-21 10:39:28 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file ansi.h
 * \brief ANSI color codes.
 */

#ifndef __INCLUDE_ANSI_H
#define __INCLUDE_ANSI_H

// Reset settings to white on black
#define ANSI_NORMAL             "\x1B[0m"
#define ANSI_BRIGHT             "\x1B[1m"
#define ANSI_BOLD               ANSI_BRIGHT     // alias
#define ANSI_ITALICS            "\x1B[3m"
#define ANSI_NOITALICS          "\x1B[23m"
#define ANSI_UNDERLINE          "\x1B[4m"
#define ANSI_NOUNDERLINE        "\x1B[24m"
#define ANSI_FLASH              "\x1B[5m"
#define ANSI_NOFLASH            "\x1B[25m"
#define ANSI_BLINK              ANSI_FLASH      // alias
#define ANSI_NOBLINK            ANSI_NOFLASH    // alias

// Foreground colors
#define ANSI_BLACK              "\x1B[30m"
#define ANSI_RED                "\x1B[31m"
#define ANSI_GREEN              "\x1B[32m"
#define ANSI_YELLOW             "\x1B[33m"
#define ANSI_BLUE               "\x1B[34m"
#define ANSI_MAGENTA            "\x1B[35m"
#define ANSI_CYAN               "\x1B[36m"
#define ANSI_WHITE              "\x1B[37m"
// Background colors
#define ANSI_BBLACK             "\x1B[40m"
#define ANSI_BRED               "\x1B[41m"
#define ANSI_BGREEN             "\x1B[42m"
#define ANSI_BYELLOW            "\x1B[43m"
#define ANSI_BBLUE              "\x1B[44m"
#define ANSI_BMAGENTA           "\x1B[45m"
#define ANSI_BCYAN              "\x1B[46m"
#define ANSI_BWHITE             "\x1B[47m"

#endif // __INCLUDE_ANSI_H
