/***************************************************************************

                      language options for kvoctrain

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

//based on code copyright (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

#include "languageoptions.h"

#include "prefs.h"
#include "languagesettings.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFileInfo>
#include <QPixmap>
#include <QByteArray>
#include <QMenu>
#include <QtDBus>

#include <klocale.h>
#include <kimageio.h>
#include <kcombobox.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kmessagebox.h>


LanguageOptions::LanguageOptions(KEduVocDocument* doc, QWidget* parent) : QWidget(parent)
{
    m_doc = doc;

    setupUi(this);

    m_kdeLanguagesMenu = 0;
    m_isoLanguagesMenu = 0;

    // the locale button
    connect(d_shortName, SIGNAL(activated(const QString&)), this, SLOT(slotShortActivated(const QString&)));


    connect(d_kblayout, SIGNAL(activated(const QString&)), this, SLOT(slotKeyboardLayoutChanged(const QString&)));

    connect(iconComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotIconChanged(int)));


    // Load the languages first, then the countries and create the
    // menus for the languages last, so they will have flags
    loadCountryData();

//     b_lang_kde->setMenu(m_kdeLanguagesMenu);

//     createISO6391Menus();
//     b_lang_iso1->setMenu(m_isoLanguagesMenu);

    // This is actually the ADD button.
//     b_langNew->setEnabled(false); // activate after data is entered

//     foreach(const KVTLanguage &language, m_langSet)
//         d_shortName->addItem(language.shortId());

    for ( int i=0; i<m_doc->identifierCount(); i++) {
        d_shortName->addItem(m_doc->identifier(i).locale());
    }

    enableLangWidgets();

    if (d_shortName->count()) {
        d_shortName->setCurrentIndex(0);

        kDebug() << "Config for " << d_shortName->currentText();

        // read config
        LanguageSettings currentSettings(d_shortName->currentText());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();

//         kDebug() << "Icon for " << d_shortName->currentText() << " is " << icon;
//         if (!icon.isEmpty()) {
//             b_langPixmap->setText(QString());
//             b_langPixmap->setIcon(QIcon(icon));
//         } else {
//             b_langPixmap->setText(i18n("No Picture Selected"));
//             b_langPixmap->setIcon(QIcon());
//         }
    }
    m_hasChanged = false;
}


void LanguageOptions::slotDeleteClicked()
{
//     // delete the item
//     if (d_shortName->count() != 0) {
//         m_langSet.removeAt(d_shortName->currentIndex());
//         emit widgetModified();
//         m_hasChanged = true;
//         d_shortName->removeItem(d_shortName->currentIndex());
//         if (d_shortName->count() != 0)
//             d_shortName->setCurrentIndex(0);
//     }
//
//     // select the next in line and make it active
//     if (d_shortName->count() != 0) {
//         setPixmap(m_langSet[d_shortName->currentIndex()].pixmapFile());
//         e_langLong->setText(m_langSet[d_shortName->currentIndex()].longId());
//     } else {
//         // the deleted one was the last one:
//         b_langPixmap->setText(i18n("No picture selected"));
//         b_langPixmap->setIcon(QIcon());
//         e_langLong->setText("");
//         b_langPixmap->setEnabled(false);
//     }
//     enableLangWidgets();
//
//     // doesn't work yet
//     if (d_shortName->count() != 0 && d_kblayout->isEnabled()) {
//         for (int i = 0; i < d_kblayout->count(); i++) {
//             if (d_kblayout->itemText(i) == m_langSet[d_shortName->currentIndex()].keyboardLayout()) {
//                 d_kblayout->setCurrentIndex(i);
//                 break;
//             }
//         }
//     }
}


void LanguageOptions::slotKeyboardLayoutChanged(const QString& layout)
{
    LanguageSettings settings(d_shortName->currentText());
    settings.setKeyboardLayout(layout);
    settings.writeConfig();

    emit widgetModified();
    m_hasChanged = true;
}

void LanguageOptions::enableLangWidgets()
{
    // check if we have a short name
    bool enabled = d_shortName->count() != 0;
    // and set everything depending upon short name
    d_shortName->setEnabled(enabled);

    // try to talk to kxbk - get a list of keyboard layouts
    if (enabled) {
        QDBusInterface kxbk("org.kde.kxkb", "/kxkb", "org.kde.KXKB");
        QDBusReply<QStringList> reply = kxbk.call("getLayoutsList");
        if (reply.isValid()) {
            QStringList layouts = reply;
            layouts.prepend(QString());
            d_kblayout->clear();
            d_kblayout->addItems(layouts);
        } else {
            kDebug() << "kxkb dbus error";
        }
    } else {
        //kDebug() << "kxkb not enabled";
        d_kblayout->clear();
        d_kblayout->setEnabled(false);
    }
}


void LanguageOptions::slotNewNameChanged(const QString& lang)
{
    // This is actually the ADD button. Enable it, if there are enough chars.
//     b_langNew->setEnabled(lang.simplified().length() >= 2);
}


void LanguageOptions::slotNewClicked()
{
//     // e_newName is the line edit
//     QString s = e_newName->text();
//     // add it's contents to the combo box
//     d_shortName->addItem(s.simplified());
//     // select it
//     d_shortName->setCurrentIndex(d_shortName->count()-1);
//     // update/enable the other widgets
//     enableLangWidgets();
//
//     slotShortActivated(s);
//     // clear the entry line
//     e_newName->setText("");
//     // let the user type a new long name
//     e_langLong->setFocus();
}


void LanguageOptions::slotShortActivated(const QString& locale)
{
    loadCountryData();

    // read config
    LanguageSettings currentSettings(locale);
    currentSettings.readConfig();
    QString icon = currentSettings.icon();

    kDebug() << "Icon for " << d_shortName->currentText() << " is " << icon;
//     if (!icon.isEmpty()) {
//         b_langPixmap->setText(QString());
//         b_langPixmap->setIcon(QIcon(icon));
//     } else {
//         b_langPixmap->setText(i18n("No Picture Selected"));
//         b_langPixmap->setIcon(QIcon());
//     }

    QString layout = currentSettings.keyboardLayout();
    kDebug() << "layout to select " << layout;
    d_kblayout->setCurrentIndex(d_kblayout->findText(layout));
}






//     int i = 0;
//     QString id = _id.simplified();
//     if (d_shortName->count() > m_langSet.count()) {
//         // avoid duplicates in language code
//         for (i = 0; i < d_shortName->count(); i++)
//             if (d_shortName->itemText(i).isEmpty()) {
//                 d_shortName->removeItem(d_shortName->currentIndex());
//                 d_shortName->setCurrentIndex(0);
//             }
//
//         for (i = 0; i < d_shortName->count()-1; i++)  // omit last
//             if (id == d_shortName->itemText(i)) {
//                 d_shortName->removeItem(d_shortName->currentIndex());
//                 d_shortName->setCurrentIndex(i);
//                 return;
//             }
//
//         if (d_shortName->count() > m_langSet.count() && m_langSet.count() < MAX_LANGSET) {
//             m_langSet.addLanguage(id, "", "");
//             emit widgetModified();
//             m_hasChanged = true;
//         }
//     }
//
//     if (d_shortName->count() != 0) {
//         b_langPixmap->setEnabled(true);
//         e_langLong->setText(m_langSet[d_shortName->currentIndex()].longId());
//
//         if (!m_langSet[d_shortName->currentIndex()].pixmapFile().isEmpty()) {
//             QPixmap pix(m_langSet[d_shortName->currentIndex()].pixmapFile());
//             if (!pix.isNull()) {
//                 b_langPixmap->setText(QString());
//                 b_langPixmap->setIcon(QIcon(pix));
//             } else {
//                 b_langPixmap->setText(i18n("Picture is Invalid"));
//                 b_langPixmap->setIcon(QIcon());
//             }
//         } else {
//             b_langPixmap->setText(i18n("No Picture Selected"));
//             b_langPixmap->setIcon(QIcon());
//         }
//
//         QString layout = m_langSet[d_shortName->currentIndex()].keyboardLayout();
//
//         //kDebug() << "layout to select " << layout;
//
//         for (int i = 0; i < d_kblayout->count(); i++) {
//             if (d_kblayout->itemText(i) == layout) {
//                 d_kblayout->setCurrentIndex(i);
//                 break;
//             }
//         }
//         // not found
//         if (d_kblayout->currentText() != layout) {
//             d_kblayout->setCurrentIndex(0);
//         }
//     }
// }


void LanguageOptions::slotLangChanged(const QString& s)
{
//     if (d_shortName->count() != 0 && d_shortName->currentIndex() < m_langSet.count()) {
//         KVTLanguage l = m_langSet[d_shortName->currentIndex()];
//         KVTLanguage l2(l.shortId(), s, l.pixmapFile(), l.keyboardLayout());
//         m_langSet.replace(d_shortName->currentIndex(), l2);
//         emit widgetModified();
//         m_hasChanged = true;
//     }
}


bool LanguageOptions::setPixmap(const QString &pm)
{
//     if (d_shortName->count()) {
//         QPixmap pix(pm);
//         if (!pix.isNull()) {
//             KVTLanguage l = m_langSet[d_shortName->currentIndex()];
//             KVTLanguage l2(l.shortId(), l.longId(), pm, l.keyboardLayout());
//             m_langSet.replace(d_shortName->currentIndex(), l2);
//             b_langPixmap->setText(QString());
//             b_langPixmap->setIcon(QIcon(pix));
//             emit widgetModified();
//             m_hasChanged = true;
//             return true;
//         }
//     }
//     return false;

}


void LanguageOptions::slotPixmapClicked()
{
//     if (m_langSet.count() > 0) {
//         if (m_lastPix.isNull() || QPixmap(m_lastPix).isNull()) {
//             QString s;
//             if (!m_langSet[d_shortName->currentIndex()].shortId().isNull()) {
//                 s = m_langSet[d_shortName->currentIndex()].shortId();
//                 m_lastPix = KStandardDirs::locate("locale", "l10n/"+s+"/flag.png");
//                 if (m_lastPix.isNull()) {
//                     m_lastPix = KStandardDirs::locate("locale", "l10n/");
//                 }
//             } else {
//                 m_lastPix = KStandardDirs::locate("locale", "l10n/");
//             }
//         } else {
//             QFileInfo fi(m_lastPix);
//             m_lastPix = fi.path()+"/flag.png";
//         }
//
//         QString s = KFileDialog::getOpenFileName(m_lastPix, KImageIO::pattern(KImageIO::Reading));
//         if (!s.isEmpty()) {
//             if (setPixmap(s))
//                 m_lastPix = s;
//             else {
//                 b_langPixmap->setText(i18n("Picture is invalid"));
//                 b_langPixmap->setIcon(QIcon());
//                 KMessageBox::sorry(this, i18n("File does not contain a valid graphics format\n"));
//             }
//         }
//     }
}

// KVTLanguageList LanguageOptions::getLangSet() const
// {
//     return m_langSet;
// }


void LanguageOptions::loadCountryData()
{
    LanguageSettings currentSettings(d_shortName->currentText());
    currentSettings.readConfig();
    QString currentIcon = currentSettings.icon();

kDebug() << " Current icon: " << currentIcon;

    iconComboBox->addItem(i18n("No icon"));

    QStringList countrylist = KGlobal::locale()->allCountriesList();
    countrylist.sort();

    int currentIndex = 0;

    foreach(QString code, countrylist) {
        QString country = KGlobal::dirs()->findResource("locale",
                QString("l10n/%1/entry.desktop").arg(code));
        KConfig entry(country, KConfig::OnlyLocal);
        KConfigGroup group = entry.group("KCM Locale");
        QString name = group.readEntry("Name", i18n("without name"));

        QString pixmap = country;
        pixmap.truncate(pixmap.lastIndexOf('/'));
        pixmap += "/flag.png";

        if ( pixmap == currentIcon ) {
            currentIndex = iconComboBox->count();
        }

        iconComboBox->addItem(QIcon(pixmap), name, code);
    }


    iconComboBox->setCurrentIndex(currentIndex);



/*

    QStringList codes = KGlobal::locale()->allLanguagesList();
    codes.sort();

    // all language codes
    foreach(QString code, codes) {
      QString languageName = KGlobal::locale()->languageCodeToName(code);
      if (!languageName.isEmpty())
        ;
//         global_langset.addLanguage(code, languageName, QString(), QString());
    }

    // localized region names are in in kde/share/locale/l10n/*.desktop
    // eg centraleurope.desktop or southasia.desktop
    QString sub = QString::fromLatin1("l10n/");
    QStringList regionlist = KGlobal::dirs()->findAllResources("locale", sub + QString::fromLatin1("*.desktop")); // why use fromLatin1 ?
    regionlist.sort();

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
    QStringList countrylist = KGlobal::locale()->allCountriesList();
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

//         int id = global_langset.indexShortId(it);
//         if (id > 0) {
//           langs.append(id);
//           // Set the pixmap of the language to the flag of the first contry that
//           // has the language as official language
//           ///@todo I have turned off icons for the ISO menu as the result was frequently wrong.
//           //if (global_langset.pixmapFile(id).isEmpty())
//           //global_langset.setPixmapFile(pixmap, id);
//         }
        //else
        //kDebug() << "Couldn't find the language for: " << *it;
      }

      int id = idx++;
      countryIdMap.insert(id, Country(name, langs, pixmap, id));
      regions[submenu].countries.append(countryIdMap[id]);

        iconComboBox->addItem(QIcon(pixmap), name);
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
    }*/

    // restore the old global locale
//     KGlobal::setLocale(lsave);
}


void LanguageOptions::slotLangFromGlobalActivated(QAction *act)
{
//     int i = act->data().toInt();
//     kDebug() << i;
//     if (countryIdMap.contains(i)) {
//         Country c = countryIdMap[i];
//         bool first = true;
//         for (QList<int>::Iterator it = c.langs.begin(); it != c.langs.end(); ++it) {
//             QString s = global_langset[*it].shortId();
//
//             if (d_shortName->contains(s.simplified())) {
//                 if (first) {
//                     d_shortName->setCurrentItem(s);
//                     slotShortActivated(s);
//                     first = false;
//                 }
//                 continue;
//             }
//
//             d_shortName->addItem(s.simplified());
//             m_langSet.addLanguage(s, global_langset[*it].longId(), global_langset[*it].pixmapFile());
//             emit widgetModified();
//             m_hasChanged = true;
//
//             if (first) {
//                 d_shortName->setCurrentIndex(d_shortName->count()-1);
//                 slotShortActivated(s);
//                 enableLangWidgets();
//
//                 e_langLong->setText(global_langset[*it].longId());
//                 slotLangChanged(global_langset[*it].longId());
//
//                 setPixmap(c.pixmap);
//                 e_newName->setText("");
//                 e_langLong->setFocus();
//                 e_langLong->selectAll();
//                 first = false;
//             }
//         }
//     }
}


void LanguageOptions::createISO6391Menus()
{
    // To have it sorted by name
//     QMap<QString, int> languages;
//     for (int id = 0; id < global_langset.count(); ++id) {
//         QString s = global_langset[id].longId();
//         languages.insert(s, id);
//     }

//     m_isoLanguagesMenu = new QMenu();
//
//     QMenu *pop = 0;
//     QAction *a = 0;
//
//     QString lang = "";
//     for (QMap<QString, int>::Iterator it = languages.begin(); it != languages.end(); ++it) {
//         if (it.key()[0].toUpper() != lang[0].toUpper())
//             pop = m_isoLanguagesMenu->addMenu(QString(it.key()[0].toUpper()));
//
//         lang = it.key();
//         QString shortid = global_langset[it.value()].shortId();
//         lang += "\t(" + shortid + ')';
//         QString pixmap = global_langset[it.value()].pixmapFile();
//         if (pixmap.isEmpty()) {
//             a = pop->addAction(lang);
//             a->setData(it.value());
//         } else {
//             a = pop->addAction(QPixmap(pixmap), lang);
//             a->setData(it.value());
//         }
//         connect(pop, SIGNAL(triggered(QAction *)), this, SLOT(slotLangFromISO6391Activated(QAction *)));
//     }
}

void LanguageOptions::slotLangFromISO6391Activated(QAction *act)
{
    int id = act->data().toInt();

//     if (id < global_langset.count()) {
//         QString shortid = global_langset[id].shortId();
//
//         if (d_shortName->contains(shortid.simplified())) {
//             d_shortName->setCurrentItem(shortid);
//             slotShortActivated(shortid);
//             return;
//         }
//
//         d_shortName->addItem(shortid.simplified());
//         d_shortName->setCurrentIndex(d_shortName->count()-1);
//         slotShortActivated(shortid);
//         enableLangWidgets();
//
//         e_langLong->setText(global_langset[id].longId());
//         slotLangChanged(e_langLong->text());
//
//         setPixmap(global_langset[id].pixmapFile());
//         e_newName->setText("");
//         e_langLong->setFocus();
//         e_langLong->selectAll();
//     }
}

bool LanguageOptions::hasChanged()
{
    return m_hasChanged;
}


void LanguageOptions::updateSettings()
{
//     m_langSet.write();
//     m_hasChanged = false;
}

void LanguageOptions::slotIconChanged(int index)
{
    QString code = iconComboBox->itemData(index).toString();
    kDebug() << "Country code selected: " << code;

    QStringList countrylist = KGlobal::locale()->allCountriesList();
    countrylist.sort();

    QString country = KGlobal::dirs()->findResource("locale",
            QString("l10n/%1/entry.desktop").arg(code));

    QString icon = country;
    icon.truncate(icon.lastIndexOf('/'));
    icon += "/flag.png";

    LanguageSettings settings(d_shortName->currentText());
    settings.setIcon(icon);
    settings.writeConfig();
}

#include "languageoptions.moc"
