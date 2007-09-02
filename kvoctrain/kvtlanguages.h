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
    KVTLanguage() {}
    KVTLanguage(const QString &shortId, const QString &longId,
            const QString &pixmapFile,
            const QString& keyboardLayout = QString());

    ~KVTLanguage() {}

    const QString &shortId() const { return m_shortId; }
    const QString &longId() const { return m_longId; }
    const QString &pixmapFile() const { return m_pixmapFile; }
    const QString &keyboardLayout() const { return m_keyboardLayout; }

    bool operator==(const KVTLanguage&) const;
    KVTLanguage& operator=(const KVTLanguage& n);

private:
    QString m_shortId;
    QString m_longId;
    QString m_pixmapFile;
    QString m_keyboardLayout;
};


class KVTLanguageList : public QList<KVTLanguage>
{
public:

    void addLanguage(const QString &shortId, const QString &longId,
            const QString &pixmapFile,
            const QString& keyboardLayout = QString());

    void addLanguage(KVTLanguage language);

    int indexShortId(const QString &shortId) const;
    int indexLongId(const QString &longId) const;

    void read();
    void write();
};


#endif
