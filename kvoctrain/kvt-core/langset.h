/***************************************************************************

                     properties for a language

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#ifndef langset_h
#define langset_h

#include <vector>
using namespace std;

#include <qstring.h>


class LangSet
{
 public:

   LangSet () {}

   void addSet (QString shortId, QString longId, QString PixMapFile,
		const QString& shortId2 = QString::null,
		const QString& keyboardLayout = QString::null);

/*    void addSet (QString shortId, QString shortId2, QString longId, */
/* 		QString PixMapFile,  */
/* 		const QString& keyboardLayout = QString::null); */

   void appendSet(const LangSet &set);

   unsigned int size () const { return langs.size(); }
   void erase (int idx);
   void clear ();

   QString shortId (int index) const;
   QString shortId2 (int index) const;
   QString longId (int index) const;
   QString PixMapFile (int index) const;
   QString keyboardLayout (int index) const;

   int indexShortId (QString shortId) const;
   int indexLongId (QString longId) const;
   int indexPixMapFile (QString PixMapFile) const;
   // doesn't make sense for keyboard layouts since there is no 1-to-1 relation to languages

   void setShortId (const QString & shortId, int index);
   void setShortId2 (const QString & shortId2, int index);
   void setLongId (const QString & longId, int index);
   void setPixMapFile (const QString & PixMapFile, int index);
   void setKeyboardLayout(const QString & layout, int index);

   QString findShortId (const QString & longId)  const;
   QString findLongId  (const QString & shortId) const;

 private:

   struct LangDef {
     QString shortId,
             shortId2,
             longId,
       PixMapFile,
       keyboardLayout;
   };

   vector<LangDef> langs;
};


#endif // langset_h
