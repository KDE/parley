/***************************************************************************

    $Id$

           provide minimal compatibility for Qt2.x and KDE2.x

    -----------------------------------------------------------------------


    begin                : Sun Dec 19 11:26:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************/

/************x***************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#define EA_QT2x
#define EA_KDE2x

#ifndef COMPAT_2X_H
#define COMPAT_2X_H


# include "qnamespace.h"
# define  EA_QtNS(x)  Qt::x
# define  EA_LOCAL(x) x.local8Bit()
# define  EA_ENCODEFILE(x) QFile::encodeName(x)
# define  EA_STRICMP(a,b) QString::compare(a.upper(), b.upper())

#include <klocale.h>
#include <kstddirs.h>
#include  <kconfig.h>

# define  EA_KappGetConfig           KApplication::kApplication()->config()
# define  EA_KDEHTMLDIR(path, file)  locate( "html", file)
# define  EA_KDEDATADIR(path, file)  locate( "data", file)
# define  EA_I18N(context, msg)      i18n(context, msg)

#endif  // COMPAT_2X_H
