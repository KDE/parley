/***************************************************************************

    $Id$

                     common #defines for kvoctrain core

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 10:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/12/22 09:01:46  arnold
    fixed version string handling

    Revision 1.3  2001/12/13 18:39:52  arnold
    added phonetic alphabet stuff

    Revision 1.2  2001/11/25 11:11:23  arnold
    switch for inline edit, splitted kv_resource.h

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef KVOCTRAINCORE_H
#define KVOCTRAINCORE_H

///////////////////////////////////////////////////////////////////
// Config file entries

#define      CFG_MODIFIED             "Modified"
#define      CFG_FILENAME             "Filename"
#define      CFG_TITLE                "Title"

#define      CFG_APPEARANCE           "Appearance"
#define       CFG_XMLOPS_R             "XmlOps_R"
#define       CFG_XMLOPS_W             "XmlOps_W"
#define       CFG_XMLOPS_S             "XmlOps_S"
#define       CFG_IPA_FSIZE            "IPAFontSize"
#define       CFG_IPA_FFAMILY          "IPAFontFamily"
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
#define       CFG_SHOW_TOOLBAR         "ShowToolbar"
#define       CFG_SHOW_STATUSBAR       "ShowStatusbar"
#define       CFG_TOOLBAR_POS          "ToolBar_Pos"
#define       CFG_INLINE_EDIT          "EnableInlineEdit"

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
#define       CFG_ENTRYAUTOAPPLY       "EntryAutomaticApply"

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

#define MAX_LESSON       25
#define THRESH_LESSON    KV_MIN_GRADE
#define MAX_LANGSET      100

#define KVTLIB_DEBUG_AREA         12003
#define KVOCTRAIN_VERSION_STRING  "0.8.0"  // maybe split KVTLIB_VERSION/KVAPP_VERSION

#endif // KVOCTRAINCORE_H
