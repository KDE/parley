/***************************************************************************

                      properties for a language

    -----------------------------------------------------------------------

    begin          : Wed Jun 30 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "kvtlanguages.h"

void KVTLanguages::addLanguage(QString _shortId, QString _longId, QString _pixmapFile, const QString& _shortId2, const QString& keyboardLayout)
{
  KVTLanguage def;
  def.shortId = _shortId;
  def.shortId2 = _shortId2;
  def.longId = _longId;
  def.pixmapFile = _pixmapFile;
  def.keyboardLayout = keyboardLayout;
  m_languages.append(def);
}


void KVTLanguages::clear()
{
  m_languages.clear();
}


void KVTLanguages::erase(int idx)
{
  if (idx >= 0 && idx < (int) m_languages.size())
    m_languages.erase (m_languages.begin() + idx);
}


QString KVTLanguages::shortId(int index) const
{
  if (index >= 0 && index < (int) m_languages.size())
    return m_languages[index].shortId;

  return QString();
}


QString KVTLanguages::shortId2(int index) const
{
  if (index >= 0 && index < (int) m_languages.size())
    return m_languages[index].shortId2;

  return QString();
}


QString KVTLanguages::longId(int index) const
{
  if (index >= 0 && index < (int) m_languages.size())
    return m_languages[index].longId;

  return QString();
}


QString KVTLanguages::pixmapFile(int index) const
{
  if (index >= 0 && index < (int) m_languages.size())
    return m_languages[index].pixmapFile;
  return "";
}

QString KVTLanguages::keyboardLayout(int index) const
{
  if (index >= 0 && index < (int) m_languages.size()) {
    return m_languages[index].keyboardLayout;
  }
  else {
    return QString();
  }
}


QString KVTLanguages::findShortId(const QString &_longId) const
{
  if (_longId.isEmpty())
    return "";

  for (int i = 0; i < (int) m_languages.size(); i++)
    if (_longId == m_languages[i].longId)
      return m_languages[i].shortId;

  return QString();
}


QString KVTLanguages::findLongId(const QString &_shortId) const
{
  if (_shortId.isEmpty())
    return "";

  for (int i = 0; i < (int) m_languages.size(); i++ )
    if (_shortId == m_languages[i].shortId  || _shortId == m_languages[i].shortId2)
      return m_languages[i].longId;

  return QString();
}


int KVTLanguages::indexShortId(QString _shortId) const
{
  if (_shortId.isEmpty())
    return -1;

   for (int i = 0; i < (int) m_languages.size(); i++) {
     if (m_languages[i].shortId == _shortId || m_languages[i].shortId2 == _shortId)
       return i;
   }
   return -1;
}


int KVTLanguages::indexLongId(QString _longId) const
{
  if (_longId.isEmpty())
    return -1;

   for (int i = 0; i < (int) m_languages.size(); i++) {
     if (m_languages[i].longId == _longId)
       return i;
   }
   return -1;
}


void KVTLanguages::setShortId(const QString & s, int index)
{
  if (index < (int) m_languages.size())
    m_languages[index].shortId = s;
}


void KVTLanguages::setShortId2(const QString & s, int index)
{
  if (index < (int) m_languages.size())
    m_languages[index].shortId2 = s;
}


void KVTLanguages::setLongId(const QString & s, int index)
{
  if (index < (int) m_languages.size())
    m_languages[index].longId = s;
}


void KVTLanguages::setPixmapFile(const QString & s, int index)
{
  if (index < (int) m_languages.size())
    m_languages[index].pixmapFile = s;
}

void KVTLanguages::setKeyboardLayout(const QString& layout, int index)
{
  if (index < (int) m_languages.size()) {
    m_languages[index].keyboardLayout = layout;
  }
}
