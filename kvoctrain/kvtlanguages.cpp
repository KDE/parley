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

#include "prefs.h"
#include "languagesettings.h"

KVTLanguage::KVTLanguage(const QString & shortId, const QString & longId, const QString & pixmapFile, const QString & shortId2, const QString & keyboardLayout)
                       : m_shortId(shortId), m_shortId2(shortId2), m_longId(longId), m_pixmapFile(pixmapFile), m_keyboardLayout(keyboardLayout)
{
}


bool KVTLanguage::operator ==(const KVTLanguage &n) const
{
  if (n.shortId() == m_shortId &&
      n.shortId2() == m_shortId2 &&
      n.longId() == m_longId &&
      n.pixmapFile() == m_pixmapFile &&
      n.keyboardLayout() == m_keyboardLayout) 
      return true;
  return false;
}


KVTLanguage & KVTLanguage::operator =(const KVTLanguage & n)
{
    m_shortId  = n.shortId();
    m_shortId2 = n.shortId2();
    m_longId  = n.longId();
    m_pixmapFile = n.pixmapFile();
    m_keyboardLayout = n.keyboardLayout();
    return *this;
}


void KVTLanguageList::addLanguage(const QString &_shortId, const QString &_longId, const QString &_pixmapFile, const QString& _shortId2, const QString& keyboardLayout)
{
    KVTLanguage language = KVTLanguage(_shortId, _longId, _pixmapFile, _shortId2, keyboardLayout);
    addLanguage(language);
}


void KVTLanguageList::addLanguage(KVTLanguage language)
{
    if (!this->contains(language))
        this->append(language);

    kDebug() << "Adding language: " << language.shortId() << " " << language.shortId2() << " " << language.longId() << " " << language.pixmapFile() << endl;
}


int KVTLanguageList::indexShortId(const QString &_shortId) const
{
    if (_shortId.isEmpty())
        return -1;

    for (int i = 0; i < count(); i++) {
        if (this->at(i).shortId() == _shortId || this->at(i).shortId2() == _shortId)
            return i;
    }
    return -1;
}


int KVTLanguageList::indexLongId(const QString &_longId) const
{
    if (_longId.isEmpty())
        return -1;

    for (int i = 0; i < count(); i++) {
        if (this->at(i).longId() == _longId)
            return i;
    }
    return -1;
}


void KVTLanguageList::read()
{
    clear();
    int ls = Prefs::numLangSet();
    for (int i = 0 ; i < ls; i++) {
        LanguageSettings languageSettings(QString::number(i));
        languageSettings.readConfig();

        QString shortId = languageSettings.shortId();
        if (shortId.simplified().length() == 0) {
            shortId.setNum(i);
            shortId.prepend("id");
        }

        QString longId = languageSettings.longId();
        if (longId.simplified().length() == 0) {
            longId.setNum(i);
            longId.prepend("ident");
        }

        addLanguage(shortId, longId, languageSettings.pixmapFile(), languageSettings.short2Id(), languageSettings.keyboardLayout());
    }
}


void KVTLanguageList::write()
{
    Prefs::setNumLangSet(count());
    for (int i = 0 ; i < count(); i++) {
        LanguageSettings languageSettings(QString::number(i));
        languageSettings.setShortId(this->at(i).shortId());
        languageSettings.setShort2Id(this->at(i).shortId2());
        languageSettings.setLongId(this->at(i).longId());
        languageSettings.setPixmapFile(this->at(i).pixmapFile());
        languageSettings.setKeyboardLayout(this->at(i).keyboardLayout());
        languageSettings.writeConfig();
    }
}

