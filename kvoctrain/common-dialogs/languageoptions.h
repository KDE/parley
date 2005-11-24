/***************************************************************************

                      language options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <kmenu.h>

#include "languageoptionsbase.h"
#include "langset.h"

class LanguageOptions : public QWidget, public Ui::LanguageOptionsBase
{
  Q_OBJECT
public:
  LanguageOptions(LangSet & langset, QWidget* parent = 0);
  virtual ~LanguageOptions();

  LangSet getLangSet () const;

  struct Country
  {
    Country(const QString& c, const QList<int> l, const QString& p, int i)
      : country(c), langs(l), pixmap(p), id(i) { }
    Country() { }
    QString country;
    QList<int> langs;
    QString pixmap;
    int id;
  };

  struct Region
  {
    Region (const QString& reg)
      : region(reg) {}
    Region() {}
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
  void slotLangFromGlobalActivated(int);
  void slotLangFromISO6391Activated(int);
  void slotKeyboardLayoutChanged(const QString&);

private:
  bool setPixmap(QString pm);
  void enableLangWidgets();
  void loadCountryData();
  void loadISO6391Data();
  void createISO6391Menus();

  LangSet global_langset;
  QMap<int, Country> countryIdMap;
  KMenu * langset_popup;
  KMenu * iso6391_popup;
  LangSet m_langSet;
  QString m_lastPix;
  bool m_hasChanged;
};

#endif

