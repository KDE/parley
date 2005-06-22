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
#define ID_RESUME_QUERY             13070
#define ID_RESUME_MULTIPLE          13110


#endif // RESOURCE_H

