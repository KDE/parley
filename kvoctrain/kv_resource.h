/***************************************************************************

                  common #defines for kvoctrain gui

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

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

#ifndef I18N_NOOP2
#define I18N_NOOP2(context, str) context, str
#endif

#define PREFIX_Type                 I18N_NOOP2("Abbreviation for T)ype of word", "T:")
#define PREFIX_Remark               I18N_NOOP2("Abbreviation for R)emark","R:")
#define PREFIX_Pronunce             I18N_NOOP2("Abbreviation for P)ronouncation","P:")

//#define IDS_DEFAULT                 (doc && doc->isModified() ? " * Ready." : " Ready.")
#define IDS_DEFAULT                 I18N_NOOP("Ready.")

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
// other strings
//#define KVOCTRAIN_TITLE i18n("Vocabulary Trainer")
#define KVOCTRAIN_NAME "kvoctrain"

#define MAX_RECENTFILES  10

#define KVOCTRAIN_DEBUG_AREA  12002

#endif // RESOURCE_H

