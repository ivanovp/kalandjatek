/*
 * File:        kj.cc
 * Purpose:     Main program
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005-01-13
 * Last modify: 2008-08-21 10:39:19 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "app.h"

int main (int argc, char* argv[])
{
#ifdef GTKMM
    Gtk::Main kit (argc, argv);
    CApp app;
    // Shows the window and returns when it is closed.
    Gtk::Main::run (app);
#else
    CApp app;
    app.run ();
#endif
    return 0;
} // main

