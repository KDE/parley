/***************************************************************************

    $Id$

                      properties for a language

    -----------------------------------------------------------------------

    begin                : Wed Jun 30 20:50:53 MET 1999
                                           
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


#include "langset.h"
#include <iostream.h>


QFont SpecFont_t::limitedFont (int max, int min)
{
   QFont retfont = font;
   if (retfont.pointSize() > max)
     retfont.setPointSize(max);

   if (retfont.pointSize() < min)
     retfont.setPointSize(min);

   return retfont;
}


void LangSet::addSet (QString _shortId, QString _longId,
                      QString _PixMapFile, QFont font, bool specfont)
{
  LangDef def;
  def.shortId = _shortId;
  def.shortId2 = "";
  def.longId = _longId;
  def.PixMapFile = _PixMapFile;
  def.Font = font;
  def.specfont = specfont;
  langs.push_back (def);
}


void LangSet::addSet (QString _shortId, QString _shortId2, QString _longId,
                      QString _PixMapFile, QFont font, bool specfont)
{
  LangDef def;
  def.shortId = _shortId;
  def.shortId2 = _shortId2;
  def.longId = _longId;
  def.PixMapFile = _PixMapFile;
  def.Font = font;
  def.specfont = specfont;
  langs.push_back (def);
}


void LangSet::erase (int idx)
{
  if (idx >= 0 && idx < (int) langs.size() )
    langs.erase (&langs[idx], &langs[idx+1]);
}


QString LangSet::shortId (int index) const
{
  if (index >= 0 && index < (int) langs.size() )
    return langs[index].shortId;

  return "";
}


QString LangSet::shortId2 (int index) const
{
  if (index >= 0 && index < (int) langs.size() )
    return langs[index].shortId2;

  return "";
}


QString LangSet::longId (int index) const
{
  if (index >= 0 && index < (int) langs.size() )
    return langs[index].longId;

  return "";
}


QString LangSet::PixMapFile (int index) const
{
  if (index >= 0 && index < (int) langs.size() )
    return langs[index].PixMapFile;
  return "";
}


void LangSet::Font (int index, QFont& font, bool &specfont) const
{
  if (index >= 0 && index < (int) langs.size() ) {
    font = langs[index].Font;
    specfont = langs[index].specfont;
  }
  else {
    specfont = false;
    font = QFont();
  }
}


QString LangSet::findShortId (const QString &_longId) const
{
  if (_longId.isEmpty())
    return "";

  for (int i = 0; i < (int) langs.size(); i++ )
    if (_longId == langs[i].longId)
      return langs[i].shortId;

  return "";
}


QString LangSet::findLongId (const QString &_shortId) const
{
  if (_shortId.isEmpty())
    return "";

  for (int i = 0; i < (int) langs.size(); i++ )
    if (   _shortId == langs[i].shortId
        || _shortId == langs[i].shortId2)
      return langs[i].longId;

  return "";
}


int LangSet::indexShortId (QString _shortId) const
{
  if (_shortId.isEmpty())
    return -1;

   for (int i = 0; i < (int) langs.size(); i++) {
     if (  langs[i].shortId == _shortId
         ||langs[i].shortId2 == _shortId )
       return i;
   }
   return -1;
}


int LangSet::indexLongId (QString _longId) const
{
  if (_longId.isEmpty())
    return -1;

   for (int i = 0; i < (int) langs.size(); i++) {
     if (langs[i].longId == _longId)
       return i;
   }
   return -1;
}


int LangSet::indexPixMapFile (QString PixMapFile) const
{
   int id = -1;
   for (int i = 0; i < (int) langs.size(); i++)
     if (langs[i].PixMapFile == PixMapFile)
       id = i;
   return id;
}

void LangSet::setShortId (QString s, int index)
{
  if (index < (int) langs.size() )
    langs[index].shortId = s;
}


void LangSet::setShortId2 (QString s, int index)
{
  if (index < (int) langs.size() )
    langs[index].shortId2 = s;
}


void LangSet::setLongId (QString s, int index)
{
  if (index < (int) langs.size() )
    langs[index].longId = s;
}


void LangSet::setPixMapFile (QString s, int index)
{
  if (index < (int) langs.size() )
    langs[index].PixMapFile = s;
}


void LangSet::setFont (QFont f, bool specfont, int index)
{
  if (index < (int) langs.size() ) {
    langs[index].Font = f;
    langs[index].specfont = specfont;
  }
}

