/***************************************************************************

    $Id$

                     common #defines for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
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

#define KV_EXTRA_COLS    1   // add col for lesson number

#define KV_COL_LESS      0   // col: lesson number
#define KV_COL_ORG       1   // col: original
#define KV_COL_TRANS     2   // col: first translation

///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES

// all values MUST remain below $FFFF

///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW                 10020
#define ID_FILE_OPEN                10030
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
#define ID_GENERAL_OPTIONS          14030
#define ID_QUERY_OPTIONS            14040
#define ID_SAVE_OPTIONS             14050

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
     i18n(PREFIX_Type)
     i18n(PREFIX_Remark)
     i18n(PREFIX_Pronunce)
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
// Config file entries

#define      CFG_MODIFIED             "Modified"
#define      CFG_FILENAME             "Filename"
#define      CFG_TITLE                "Title"

#define      CFG_APPEARANCE           "Appearance"
#define       CFG_XMLOPS_R             "XmlOps_R"
#define       CFG_XMLOPS_W             "XmlOps_W"
#define       CFG_XMLOPS_S             "XmlOps_S"
#define       CFG_FSIZE                "FontSize"
#define       CFG_FFAMILY              "FontFamily"
#define       CFG_FCHARSET             "FontCharSet"
#define       CFG_GC_USE               "UseGradeCol"
#define       CFG_GCOL0                "GradeCol0"
#define       CFG_GCOL1                "GradeCol1"
#define       CFG_GCOL2                "GradeCol2"
#define       CFG_GCOL3                "GradeCol3"
#define       CFG_GCOL4                "GradeCol4"
#define       CFG_GCOL5                "GradeCol5"
#define       CFG_GCOL6                "GradeCol6"
#define       CFG_GCOL7                "GradeCol7"

#define      CFG_GENERAL              "General"
#define       CFG_LASTFILE             "LastFile"
#define       CFG_SEPARATOR            "Separator"
#define       CFG_PASTEORDER           "PasteOrder"
#define       CFG_USECURRENT           "UseCurrent"
#define       CFG_ENTRIESLESSON        "EntriesPerLesson"
#define       CFG_BACKTIME             "BackupTime"
#define       CFG_CUR_ROW              "CurrentRow"
#define       CFG_CUR_COL              "CurrentCol"
#define       CFG_RECENT               "RecentFiles"
#define       CFG_HEADER_RESIZER       "HeaderResizer"
#define       CFG_FIRST_TIME           "FirstTime"
#define       CFG_SMART_APPEND         "SmartAppend"
#define       CFG_AUTOSAVEOPT          "AutoSaveOptions"

#define      CFG_WINPROP              "WindowProperties"
#define       CFG_WINWIDTH             "WindowWidth"
#define       CFG_WINHEIGHT            "WindowHeight"
#define       CFG_LESSONSIZE           "LessonSize"

#define      CFG_QUERYPROP            "QueryProperties"
#define       CFG_MAXTIME_PER          "MaxTimePer"
#define       CFG_QUERYMODE            "QueryMode"
#define       CFG_SWAP_DIR             "SwapDir"
#define       CFG_EXPIRE               "Expire"
#define       CFG_BLOCK                "Block"
#define       CFG_DEFTRANS             "DefaultTranslation"
#define       CFG_DEFORG               "DefaultOriginal"
#define       CFG_QUERYTIMEOUT         "QueryTimeout"
#define       CFG_SHOWCOUNTER          "Showcounter"

#define      CFG_NUMLANGSET           "NumLangSet"
#define       CFG_L_SHORT              "ShortId"
#define       CFG_L_SHORT2             "Short2Id"
# define      CFG_L_LANG               "LongId_KDE2"
# define      CFG_L_PIXMAP             "Pixmap_KDE2"
#define       CFG_L_FONT               "Font"
#define       CFG_L_STDFONT            "SpecialFont"

#define      CFG_QUERYMANAG           "QueryManager"
#define       CFG_QM_DATE_ITEM         "DateItem"
#define       CFG_QM_DATE_COMP         "DateComp"
#define       CFG_QM_QUERY_ITEM        "QueryItem"
#define       CFG_QM_QUERY_COMP        "QueryComp"
#define       CFG_QM_BAD_ITEM          "BadItem"
#define       CFG_QM_BAD_COMP          "BadComp"
#define       CFG_QM_TYPE_ITEM         "TypeItem"
#define       CFG_QM_TYPE_COMP         "TypeComp"
#define       CFG_QM_GRADE_ITEM        "GradeItem"
#define       CFG_QM_GRADE_COMP        "GradeComp"
#define       CFG_QM_LESSON_ITEM       "LessonItem"
#define       CFG_QM_LESSON_COMP       "LessonComp"
#define       CFG_QM_BLOCK_ITEM        "BlockItem"
#define       CFG_QM_EXPIRE_ITEM       "ExpireItem"

#define      CFG_QUERY_PRESETTING     "QueryPreSettings"
#define       CFG_QP_NUM               "NumPreSetting"
#define       CFG_QP_NAME              "Name"
#define       CFG_QP_THRESH            "Threshhold"
#define       CFG_QP_BLOCK             "Blocking"
#define       CFG_QP_QUERY             "Query"

///////////////////////////////////////////////////////////////////
// other strings
//#define KVOCTRAIN_TITLE i18n("Vocabulary trainer")
#define KVOCTRAIN_NAME "kvoctrain"

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
  KVOCTRAIN_TITLE
#endif

#define KVTML_EXT        "kvtml"
#define VT5_LEX_EXT      "lex"
#define QVOCAB_EXT       "qvo"
#define VCB_EXT          "vocab"
#define KVL_EXT          "vl"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"

#define TAB_REP          "%t"
#define SPC_REP          "%ws"

#define VCB_SEPARATOR    "__"

#define MAX_RECENTFILES  10
#define MAX_LESSON       25
#define THRESH_LESSON    KV_MIN_GRADE
#define MAX_LANGSET      100

#endif // RESOURCE_H

