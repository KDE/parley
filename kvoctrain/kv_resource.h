/***************************************************************************

    $Id$

                  common #defines for kvoctrain gui

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/11/25 11:12:24  arnold
    splitted kv_resource.h

    Revision 1.8  2001/11/18 12:28:46  arnold
    provided menu entry for example files

    Revision 1.7  2001/11/16 19:50:26  arnold
    added submenu to set language to main menu

    Revision 1.6  2001/11/10 17:34:43  arnold
    fixed language property dialog page

    Revision 1.5  2001/11/02 10:18:31  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.4  2001/10/28 10:16:08  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.3  2001/10/22 06:39:21  waba
    Show a cute little marker to indicate marked entries.
    (Instead of using a bold type-face)

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:42:01  arnold
    import of version 0.7.0pre8 to kde-edu

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef RESSOURCE_H
#define RESSOURCE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

///////////////////////////////////////////////////////////////////
// kv_resource.h  -- contains macros used for commands

#define KV_EXTRA_COLS    2   // add col for lesson number

#define KV_COL_LESS      0   // col: lesson number
#define KV_COL_MARK      1   // col: mark
#define KV_COL_ORG       2   // col: original
#define KV_COL_TRANS     3   // col: first translation

///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES

// all values MUST remain below $FFFF

///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW                 10020
#define ID_FILE_OPEN                10030
#define ID_FILE_OPEN_XMP            10035
#define ID_FILE_OPEN_RECENT         10040

#define ID_FILE_SAVE                10050
#define ID_FILE_SAVE_AS             10060
#define ID_FILE_MERGE               10070

#define ID_FILE_QUIT                10100

#define ID_TSEARCH                  10200
#define ID_SEARCH_CLIP              10210
#define ID_TLESSON                  10220

///////////////////////////////////////////////////////////////////
// Edit-menu entries
#define ID_EDIT_UNDO                11010
#define ID_EDIT_REDO                11020
#define ID_EDIT_COPY                11030
#define ID_EDIT_PASTE               11050
#define ID_APPEND_ROW               11060
#define ID_EDIT_ROW                 11070
#define ID_REMOVE_ROW               11080
#define ID_SEL_ALL                  11090
#define ID_CLR_SEL                  11100
#define ID_SAVE_ROW                 11110

///////////////////////////////////////////////////////////////////
// View-menu entries                    


///////////////////////////////////////////////////////////////////
// Vocabulary-menu entries
#define ID_RANDOM_QUERY             13010
#define ID_APPEND_LANG              13020
#define ID_SET_LANG                 13025
#define ID_REMOVE_LANG              13030
#define ID_SEARCH                   13040
#define ID_SHOW_STAT                13050
#define ID_RAND_CREATE              13060
#define ID_RESUME_QUERY             13070
#define ID_CLEANUP                  13080
#define ID_DOC_PROPS                13090
#define ID_DOC_PROPS_LANG           13100
#define ID_RESUME_MULTIPLE          13110
#define ID_RESUME_ARTICLE           13120
#define ID_RESUME_VERB              13130

///////////////////////////////////////////////////////////////////
// Options-menu entries
#define ID_VIEW_TOOLBAR             14010
#define ID_VIEW_STATUSBAR           14020
#define ID_VIEW_INLINE              14030
#define ID_GENERAL_OPTIONS          14040
#define ID_QUERY_OPTIONS            14050
#define ID_SAVE_OPTIONS             14060

///////////////////////////////////////////////////////////////////
// Help-menu entries
#define ID_HELP                     1002

///////////////////////////////////////////////////////////////////
// Statusbar values
#define ID_STATUS_MSG               2001
#define ID_PRON_MSG                 2002
#define ID_REM_MSG                  2003
#define ID_TYPE_MSG                 2004

#define PREFIX_Type                 "T:"
#define PREFIX_Remark               "R:"
#define PREFIX_Pronunce             "P:"

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
     i18n("Abbreviation for T)ype of word",  PREFIX_Type)
     i18n("Abbreviation for R)emark",        PREFIX_Remark)
     i18n("Abbreviation for P)ronouncation", PREFIX_Pronunce)
#endif

//#define IDS_DEFAULT                 (doc && doc->isModified() ? " * Ready." : " Ready.")
#define IDS_DEFAULT                 i18n("Ready.")

///////////////////////////////////////////////////////////////////
// General application values
#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
     IDS_DEFAULT
#endif

///////////////////////////////////////////////////////////////////
// header-menu entries
#define IDH_NULL                        0
#define IDH_REMOVE                      1
#define IDH_APPEND                      2
#define IDH_START_QUERY                 3
#define IDH_RESET_GRADE                 4
#define IDH_CREATE_LESSON               5
#define IDH_SET_LANG                    6
#define IDH_SORT_COL_ALPHA              7
#define IDH_SORT_COL_NUM                8
#define IDH_START_MULTIPLE              9
#define IDH_START_ARTICLE              10
#define IDH_START_VERB                 11
#define IDH_START_ADJECTIVE            12
#define IDH_START_SYNONYM              13
#define IDH_START_ANTONYM              14
#define IDH_START_EXAMPLE              15
#define IDH_START_PARAPHRASE           16

///////////////////////////////////////////////////////////////////
// MACROS FOR THE CONNECT OF YOUR SIGNALS TO CORRESPONDENT SLOTS 
// IN YOUR MAIN IMPLEMENTATION OF MENUBAR AND TOOLBAR


///////////////////////////////////////////////////////////////////
// MENU CONNECTS
#define CONNECT_CMD(submenu)           connect(submenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));connect(submenu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)))

///////////////////////////////////////////////////////////////////
// TOOLBAR CONNECT  
#define CONNECT_TOOLBAR(ToolBar)        connect(ToolBar, SIGNAL(clicked(int)), SLOT(commandCallback(int)));connect(ToolBar, SIGNAL(highlighted(int,bool)), SLOT(statusCallback(int)))

///////////////////////////////////////////////////////////////////
// Create cases for entries and connect them with their functions
#define ON_CMD(id, cmd_class_function)   case id:cmd_class_function ;break;

///////////////////////////////////////////////////////////////////
// Create cases for entries and connect them to change statusBar entry
#define ON_STATUS_MSG(id, message)     case id:  slotStatusHelpMsg(message);break;

///////////////////////////////////////////////////////////////////
// other strings
//#define KVOCTRAIN_TITLE i18n("Vocabulary Trainer")
#define KVOCTRAIN_NAME "kvoctrain"

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
  KVOCTRAIN_TITLE
#endif

#define MAX_RECENTFILES  10

#define KVOCTRAIN_DEBUG_AREA  12002

#endif // RESOURCE_H

