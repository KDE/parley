/***************************************************************************

    $Id$

                         debugging aids

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/09/13 13:08:13  ewald-arnold
    file moved

    Revision 1.4  2000/08/27 12:36:25  ewald-arnold
    added cvs keywords to sources


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef _EADEBUG_H
#define _EADEBUG_H

#include <kdebug.h>
#include <strstream.h>


#ifndef NDEBUG
#ifndef KDEBUG
# define KDEBUG( Level, Area, String ) kdebug( (Level), (Area), (String) );
# define KDEBUG1( Level, Area, String, Par1 ) kdebug( (Level), (Area), (String), (Par1) );
# define KDEBUG2( Level, Area, String, Par1, Par2 ) kdebug( (Level), (Area), (String), (Par1), (Par2) );
# define KDEBUG3( Level, Area, String, Par1, Par2, Par3 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3) );
# define KDEBUG4( Level, Area, String, Par1, Par2, Par3, Par4 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4) );
# define KDEBUG5( Level, Area, String, Par1, Par2, Par3, Par4, Par5 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5) );
# define KDEBUG6( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6) );
# define KDEBUG7( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7) );
# define KDEBUG8( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7), (Par8) );
# define KDEBUG9( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7), (Par8), (Par9) );
#endif
#ifndef KASSERT
# define KASSERT( Cond, Level, Area, String ) { if( !Cond ) kdebug( (Level), (Area), (String) ); }
# define KASSERT1( Cond, Level, Area, String, Par1 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1) ); }
# define KASSERT2( Cond, Level, Area, String, Par1, Par2 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2) ); }
# define KASSERT3( Cond, Level, Area, String, Par1, Par2, Par3 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3) ); }
# define KASSERT4( Cond, Level, Area, String, Par1, Par2, Par3, Par4 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4) ); }
# define KASSERT5( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5) ); }
# define KASSERT6( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6) ); }
# define KASSERT7( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7) ); }
# define KASSERT8( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7), (Par8) ); }
# define KASSERT9( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 ) { if( !Cond ) kdebug( (Level), (Area), (String), (Par1), (Par2), (Par3), (Par4), (Par5), (Par6), (Par7), (Par8), (Par9) ); }
#endif
#else
#ifndef KDEBUG
# define KDEBUG( Level, Area, String )
# define KDEBUG1( Level, Area, String, Par1 )
# define KDEBUG2( Level, Area, String, Par1, Par2 )
# define KDEBUG3( Level, Area, String, Par1, Par2, Par3 )
# define KDEBUG4( Level, Area, String, Par1, Par2, Par3, Par4 )
# define KDEBUG5( Level, Area, String, Par1, Par2, Par3, Par4, Par5 )
# define KDEBUG6( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6 )
# define KDEBUG7( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7 )
# define KDEBUG8( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8 )
# define KDEBUG9( Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 )
#endif
#ifndef KASSERT
# define KASSERT( Cond, Level, Area, String )
# define KASSERT1( Cond, Level, Area, String, Par1 )
# define KASSERT2( Cond, Level, Area, String, Par1, Par2 )
# define KASSERT3( Cond, Level, Area, String, Par1, Par2, Par3 )
# define KASSERT4( Cond, Level, Area, String, Par1, Par2, Par3, Par4 )
# define KASSERT5( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5 )
# define KASSERT6( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6 )
# define KASSERT7( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7 )
# define KASSERT8( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8 )
# define KASSERT9( Cond, Level, Area, String, Par1, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 )
#endif
#endif

#ifndef NDEBUG
#define EADEBUG( Level, Area, String ) \
       {                  \
         ostrstream str;  \
         s << String;     \
         kdebug( (Level), (Area), (str.data()) ); \
       }

#define EAASSERT( cond, Level, Area, String ) \
       { if (!(cond)) {     \
           ostrstream str;  \
           s << String;     \
           kdebug( (Level), (Area), (str.data()) ); \
         } \
       }
#else
#define EADEBUG( Level, Area, String )
#define EAASSERT( cond, Level, Area, String )
#endif // NDEBUG

/*
  enum DebugLevels {
  	KDEBUG_INFO=	0,
  	KDEBUG_WARN=	1,
  	KDEBUG_ERROR=	2,
  	KDEBUG_FATAL=	3
  };
*/


#endif // _EADEBUG_H
