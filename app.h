/*
 * File:        app.h
 * Purpose:     CApp class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/14
 * Last modify: 2008-08-22 11:18:36 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file app.h
 * \brief CApp class header.
 */

#ifndef __INCLUDE_APP_H
#define __INCLUDE_APP_H

#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>

#include <log4cpp/Category.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>

#ifdef GTKMM
#include <gtkmm.h>
#endif

#include "thing.h"
#include "item.h"
#include "creature.h"
#include "map.h"
#include "loader.h"
#include "lang.h"

typedef std::map<std::string,std::string> CSMap;        ///< String map type
typedef CSMap::iterator CSMapIt;                        ///< String map iterator type
typedef std::map<std::string,int> CIMap;                ///< Integer map type
typedef CIMap::iterator CIMapIt;                        ///< Integer map iterator type
typedef std::map<std::string,float> CFMap;              ///< Float map type
typedef CFMap::iterator CFMapIt;                        ///< Float map iterator type

/// Application class
class CApp
#ifdef GTKMM
: public Gtk::Window
#endif
{
private:
    // Configuration variables
    CSMap smap;                         ///< String configuration variables.
    CIMap imap;                         ///< Integer configuration variables.
    CFMap fmap;                         ///< Float configuration variables.

    CThingList *main_items,             ///< Main items
               *main_creatures,         ///< Main creatures
               *universe;               ///< Maps
    CCreature *player;
    std::ostringstream player_os;
#ifdef __DEBUG__
    // only for debugging
    CCreature *player2;
    std::ostringstream player2_os;
#endif
    
    /// Initializes logging system.
    void init_log ();
    /// Load items, creatures, maps.
    void init_universe ();
    /// Unload items, creatures, maps.
    void delete_universe ();
    /// Load configuration file.
    void load_config ();
    /// Save configuration file.
    void save_config ();
#ifdef GTKMM
    void init_window ();
    void delete_window ();
    void update_text_buffer ();

    /// Inserting formatted text to text_buffer.
    Gtk::TextIter insert (const Gtk::TextIter& pos, const Glib::ustring& text);
    /// Callback function for clicking on quit button.
    void on_quit ();
    /// Callback function for activating text entry.
    void on_entry ();
    /// Callback function called if textbuf changed.
    void on_textbuf ();
    /// Callback function for setting preferences.
    void on_preferences ();
    /// Callback function for toggling auto scroll.
    void on_auto_scroll ();
    /// Callback function for showing about window.
    void on_about ();
    /// Callback function for not implemented things.
    void on_not_implemented ();
#endif
    /// Call periodical (100 msec).
    bool do_something ();
    /// Show all of data. Only for debugging.
    std::string show_universe (int verbose_level = 2);

protected:
#ifdef GTKMM
    Gtk::VBox *main_vbox;
    Gtk::ScrolledWindow *scrolled_window;
    Gtk::TextView *text_view;
    Glib::RefPtr<Gtk::TextBuffer> text_buffer;
    Gtk::Entry *entry;
    Glib::RefPtr<Gtk::UIManager> ui_manager;
    Glib::RefPtr<Gtk::ActionGroup> action_group;
    //Glib::RefPtr<Gtk::RadioAction> choice_one, choice_two;
    Glib::RefPtr<Gtk::ToggleAction> auto_scroll;
#endif

public:
    static const std::string ABOUT;
    static const char* DEFAULT_FONT;
    static const char* DEFAULT_ITEM_CFG;
    static const char* DEFAULT_CREATURE_CFG;
    static const char* DEFAULT_MAP_CFG;
    static const char* DEFAULT_PLAYER_ID;
    static const char* DEFAULT_ALIASES;
    static const int DEFAULT_AUTO_SCROLL;
    static const char* DEFAULT_LOG_FILE;
    static const char* CFG_FILE;
    static const char* AUTHOR;
    static const char* HOMEPAGE;
    static const char* PROG_NAME;
    static const char* PROG_VERSION_STR;
    static const int PROG_VERSION;
    static const std::string PROG_HEADER;
    // CApp keywords (kj.cfg)
    static const char* K_WINDOW_X;
    static const char* K_WINDOW_Y;
    static const char* K_WINDOW_W;
    static const char* K_WINDOW_H;
    static const char* K_AUTO_SCROLL;
    static const char* K_ITEM_CFG;
    static const char* K_CREATURE_CFG;
    static const char* K_MAP_CFG;
    static const char* K_PLAYER_ID;
    static const char* K_FONT;
    static const char* K_ALIASES;

    /// Initializing application data
    CApp ();
    /// Destroying application data
    virtual ~CApp ();
#ifdef GTKMM
    void hide ();
#else
    /// Run the application.
    void run ();
#endif
};

#endif // __INCLUDE_APP_H
