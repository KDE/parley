/***************************************************************************

                      language options for kvoctrain

    -----------------------------------------------------------------------

    begin        : Tue Apr 5 2005

    copyright    : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef LANGUAGEOPTIONS_H
#define LANGUAGEOPTIONS_H

#include <QList>
#include <QMenu>

#include "ui_languageoptionsbase.h"
#include "kvtlanguages.h"

class LanguageOptions : public QWidget, public Ui::LanguageOptionsBase
{
    Q_OBJECT
public:
    LanguageOptions(KVTLanguages & langset, QWidget* parent = 0);

    KVTLanguages getLangSet() const;

    struct Country
    {
        Country(const QString &c, const QList<int> l, const QString &p, int i)
                : country(c), langs(l), pixmap(p), id(i)
        {}
        Country()
        { }
        QString country;
        QList<int> langs;
        QString pixmap;
        int id;
    };

    struct Region
    {
        Region(const QString &reg) : region(reg)
        {}
        Region()
        {}
        QString region;
        QList<Country> countries;
    };

    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

signals:
    void widgetModified();

protected slots:
    void slotDeleteClicked();
    void slotNewClicked();
    void slotPixmapClicked();
    void slotLangChanged(const QString&);
    void slotShort2Changed(const QString&);
    void slotShortActivated(const QString&);
    void slotNewNameChanged(const QString&);
    void slotLangFromGlobalActivated(QAction *);
    void slotLangFromISO6391Activated(QAction *);
    void slotKeyboardLayoutChanged(const QString&);

private:
    bool setPixmap(QString pm);
    void enableLangWidgets();
    void loadCountryData();
    void loadISO6391Data();
    void createISO6391Menus();

    KVTLanguages global_langset;
    QMap<int, Country> countryIdMap;
    QMenu * m_kdeLanguagesMenu;
    QMenu * m_isoLanguagesMenu;
    KVTLanguages m_langSet;
    QString m_lastPix;
    bool m_hasChanged;
};

#endif
