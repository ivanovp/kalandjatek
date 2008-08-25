/*
 * File:        app.cc
 * Purpose:     CApp class
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/14
 * Last modify: 2008-08-25 09:41:34 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <cstdlib>
#include <ctime>

#include "app.h"
#include "debug.h"
#include "creature.h"
#include "split.h" // for debugging
#include "colors.h"
#include "cfg.h"

// Default values of the application
const char* CApp::DEFAULT_FONT = "Monospace 10";
const char* CApp::DEFAULT_ITEM_CFG = "item.cfg";
const char* CApp::DEFAULT_CREATURE_CFG = "creature.cfg";
const char* CApp::DEFAULT_MAP_CFG = "map.cfg";
const char* CApp::DEFAULT_PLAYER_ID = "player";
#if (LANG == HUN)
const char* CApp::DEFAULT_ALIASES = ".:pontok/':mond/n:nÈz/l:lelt·r -l/t:t·mad";
#else
const char* CApp::DEFAULT_ALIASES = "a:attack/.:points/':say/l:look/i:inventory -l";
#endif
const int CApp::DEFAULT_AUTO_SCROLL = 1;
const char* CApp::DEFAULT_LOG_FILE = "kj.log";
const char* CApp::CFG_FILE = "kj.cfg";
const char* CApp::AUTHOR = "PÈter Ivanov <ivanovp@gmail.com>";
const char* CApp::HOMEPAGE = "http://kj.ivanov.eu/";
const char* CApp::PROG_NAME = "Kalandj·tÈk";
const char* CApp::PROG_VERSION_STR = "0.2";
const int CApp::PROG_VERSION = 0x0020;
const std::string CApp::PROG_HEADER = std::string (PROG_NAME) + " v" + PROG_VERSION_STR;

#if (LANG == ENG) 
const std::string CApp::ABOUT = PROG_HEADER + "\n" "Licence: GPL\n" "Author: " + AUTHOR + "\n" "Homepage: " + HOMEPAGE;
#endif // (LANG == ENG)
#if (LANG == HUN) 
const std::string CApp::ABOUT = PROG_HEADER + "\n" "Licensz: GPL\n" "Szerzı: " + AUTHOR + "\n" "Honlap: " + HOMEPAGE;
#endif // (LANG == HUN)

const char* CApp::K_WINDOW_X     = "iWindowX";
const char* CApp::K_WINDOW_Y     = "iWindowY";
const char* CApp::K_WINDOW_W     = "iWindowW";
const char* CApp::K_WINDOW_H     = "iWindowH";
const char* CApp::K_AUTO_SCROLL  = "iAutoScroll";
const char* CApp::K_ITEM_CFG     = "sItemCfg";
const char* CApp::K_CREATURE_CFG = "sCreatureCfg";
const char* CApp::K_MAP_CFG      = "sMapCfg";
const char* CApp::K_PLAYER_ID    = "sPlayerID";
const char* CApp::K_FONT         = "sFont";
const char* CApp::K_ALIASES      = "sAliases";      // parancs ·lnevek

/// Main logger.
log4cpp::Category& Log = log4cpp::Category::getInstance ("kj");

void CApp::init_log ()
{
    std::remove (DEFAULT_LOG_FILE);
    // 1. instantiate an appender object. 
    // This one will append to a log file.
    log4cpp::Appender* appd_file = new log4cpp::FileAppender ("FileAppender", DEFAULT_LOG_FILE);
    // This one will write to display
    log4cpp::Appender* appd_display = new log4cpp::OstreamAppender ("OstreamAppender", &std::cout);

    // 2. Instantiate a layout object, which tells how to format the log.
    // Two layouts come already available in log4cpp unless you create your own.
    // BasicLayout includes a time stamp
    log4cpp::PatternLayout* layout_file = new log4cpp::PatternLayout ();
    layout_file->setConversionPattern ("%d %p %m%n");
    log4cpp::PatternLayout* layout_display = new log4cpp::PatternLayout ();
    layout_display->setConversionPattern ("%m%n");

    // 3. attach the layout object to the appender object
    appd_file->setLayout (layout_file);
    appd_display->setLayout (layout_display);

    // 5a. 
    // An Appender when added to a category becomes an additional output 
    // destination unless Additivity is set to false when it is false,
    // the appender added to the category replaces all previously existing appenders
    Log.setAdditivity (false);
    Log.setAppender (appd_file);
    Log.setAppender (appd_display);

    // 6. Set up the priority for the category and is given INFO priority
    // attempts to log DEBUG messages will fail

#ifdef __DEBUG__
    Log.setPriority (log4cpp::Priority::DEBUG);
#else
    Log.setPriority (log4cpp::Priority::INFO);
#endif
}

CApp::CApp ()
#ifdef GTKMM
    : Gtk::Window ()
#endif // GKTMM
{
    init_log ();
    Log.info (PROG_HEADER);
#if (LANG == ENG)
    Log.info (" * Starting...");
#endif // (LANG == ENG)
#if (LANG == HUN)
    Log.info (" * IndÌt·s...");
#endif // (LANG == HUN)
    load_config ();
    init_universe ();
#ifdef GTKMM
    init_window ();
#endif // GKTMM
}

CApp::~CApp ()
{
#ifdef GTKMM
    delete_window ();
#endif // GKTMM
    delete_universe ();
    save_config ();
#if (LANG == ENG)
    Log.info (" * Exiting...");
#endif // (LANG == ENG)
#if (LANG == HUN)
    Log.info (" * KilÈpÈs...");
#endif // (LANG == HUN)
}

void CApp::init_universe ()
{
    main_items = new CThingList;
    main_creatures = new CThingList;
    universe = new CThingList;

    // creating loaders
    CLoader<CItem> item_loader;
    CLoader<CCreature> creature_loader;
    CLoader<CMap> map_loader;

    if (smap.find (K_ITEM_CFG) == smap.end ())
    {
        smap[K_ITEM_CFG] = DEFAULT_ITEM_CFG;
    }
    if (smap.find (K_CREATURE_CFG) == smap.end ())
    {
        smap[K_CREATURE_CFG] = DEFAULT_CREATURE_CFG;
    }
    if (smap.find (K_MAP_CFG) == smap.end ())
    {
        smap[K_MAP_CFG] = DEFAULT_MAP_CFG;
    }

    std::string item_cfg = smap[K_ITEM_CFG];
    std::string creature_cfg = smap[K_CREATURE_CFG];
    std::string map_cfg = smap[K_MAP_CFG];
   
    // loading datas
    item_loader.load (*main_items, item_cfg);
    creature_loader.load (*main_creatures, creature_cfg);
    map_loader.load (*universe, map_cfg);

    // for finding
    CCreature *finder = new CCreature; 

    if (smap.find (K_PLAYER_ID) == smap.end ())
    {
        smap[K_PLAYER_ID] = DEFAULT_PLAYER_ID;
    }
    std::string player_id = smap[K_PLAYER_ID];
    player = dynamic_cast<CCreature*> (finder->find (player_id, *universe));
    delete finder;
    if (player == NULL)
    {
#if (LANG == ENG)
        Log.fatal (std::string ("'" + player_id + "' identifier not found! Exiting.").c_str ());
#endif // (LANG == ENG)
#if (LANG == HUN)
        Log.fatal (std::string ("'" + player_id + "' azonosÌtÛ nem lÈtezik! KilÈpek.").c_str ());
#endif // (LANG == HUN)
        exit (-1);
    }
    if (smap.find (K_ALIASES) == smap.end ())
    {
        smap[K_ALIASES] = DEFAULT_ALIASES;
    }
    std::string aliases = smap[K_ALIASES];
    player->set_aliases (aliases);
#ifdef GTKMM
    player->set_ostream (&player_os);
    player->set_spectator (true);
#endif // GKTMM
#ifdef __DEBUG__
    const std::string PLAYER2_ID = "farkas";
    player2 = dynamic_cast<CCreature*> (finder->find (PLAYER2_ID, *universe));
    if (player2 == NULL)
    {
#if (LANG == ENG)
        Log.fatal ("'" + PLAYER2_ID + "' identifier not found! Exiting.");
#endif // (LANG == ENG)
#if (LANG == HUN)
        Log.fatal ("'" + PLAYER2_ID + "' azonosÌtÛ nem lÈtezik! KilÈpek.");
#endif // (LANG == HUN)
        exit (-1);
    }
    player2->set_aliases (aliases);
#ifdef GTKMM
    player2->set_ostream (&player2_os);
    player2->set_spectator (true);
#endif // GKTMM
#endif  // __DEBUG__
    // Initialize random number generator.
    srand (static_cast<unsigned> (time (0)));
}

void CApp::delete_universe ()
{
    // creating loader (unloading is same, we need only one loader)
    CLoader<CThing> loader;

    smap[K_ALIASES] = player->get_aliases ();
    
    loader.unload (*universe);
    loader.unload (*main_creatures);
    loader.unload (*main_items);

    delete universe;
    delete main_creatures;
    delete main_items;
}

void CApp::load_config ()
{
    CConfig cfg;
    if (!cfg.open (CFG_FILE))
    {
        return;
    }

    while (cfg.ok ())
    {
        if (cfg.process ())
        {
            std::string section = cfg.get_section ();
            // ...
        }
        std::string var, val;
        var = cfg.get_variable ();
        val = cfg.get_value ();
        // ...
        if (var[0] == 's')
        {
            smap[var] = val;
        }
        if (var[0] == 'i')
        {
            imap[var] = atoi (val.c_str ());
        }
        if (var[0] == 'f')
        {
#if (LANG == HUN) && defined (GTKMM)
            // A magyarban tizedesvesszo van. A pontot automatikusan vesszore konvertaljuk.
            // Ha GTKMM definialva van az atof () a locale szerint mukodik.
            CTrans tr (".", ",");
            tr (val);
#endif
            fmap[var] = atof (val.c_str ());
        }
    }
}

void CApp::save_config ()
{
    std::ofstream f (CFG_FILE);
    if (!f)
    {
        std::ostringstream os;
        os << __INFO__ << "Can't open " << CFG_FILE << " file for writing!";
        Log.error (os.str ());
        return;
    }
    for (CSMapIt i = smap.begin (); i != smap.end (); i++)
    {
        f << i->first << " = \"" << i->second << "\"" << std::endl;
    }
    for (CIMapIt i = imap.begin (); i != imap.end (); i++)
    {
        f << i->first << " = " << i->second << std::endl;
    }
    for (CFMapIt i = fmap.begin (); i != fmap.end (); i++)
    {
        f << i->first << " = " << i->second << std::endl;
    }
    f.close ();
}

#ifdef GTKMM
void CApp::hide ()
{
    imap[K_WINDOW_W] = get_width ();
    imap[K_WINDOW_H] = get_height ();
    int x, y;
    get_position (x, y);
    imap[K_WINDOW_X] = x;
    imap[K_WINDOW_Y] = y;
    Gtk::Window::hide ();
}

void CApp::init_window ()
{
    set_title (Glib::convert (PROG_NAME, "utf8", "iso-8859-2"));
    set_size_request (640, 480);

    if (imap[K_WINDOW_W] && imap[K_WINDOW_H])
        set_default_size (imap[K_WINDOW_W], imap[K_WINDOW_H]);
    if (imap[K_WINDOW_X] && imap[K_WINDOW_Y])
        move (imap[K_WINDOW_X], imap[K_WINDOW_Y]);
    
    // Creating main vertical box
    main_vbox = new Gtk::VBox;
    //main_vbox->set_spacing (5);
    add (*main_vbox);

    // Create actions for menus and toolbars:
    action_group = Gtk::ActionGroup::create ();
 
    // File|New sub menu:
    /*action_group->add (Gtk::Action::create ("FileNewStandard", Gtk::Stock::NEW, "_New", "Create a new file"),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
 
    action_group->add (Gtk::Action::create ("FileNewFoo", Gtk::Stock::NEW, "New Foo", "Create a new foo"),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
 
    action_group->add (Gtk::Action::create ("FileNewGoo", Gtk::Stock::NEW, "_New Goo", "Create a new goo"),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
 
    */
    // File menu:
    action_group->add (Gtk::Action::create ("FileMenu", 
#if (LANG == ENG)
                "_File"
#endif
#if (LANG == HUN)
                "_F√°jl"
#endif
                ));
    //action_group->add (Gtk::Action::create ("FileNew", Gtk::Stock::NEW)); //Sub-menu.
    action_group->add (Gtk::Action::create ("FileQuit", Gtk::Stock::QUIT),
            sigc::mem_fun (*this, &CApp::on_quit));
 
    // Edit menu:
    action_group->add (Gtk::Action::create ("EditMenu", 
#if (LANG == ENG)
                "_Edit"
#endif
#if (LANG == HUN)
                "S_zerkeszt√©s"
#endif
                ));
    /*action_group->add (Gtk::Action::create ("EditCopy", Gtk::Stock::COPY),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
    action_group->add (Gtk::Action::create ("EditPaste", Gtk::Stock::PASTE),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
    action_group->add (Gtk::Action::create ("EditSomething", "Something"), Gtk::AccelKey ("<control><alt>S"),
            sigc::mem_fun (*this, &CApp::on_not_implemented));
    */
    /*action_group->add (Gtk::Action::create ("AutoScroll", 
#if (LANG == ENG)
                "Auto _scroll"
#endif
#if (LANG == HUN)
                "Automatikus _g√∂rget√©s"
#endif
                ), Gtk::AccelKey ("<control><alt>E"),
            sigc::mem_fun (*this, &CApp::on_auto_scroll));*/
    auto_scroll = Gtk::ToggleAction::create ("AutoScroll", 
#if (LANG == ENG)
                "Auto _scroll"
#endif
#if (LANG == HUN)
                "Automatikus _g√∂rget√©s"
#endif
            );
    if (imap.find (K_AUTO_SCROLL) == imap.end ())
        imap[K_AUTO_SCROLL] = DEFAULT_AUTO_SCROLL;
    auto_scroll->set_active (imap[K_AUTO_SCROLL]);
    action_group->add (auto_scroll,
            sigc::mem_fun (*this, &CApp::on_auto_scroll));
    action_group->add (Gtk::Action::create ("EditPreferences", 
#if (LANG == ENG)
                "Pr_eferences"
#endif
#if (LANG == HUN)
                "B_e√°ll√≠t√°sok"
#endif
                ), Gtk::AccelKey ("<control>E"),
            sigc::mem_fun (*this, &CApp::on_preferences));
    // Choices menu, to demonstrate Radio items
    /*action_group->add (Gtk::Action::create ("ChoicesMenu", "_Choices"));
    Gtk::RadioAction::Group group_userlevel;
    choice_one = Gtk::RadioAction::create (group_userlevel, "ChoiceOne", "One");
    action_group->add (choice_one,
            sigc::mem_fun (*this, &CApp::on_not_implemented));
    choice_two = Gtk::RadioAction::create (group_userlevel, "ChoiceTwo", "Two");
    action_group->add (choice_two,
            sigc::mem_fun (*this, &CApp::on_not_implemented));
    */
    // Help menu:
    action_group->add (Gtk::Action::create ("HelpMenu", 
#if (LANG == ENG)
                "_Help"
#endif
#if (LANG == HUN)
                "_S√∫g√≥"
#endif
                ));
    action_group->add (Gtk::Action::create ("HelpAbout", Gtk::Stock::HELP),
            sigc::mem_fun (*this, &CApp::on_about));
 
    ui_manager = Gtk::UIManager::create ();
    ui_manager->insert_action_group (action_group);
  
    add_accel_group (ui_manager->get_accel_group ());
 
    // Layout the actions in a menubar and toolbar:
    try
    {
        Glib::ustring ui_info = 
            "<ui>"
            "  <menubar name='MenuBar'>"
            "    <menu action='FileMenu'>"
            /*"      <menu action='FileNew'>"
            "        <menuitem action='FileNewStandard'/>"
            "        <menuitem action='FileNewFoo'/>"
            "        <menuitem action='FileNewGoo'/>"
            "      </menu>"
            "      <separator/>"*/
            "      <menuitem action='FileQuit'/>"
            "    </menu>"
            "    <menu action='EditMenu'>"
            "      <menuitem action='AutoScroll'/>"
            "      <menuitem action='EditPreferences'/>"
            /*"      <menuitem action='EditCopy'/>"
            "      <menuitem action='EditPaste'/>"
            "      <menuitem action='EditSomething'/>"
            "    </menu>"
            "    <menu action='ChoicesMenu'>"
            "      <menuitem action='ChoiceOne'/>"
            "      <menuitem action='ChoiceTwo'/>"*/
            "    </menu>"
            "    <menu action='HelpMenu'>"
            "      <menuitem action='HelpAbout'/>"
            "    </menu>"
            "  </menubar>"
            "  <toolbar name='ToolBar'>"
            //"    <toolitem action='FileNewStandard'/>"
            "    <toolitem action='FileQuit'/>"
            "  </toolbar>"
            "</ui>";

        ui_manager->add_ui_from_string (ui_info);
    }
    catch (const Glib::Error& ex)
    {
        std::ostringstream os;
#if (LANG == ENG)
        os << __INFO__ << "Building menus failed: " << ex.what ();
#endif
#if (LANG == ENG)
        os << __INFO__ << "A men¸ lÈtrehoz·sa nem siker¸lt: " << ex.what ();
#endif
        Log.fatal (os.str ());
        exit (-1);
    }

    // Get the menubar and toolbar widgets, and add them to a container widget:
    Gtk::Widget* menubar = ui_manager->get_widget ("/MenuBar");
    if (menubar)
        main_vbox->pack_start (*menubar, Gtk::PACK_SHRINK, 0);
 
    Gtk::Widget* toolbar = ui_manager->get_widget ("/ToolBar") ;
    if (toolbar)
        main_vbox->pack_start (*toolbar, Gtk::PACK_SHRINK, 0);

    // Creating tagtables
    Glib::RefPtr<Gtk::TextBuffer::TagTable> tag_table = Gtk::TextBuffer::TagTable::create ();

    Gtk::VBox *vbox = new Gtk::VBox;
    vbox->set_border_width (5);
    //vbox->set_spacing (5);
    main_vbox->pack_start (*vbox, Gtk::PACK_EXPAND_WIDGET, 0);
    
    // Creating text view
    text_view = new Gtk::TextView;
    text_view->set_wrap_mode (Gtk::WRAP_WORD);
    text_view->set_editable (false);
 
    // Creating tags
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag;
    tag = Gtk::TextBuffer::Tag::create ("default");
    tag->property_foreground () = "White";
    tag_table->add (tag);

    tag = Gtk::TextBuffer::Tag::create ("command");
    tag->property_foreground () = "DarkTurquoise";
    tag_table->add (tag);
    
    tag = Gtk::TextBuffer::Tag::create ("pcmd");
    tag->property_foreground () = "Turquoise";
    tag_table->add (tag);

    tag = Gtk::TextBuffer::Tag::create ("error");
    tag->property_foreground () = "OrangeRed";
    tag_table->add (tag);
    
    tag = Gtk::TextBuffer::Tag::create ("do");
    tag->property_foreground () = "Cyan";
    tag_table->add (tag);

    tag = Gtk::TextBuffer::Tag::create ("prompt");
    tag->property_foreground () = "CornflowerBlue";
    tag_table->add (tag);

    tag = Gtk::TextBuffer::Tag::create ("highlight");
    tag->property_foreground () = "Yellow";
    tag_table->add (tag);

    // Creating buffer for text view
    text_buffer = Gtk::TextBuffer::create (tag_table);
    text_buffer->signal_changed ().connect (sigc::mem_fun (*this, &CApp::on_textbuf));
    text_view->set_buffer (text_buffer);
    // Changing default colors
    Gdk::Color fg ("White"), bg ("Black");
    text_view->modify_text (Gtk::STATE_NORMAL, fg);
    text_view->modify_base (Gtk::STATE_NORMAL, bg);
    // Setting margins
    text_view->set_left_margin (1);
    text_view->set_right_margin (1);
    // Change default font throughout the widget
    if (smap.find (K_FONT) == smap.end ())
    {
        smap[K_FONT] = DEFAULT_FONT;
    }
    Pango::FontDescription font_desc (smap[K_FONT]);
    text_view->modify_font (font_desc);

    // Creating scrolled window and put text box into scrolled window
    scrolled_window = new Gtk::ScrolledWindow;
    scrolled_window->add (*text_view);
    scrolled_window->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
    //scrolled_window->set_border_width (5);
    vbox->pack_start (*scrolled_window, Gtk::PACK_EXPAND_WIDGET);

    // Creating tabs for text view
    // (used in cmd_inventory for listing)
    /*Pango::TabArray tabs (5, true);
    tabs.set_tab (0, Pango::TAB_LEFT, 40);
    tabs.set_tab (1, Pango::TAB_LEFT, 200);
    tabs.set_tab (2, Pango::TAB_LEFT, 250);
    tabs.set_tab (3, Pango::TAB_LEFT, 300);
    tabs.set_tab (4, Pango::TAB_LEFT, 350);
    text_view->set_tabs (tabs);*/
    
    // Creating text entry line
    entry = new Gtk::Entry;
    entry->signal_activate ().connect (sigc::mem_fun (*this, &CApp::on_entry));
    vbox->pack_start (*entry, Gtk::PACK_SHRINK, 5);

    // Creating buttons
    /*
    button_box = new Gtk::HButtonBox;
    button_box->set_border_width (5);
    button_box->set_spacing (5);
    button_box->set_layout (Gtk::BUTTONBOX_END);
    
    quit_btn = new Gtk::Button (Gtk::Stock::QUIT);
    quit_btn->signal_clicked ().connect(sigc::mem_fun (*this, &CApp::on_quit));
    //QuitBtn->signal_clicked ().connect (sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &CApp::quit), "QuitBtn"));
    button_box->pack_start (*quit_btn, Gtk::PACK_SHRINK);
    main_vbox->pack_start (*button_box, Gtk::PACK_SHRINK, 5);
    */

    show_all_children ();
    // Setting entry default
    set_focus (*entry);

    player->parser (CCreature::CMD_LOOK);
    if (!player_os.str ().empty ())
    {
        insert (text_buffer->end (), Glib::convert (player_os.str (), "utf8", "iso-8859-2"));
        // deleting buffer
        player_os.str ("");
    }
    // Call periodical
    Glib::signal_timeout ().connect (sigc::mem_fun (*this, &CApp::do_something), 500 /* msec */);
}

void CApp::delete_window ()
{
    delete entry;
    delete text_view;
    delete scrolled_window;
    delete main_vbox;
}

void CApp::update_text_buffer ()
{
    if (!player_os.str ().empty ())
    {
        insert (text_buffer->end (), Glib::convert (player_os.str (), "utf8", "iso-8859-2"));
        // deleting stream buffer
        player_os.str ("");
        text_buffer->place_cursor (text_buffer->end ());
    }
#ifdef __DEBUG__
    if (!player2_os.str ().empty ())
    {
        std::ostringstream os;
        os << "###### " << player2->get_name () << std::endl;
        CSplit split;
        CStringVector sv = split ("\\n", player2_os.str (), RE_DEFAULT | RE_NEWLINE | RE_NOTEOL);
        for (unsigned int i = 0; i < sv.size (); i++)
            os << "# " << sv[i] << std::endl;
        os << "######" << std::endl;

        insert (text_buffer->end (), Glib::convert (os.str (), "utf8", "iso-8859-2"));
        // deleting stream buffer
        player2_os.str ("");
        text_buffer->place_cursor (text_buffer->end ());
    }
#endif
}

Gtk::TextIter CApp::insert (const Gtk::TextIter& pos, const Glib::ustring& text)
{
    Gtk::TextIter p = pos;
    int start = 0;
    Glib::ustring tag, next_tag;
    // .- start
    // |        .- txt_start  .- txt_end
    // v        v             v
    // \x1B{tag}text text text\x1B{reset}
    for (unsigned int i = 0; i < text.size (); i++)
    {
        if (text[i] == C_ESCAPE)
        {
            unsigned int end = text.find_first_of (C_END, i);
            next_tag = text.substr (i + 2, end - i - 2);
            //std::cout << "***** tag: [" << tag << "] next_tag: [" << next_tag << "] *****" << std::endl;
            if (!tag.empty ())
            {
                //p = text_buffer->insert (p, text.substr (start, start - i));
                unsigned int txt_start, txt_end;
                Glib::ustring s;
                txt_start = text.find_first_of (C_END, start) + 1;
                txt_end = i;
                s = text.substr (txt_start, txt_end - txt_start);
                //std::cout << "===== s: [" << s << "] =====" << std::endl << std::endl;
                p = text_buffer->insert_with_tag (p, s, tag);
                i = text.find_first_of (C_END, txt_end);
                if (next_tag == C_RESET)
                {
                    start = i + 1;
                    tag = "";
                }
                else
                {
                    start = txt_end + 1;
                    tag = next_tag;
                }
            }
            else
            {
                start = i;
                tag = next_tag;
                if (next_tag == C_RESET)
                {
                    std::ostringstream os;
                    os << __INFO__ << "Internal error. C_RST without attribute changing.";
                    Log.error (os.str ());
                }
            }
        }
        else
        {
            if (tag.empty ())
            {
                p = text_buffer->insert (p, text.substr (i, 1));
            }
        }
    }
    return p;
}

void CApp::on_quit ()
{
    hide ();
}

void CApp::on_entry ()
{
    Glib::ustring buf = entry->get_text ();
    insert (text_buffer->end (), std::string (C_PROMPT) + "$ " + C_RST + C_PROMPT_CMD + buf + C_RST + "\n");
#ifdef __DEBUG__
    if (buf[0] == '!')
    {
        insert (text_buffer->end (), 
                Glib::convert (
                    show_universe (buf[1] >= '0' && buf[1] <= '9' ? buf[1] - '0' : 2), "utf8", "iso-8859-2")
                );
    }
    else if (buf[0] == '@')
    {
        insert (text_buffer->end (), 
                Glib::convert (
                    player->info (buf[1] >= '0' && buf[1] <= '9' ? buf[1] - '0' : 4), "utf8", "iso-8859-2")
                );
    }
    else if (buf[0] == ':')
    {
        buf = buf.substr (1, buf.size () - 1);
        player2->parser (Glib::convert (buf, "iso-8859-2", "utf8"));
    }
    else
#endif  // __DEBUG__
    if (buf == "q")
    {
        hide ();
    }
    else
    {
        player->parser (Glib::convert (buf, "iso-8859-2", "utf8"));
    }
    update_text_buffer ();
    entry->set_text ("");
}

void CApp::on_textbuf ()
{
    // Scrolling to the end of the buffer.
    // We must create a mark.
    if (imap[K_AUTO_SCROLL])
    {
        Glib::RefPtr<Gtk::TextBuffer::Mark> mark;
        mark = text_buffer->create_mark (text_buffer->end ());
        text_view->scroll_to (mark, 0);
    }
}

void CApp::on_auto_scroll ()
{
    imap[K_AUTO_SCROLL] = auto_scroll->get_active ();
}

void CApp::on_preferences ()
{
    Gtk::FontSelectionDialog *dlg = new Gtk::FontSelectionDialog;
    dlg->set_font_name (smap[K_FONT]);
    dlg->run ();
    Pango::FontDescription font_desc (dlg->get_font_name ());
    text_view->modify_font (font_desc);
    smap[K_FONT] = dlg->get_font_name ();
    delete dlg;
}

void CApp::on_about ()
{
    Gtk::MessageDialog *msg;
    msg = new Gtk::MessageDialog (*this, Glib::convert (ABOUT, "utf8", "iso-8859-2"));
    msg->run ();
    delete msg;
}

void CApp::on_not_implemented ()
{
    Gtk::MessageDialog *msg;
#if (LANG == ENG)
    msg = new Gtk::MessageDialog (*this, "Sorry. Not implemented.");
#endif // (LANG == ENG)
#if (LANG == HUN)
    msg = new Gtk::MessageDialog (*this, "Sajnos m√©g nincs implement√°lva.");
#endif // (LANG == HUN)
    msg->run ();
    delete msg;
}
#endif // GKTMM

bool CApp::do_something ()
{
    for (CThingListIt i = universe->begin (); i != universe->end (); i++)
    {
        (*i)->do_something ();
    }
#ifdef GTKMM
    update_text_buffer ();
#endif
    return true;
}

#ifndef GTKMM
void CApp::run ()
{
    player->set_ostream (&std::cout);
    player->set_spectator (true);
#ifdef __DEBUG__
    player2->set_ostream (&player2_os);
    player2->set_spectator (true);
#endif  // __DEBUG__
    
    player->parser (CCreature::CMD_LOOK);

    char buf[256]; // input buffer
    //std::string input;
    bool end = false;
    do
    {
        // writing prompt
        std::cout << C_PROMPT << "$ " << C_RST;
        std::cout.flush ();
        std::cout << C_CMD;
        /*input.clear ();
        char c = 0;
        do 
        {
            while (std::cin.rdbuf ()->in_avail () > 0 && c != '\n')
            {
                c = std::cin.get ();
                if (c != '\n')
                    input += c;
            }
            usleep (1000);
            if (player->parent != NULL)
                player->parent->do_something ();
        } while (c != '\n');*/
        // getting a line from standard input
        std::cin.getline (buf, sizeof (buf));
        std::cout << C_RST;
        if ((std::string) buf == "q" || std::cin.eof ())
        //if (input == "q" || std::cin.eof ())
        {
            // exiting if end of file or typed 'q'
#if (LANG == ENG)
            std::cout << "Bye." << std::endl;
#endif // (LANG == ENG)
#if (LANG == HUN)
            std::cout << "Cs·." << std::endl;
#endif // (LANG == HUN)
            end = true;
        }
        else
        {
#ifdef __DEBUG__
            if (buf[0] == '!')
            {
                std::cout << show_universe (buf[1] >= '0' && buf[1] <= '9' ? buf[1] - '0' : 2);
            }
            else if (buf[0] == '@')
            {
                std::cout << player->info (buf[1] >= '0' && buf[1] <= '9' ? buf[1] - '0' : 4) << std::endl;
            }
            else if (buf[0] == ':') // control second player
            {
                //buf = buf.substr (1, buf.size () - 1);
                buf[0] = ' ';
                player2->parser (buf);
            }
            else
#endif  // __DEBUG__
            // otherwise forwarding command to parser
            player->parser (buf);
#ifdef __DEBUG__
            std::string player2_out = player2_os.str ();
            if (player2_out.size () > 0) 
            {
                std::cout << "###### " << player2->get_name () << std::endl;
                CSplit split;
                CStringVector sv = split ("\\n", player2_out, RE_DEFAULT | RE_NEWLINE | RE_NOTEOL);
                for (unsigned int i = 0; i < sv.size (); i++)
                    std::cout << "# " << sv[i] << std::endl;
                std::cout << "######" << std::endl;
                player2_os.str ("");
            }
#endif  // __DEBUG__
        }
    } while (!end);
}
#endif // !GKTMM

std::string CApp::show_universe (int verbose_level)
{
    std::ostringstream os;
    os << "### MAIN ITEMS ###" << std::endl;
    for (CThingListIt i = main_items->begin (); i != main_items->end (); i++)
    {
        os << (*i)->info (verbose_level);
        os << std::endl;
    }
    
    os << "### MAIN CREATURES ###" << std::endl;
    for (CThingListIt i = main_creatures->begin (); i != main_creatures->end (); i++)
    {
        os << (*i)->info (verbose_level);
        os << std::endl;
    }
    
    os << "### MAPS ###" << std::endl;
    for (CThingListIt i = universe->begin (); i != universe->end (); i++)
    {
        os << (*i)->info (verbose_level);
        os << std::endl;
    }
    return os.str ();
}
// :set encoding=utf-8 fileencodings=:
