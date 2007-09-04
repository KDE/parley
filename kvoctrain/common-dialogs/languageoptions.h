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
    /** Constructor - creates the widget of Settings->KVocTrain->Languages */
    LanguageOptions(KVTLanguageList & langset, QWidget* parent);

    /** Return the languages m_langSet */
    KVTLanguageList getLangSet() const;

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


    /**
     * Status of m_hasChanged variable, reporting if the language list changed.
     * @return m_hasChanged
     */
    bool hasChanged();
    /**
     *
     * @return true, always
     */
    bool isDefault();

    /**
     * Writes the changes to the prefs.
     */
    void updateSettings();

signals:
    void widgetModified();

protected slots:
    /** Remove a language */
    void slotDeleteClicked();
    /** A click on the ADD button - add the new entry to the combo box. */
    void slotNewClicked();
    void slotPixmapClicked();

    /**
     * The long name (lineEdit) changed. This will set it in m_langSet
     * @param s is the new name for the language
     */
    void slotLangChanged(const QString&);

    /** The combo box selection changed.
     * @param _id was selected
    */
    void slotShortActivated(const QString&);
    /** Activate the ADD button, if @p lang has at least two chars.
     * This is connected to the line edit at the bottom of the dialog.
     * @param lang is the new language name
    */
    void slotNewNameChanged(const QString& lang);

    /**
     * A language was added by the button "Add Language Data From KDE Database"
     * @param act
     */
    void slotLangFromGlobalActivated(QAction *);

    /**
     * A language was added by the button "Add Language Data From ISO639-1"
     * @param act
     */
    void slotLangFromISO6391Activated(QAction *);

    /** Set the keyboard layout for the currently selected language. */
    void slotKeyboardLayoutChanged(const QString&);

private:
    /**
     * Add a nice little icon to the language.
     * @param pm is the pixmap
     * @return @c true if successfully changed
     */
    bool setPixmap(const QString &pm);

    /** If a short name is present, enable the other widgets. */
    void enableLangWidgets();

    /**
     * Creates the data structure that can be added by the button using KLocale.
     */
    void loadCountryData();

    /**
     * Create the menu for the ISO6391 button.
     */
    void createISO6391Menus();

    KVTLanguageList global_langset;
    QMap<int, Country> countryIdMap;
    QMenu * m_kdeLanguagesMenu;
    QMenu * m_isoLanguagesMenu;
    /// The languages are kept here:
    KVTLanguageList m_langSet;
    QString m_lastPix;
    bool m_hasChanged;
};

#endif
