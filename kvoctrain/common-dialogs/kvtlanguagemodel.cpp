/***************************************************************************

                             kvtlanguagemodel

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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

#include "kvtlanguagemodel.h"

#include <QFileInfo>

#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>

#include "../common-dialogs/languageoptions.h"

/*

    // temporary use of our locale as the global locale
    KLocale *lsave = KGlobal::locale();
    QString curr_lang = lsave->language();

    KLocale locale(QString::null);	//krazy:exclude=nullstrassign for old broken gcc
    locale.setLanguage(curr_lang);
    KGlobal::setLocale(&locale);

    QStringList codes = locale.allLanguagesList();
    codes.sort();

    // all two letter language codes
    foreach(QString code, codes) {
      QString languageName = locale.languageCodeToName(code);
      if (!languageName.isEmpty())
        global_langset.addLanguage(code, languageName, QString(), QString());
    }

    // localized region names are in in kde/share/locale/l10n/ *.desktop
    // eg centraleurope.desktop or southasia.desktop
    QString sub = QString::fromLatin1("l10n/");
    QStringList regionlist = KGlobal::dirs()->findAllResources("locale", sub + QString::fromLatin1("*.desktop")); // why use fromLatin1 ?
    regionlist.sort();

//kDebug() << regionlist;

    QMap<QString, Region> regions;

    foreach(QString region, regionlist) {
      QFileInfo fi(region);
      QString tag = fi.baseName();

      KConfig entry(region, KConfig::OnlyLocal);
      KConfigGroup group = entry.group(QString::fromLatin1("KCM Locale"));
      QString name = group.readEntry(QString::fromLatin1("Name"), i18n("without name"));

      regions.insert(tag, Region(name));
    }

    // add all languages to the list
    QStringList countrylist = locale.allCountriesList();
    countrylist.sort();

    int idx = 0;
    foreach(QString code, countrylist) {
      QString country = KGlobal::dirs()->findResource("locale", QString::fromLatin1("%1%2/entry.desktop").arg(sub).arg(code));
      KConfig entry(country, KConfig::OnlyLocal);
      KConfigGroup group = entry.group(QString::fromLatin1("KCM Locale"));
      QString name = group.readEntry(QString::fromLatin1("Name"), i18n("without name"));
      QString submenu = group.readEntry(QString::fromLatin1("Region"), QString());

      QStringList all_langs = group.readEntry(QString::fromLatin1("Languages"), QString()).split(",", QString::SkipEmptyParts);
      QList<int> langs;

      QString pixmap = country;
      int index = pixmap.lastIndexOf('/');
      pixmap.truncate(index);
      pixmap += "/flag.png";

      foreach(QString lang, all_langs) {
        // Treat ie "en_GB" and "en_USE" as "en" because the language list
        // only contains the first letters
        QString it = lang;
        if (it.indexOf("_"))
          it = it.left(it.indexOf("_"));

        int id = global_langset.indexShortId(it);
        if (id > 0) {
          langs.append(id);
          // Set the pixmap of the language to the flag of the first contry that
          // has the language as official language
          ///@todo I have turned off icons for the ISO menu as the result was frequently wrong.
          //if (global_langset.pixmapFile(id).isEmpty())
          //global_langset.setPixmapFile(pixmap, id);
        }
        //else
        //kDebug() << "Couldn't find the language for: " << *it;
      }

      int id = idx++;
      countryIdMap.insert(id, Country(name, langs, pixmap, id));
      regions[submenu].countries.append(countryIdMap[id]);
    }

    m_kdeLanguagesMenu = new QMenu();

    // To have it sorted by name
    QMap<QString, Region> regmap;
    foreach(Region reg, regions)
      regmap.insert(reg.region, reg);

    QAction *a;

    for (QMap<QString, Region>::Iterator it = regmap.begin(); it != regmap.end(); ++it) {
      QMenu *regpop = m_kdeLanguagesMenu->addMenu(it.key());
      connect(regpop, SIGNAL(triggered(QAction *)), this, SLOT(slotLangFromGlobalActivated(QAction *)));
      Region r = it.value();

      // To have it sorted by name
      QMap<QString, Country> countrymap;
      for (QList<Country>::Iterator it = r.countries.begin(); it != r.countries.end(); ++it) {
        countrymap.insert((*it).country, *it);
      }
      for (QMap<QString, Country>::Iterator it = countrymap.begin(); it != countrymap.end(); ++it) {
          a = regpop->addAction(QPixmap(it.value().pixmap), it.key());
          a->setData(it.value().id);
      }
    }

    // restore the old global locale
    KGlobal::setLocale(lsave);
}
*/



KVTLanguageModel::KVTLanguageModel(QObject *parent)
 : QStandardItemModel(parent)
{
    setHorizontalHeaderItem(0, new QStandardItem(i18n("Language")));
    QStandardItem *parentItem = invisibleRootItem();

    // localized region names are in in kde/share/locale/l10n/*.desktop
    // eg centraleurope.desktop or southasia.desktop
//    QStringList regionlist = KGlobal::dirs()->findAllResources("locale", "l10n/*.desktop");
  //  regionlist.sort();

//kDebug() << regionlist;

//    QMap<QString, Region> regions;
/*
    foreach(QString region, regionlist) {
        QFileInfo fi(region);
        QString tag = fi.baseName();

        KConfig entry(region, KConfig::OnlyLocal);
        KConfigGroup group = entry.group(QString::fromLatin1("KCM Locale"));
        QString name = group.readEntry(QString::fromLatin1("Name"), i18n("without name"));

//        regions.insert(tag, Region(name));
        QStandardItem *item = new QStandardItem(name);
        parentItem->appendRow(item);
    }
  */  
    KLocale *locale = KGlobal::locale(); //(QString::null);	//krazy:exclude=nullstrassign for old broken gcc
    QStringList codes = locale->allLanguagesList();
    //codes.sort();
    
    foreach (QString code, codes){
        /*
        QString pixmap = KGlobal::dirs()->findResource("locale", 
            "l10n/" + code + "/entry.desktop");
        pixmap.truncate(pixmap.lastIndexOf('/'));
        pixmap += "/flag.png";
        //kDebug() << code << ": " << pixmap;
        */
        const QString &languageName = locale->languageCodeToName(code);
        if(! languageName.isEmpty()) {
            QStandardItem *languageItem = new QStandardItem(languageName);
            languageItem->setData(code, LanguageTwoLetterRole);
            languageItem->setEditable(false);
            //languageItem->setIcon(QIcon(pixmap));
            //QStandardItem *item = new QStandardItem(code);
            QList<QStandardItem *> list;
            list << languageItem;
            parentItem->appendRow(list);
        }
    }

/*
    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("language %0").arg(i));
        parentItem->appendRow(item);
        parentItem = item;
    }
*/
}




#include "kvtlanguagemodel.moc"
