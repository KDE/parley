/***************************************************************************

    $Id$

                     properties for a language

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

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

#include <vector.h>
#include <qstring.h>
#include <qfont.h>


struct SpecFont_t {
  QFont font;
  bool  specfont;

  QFont limitedFont (int max=24, int min=5);
};


class LangSet
{
 public:

   LangSet () {}

   void addSet (QString shortId, QString longId, QString PixMapFile, QFont font, bool specfont);
   void addSet (QString shortId, QString shortId2, QString longId, QString PixMapFile, QFont font, bool specfont);

   unsigned int size () const { return langs.size(); }
   void erase (int idx);

   QString shortId (int index) const;
   QString shortId2 (int index) const;
   QString longId (int index) const;
   QString PixMapFile (int index) const;
   void Font (int index, QFont& font, bool &specfont) const;

   int indexShortId (QString shortId) const;
   int indexLongId (QString longId) const;
   int indexPixMapFile (QString PixMapFile) const;

   void setShortId (QString shortId, int index);
   void setShortId2 (QString shortId2, int index);
   void setLongId (QString longId, int index);
   void setPixMapFile (QString PixMapFile, int index);
   void setFont (QFont font, bool specfont, int index);

   QString findShortId (const QString &longId)  const;
   QString findLongId  (const QString &shortId) const;

 private:

   struct LangDef {
     QString shortId,
             shortId2,
             longId,
             PixMapFile;
     QFont   Font;
     bool    specfont;
   };

   vector<LangDef> langs;
};


#endif // langset_h
