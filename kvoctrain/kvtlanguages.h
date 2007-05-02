/***************************************************************************

                     properties for a language

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

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

#ifndef kvtlanguages_h
#define kvtlanguages_h

#include <QString>
#include <QList>

class KVTLanguage
{
public:
    KVTLanguage()
    {}
    QString shortId;
    QString shortId2;
    QString longId;
    QString pixmapFile;
    QString keyboardLayout;
};

class KVTLanguages
{
public:
    KVTLanguages()
    {}

    void addLanguage(const QString &shortId, const QString &longId, const QString &pixmapFile,
                     const QString& shortId2 = QString(), const QString& keyboardLayout = QString());

    int count() const
    {
        return m_languages.count();
    }
    void erase(int idx);
    void clear();

    QString shortId(int index) const;
    QString shortId2(int index) const;
    QString longId(int index) const;
    QString pixmapFile(int index) const;
    QString keyboardLayout(int index) const;

    int indexShortId(QString shortId) const;
    int indexLongId(QString longId) const;
    // doesn't make sense for keyboard layouts since there is no 1-to-1 relation to languages

    void setShortId(const QString & shortId, int index);
    void setShortId2(const QString & shortId2, int index);
    void setLongId(const QString & longId, int index);
    void setPixmapFile(const QString & pixmapFile, int index);
    void setKeyboardLayout(const QString & layout, int index);

    QString findShortId(const QString & longId)  const;
    QString findLongId(const QString & shortId) const;

private:
    QList<KVTLanguage> m_languages;
};


#endif // langset_h
