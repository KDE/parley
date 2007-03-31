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

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFileInfo>
#include <QPixmap>
#include <QByteArray>
#include <QMenu>

#include <klocale.h>
#include <kcombobox.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <QtDBus>
#include "languageoptions.h"
#include "languagesettings.h"
#include "prefs.h"

#define MAX_LANGSET      100

// ISO 639-2 codes can be found at http://www.loc.gov/standards/iso639-2/php/English_list.php

struct KV_ISO639_Code
{
    const char *iso1code;
    const char *iso2code;
    const char *langname;
};

KV_ISO639_Code kv_iso639_1[] = {
                                   {"aa", "aar", I18N_NOOP("Afar")},
                                   {"ab", "abk", I18N_NOOP("Abkhazian")},
                                   {"ae", "ave", I18N_NOOP("Avestan")},
                                   {"af", "afr", I18N_NOOP("Afrikaans")},
                                   {"am", "amh", I18N_NOOP("Amharic")},
                                   {"ar", "ara", I18N_NOOP("Arabic")},
                                   {"as", "asm", I18N_NOOP("Assamese")},
                                   {"ay", "aym", I18N_NOOP("Aymara")},
                                   {"az", "aze", I18N_NOOP("Azerbaijani")},
                                   {"ba", "bak", I18N_NOOP("Bashkir")},
                                   {"be", "bel", I18N_NOOP("Belarusian")},
                                   {"bg", "bul", I18N_NOOP("Bulgarian")},
                                   {"bh", "bih", I18N_NOOP("Bihari")},
                                   {"bi", "bis", I18N_NOOP("Bislama")},
                                   {"bn", "ben", I18N_NOOP("Bengali")},
                                   {"bo", "tib", I18N_NOOP("Tibetan")},
                                   {"br", "bre", I18N_NOOP("Breton")},
                                   {"bs", "bos", I18N_NOOP("Bosnian")},
                                   {"ca", "cat", I18N_NOOP("Catalan")},
                                   {"ce", "che", I18N_NOOP("Chechen")},
                                   {"ch", "cha", I18N_NOOP("Chamorro")},
                                   {"co", "cos", I18N_NOOP("Corsican")},
                                   {"cs", "cze", I18N_NOOP("Czech")},
                                   {"cu", "chu", I18N_NOOP("Church Slavic")},
                                   {"cv", "chv", I18N_NOOP("Chuvash")},
                                   {"cy", "wel", I18N_NOOP("Welsh")},
                                   {"da", "dan", I18N_NOOP("Danish")},
                                   {"de", "ger", I18N_NOOP("German")},
                                   {"dz", "dzo", I18N_NOOP("Dzongkha")},
                                   {"el", "gre", I18N_NOOP("Greek")},
                                   {"en", "eng", I18N_NOOP("English")},
                                   {"eo", "epo", I18N_NOOP("Esperanto")},
                                   {"es", "spa", I18N_NOOP("Spanish")},
                                   {"et", "est", I18N_NOOP("Estonian")},
                                   {"eu", "baq", I18N_NOOP("Basque")},
                                   {"fa", "per", I18N_NOOP("Persian")},
                                   {"fi", "fin", I18N_NOOP("Finnish")},
                                   {"fj", "fij", I18N_NOOP("Fijian")},
                                   {"fo", "fao", I18N_NOOP("Faroese")},
                                   {"fr", "fre", I18N_NOOP("French")},
                                   {"fy", "fry", I18N_NOOP("Frisian")},
                                   {"ga", "gle", I18N_NOOP("Irish")},
                                   {"gd", "gla", I18N_NOOP("Gaelic")},
                                   {"gd", "gla", I18N_NOOP("Scottish Gaelic")},
                                   {"gl", "glg", I18N_NOOP("Gallegan")},
                                   {"gn", "grn", I18N_NOOP("Guarani")},
                                   {"gu", "guj", I18N_NOOP("Gujarati")},
                                   {"gv", "glv", I18N_NOOP("Manx")},
                                   {"ha", "hau", I18N_NOOP("Hausa")},
                                   {"he", "heb", I18N_NOOP("Hebrew")},
                                   {"hi", "hin", I18N_NOOP("Hindi")},
                                   {"ho", "hmo", I18N_NOOP("Hiri Motu")},
                                   {"hr", "scr", I18N_NOOP("Croatian")},
                                   {"hu", "hun", I18N_NOOP("Hungarian")},
                                   {"hy", "arm", I18N_NOOP("Armenian")},
                                   {"hz", "her", I18N_NOOP("Herero")},
                                   {"id", "ind", I18N_NOOP("Indonesian")},
                                   {"ie", "ile", I18N_NOOP("Interlingue")},
                                   {"ik", "ipk", I18N_NOOP("Inupiaq")},
                                   {"is", "ice", I18N_NOOP("Icelandic")},
                                   {"it", "ita", I18N_NOOP("Italian")},
                                   {"iu", "iku", I18N_NOOP("Inuktitut")},
                                   {"ja", "jpn", I18N_NOOP("Japanese")},
                                   {"jv", "jav", I18N_NOOP("Javanese")},
                                   {"ka", "geo", I18N_NOOP("Georgian")},
                                   {"ki", "kik", I18N_NOOP("Kikuyu")},
                                   {"kj", "kua", I18N_NOOP("Kuanyama")},
                                   {"kk", "kaz", I18N_NOOP("Kazakh")},
                                   {"kl", "kal", I18N_NOOP("Kalaallisut")},
                                   {"km", "khm", I18N_NOOP("Khmer")},
                                   {"kn", "kan", I18N_NOOP("Kannada")},
                                   {"ko", "kor", I18N_NOOP("Korean")},
                                   {"ks", "kas", I18N_NOOP("Kashmiri")},
                                   {"ku", "kur", I18N_NOOP("Kurdish")},
                                   {"kv", "kom", I18N_NOOP("Komi")},
                                   {"kw", "cor", I18N_NOOP("Cornish")},
                                   {"ky", "kir", I18N_NOOP("Kirghiz")},
                                   {"la", "lat", I18N_NOOP("Latin")},
                                   {"lb", "ltz", I18N_NOOP("Letzeburgesch")},
                                   {"ln", "lin", I18N_NOOP("Lingala")},
                                   {"lo", "lao", I18N_NOOP("Lao")},
                                   {"lt", "lit", I18N_NOOP("Lithuanian")},
                                   {"lv", "lav", I18N_NOOP("Latvian")},
                                   {"mg", "mlg", I18N_NOOP("Malagasy")},
                                   {"mh", "mah", I18N_NOOP("Marshall")},
                                   {"mi", "mao", I18N_NOOP("Maori")},
                                   {"mk", "mac", I18N_NOOP("Macedonian")},
                                   {"ml", "mal", I18N_NOOP("Malayalam")},
                                   {"mn", "mon", I18N_NOOP("Mongolian")},
                                   {"mo", "mol", I18N_NOOP("Moldavian")},
                                   {"mr", "mar", I18N_NOOP("Marathi")},
                                   {"ms", "may", I18N_NOOP("Malay")},
                                   {"mt", "mlt", I18N_NOOP("Maltese")},
                                   {"my", "bur", I18N_NOOP("Burmese")},
                                   {"na", "nau", I18N_NOOP("Nauru")},
                                   {"nb", "nob", I18N_NOOP("BokmÃÂÃÂ¥l")},
                                   {"nd", "nde", I18N_NOOP("Ndebele, North")},
                                   {"ne", "nep", I18N_NOOP("Nepali")},
                                   {"ng", "ndo", I18N_NOOP("Ndonga")},
                                   {"nl", "dut", I18N_NOOP("Dutch")},
                                   {"nn", "nno", I18N_NOOP("Norwegian Nynorsk")},
                                   {"no", "nor", I18N_NOOP("Norwegian")},
                                   {"nr", "nbl", I18N_NOOP("Ndebele, South")},
                                   {"nv", "nav", I18N_NOOP("Navajo")},
                                   {"ny", "nya", I18N_NOOP("Chichewa")},
                                   {"ny", "nya", I18N_NOOP("Nyanja")},
                                   {"oc", "oci", I18N_NOOP("Occitan")},
                                   {"oc", "oci", I18N_NOOP("Provencal")},
                                   {"om", "orm", I18N_NOOP("Oromo")},
                                   {"or", "ori", I18N_NOOP("Oriya")},
                                   {"os", "oss", I18N_NOOP("Ossetic")},
                                   {"pa", "pan", I18N_NOOP("Panjabi")},
                                   {"pi", "pli", I18N_NOOP("Pali")},
                                   {"pl", "pol", I18N_NOOP("Polish")},
                                   {"ps", "pus", I18N_NOOP("Pushto")},
                                   {"pt", "por", I18N_NOOP("Portuguese")},
                                   {"qu", "que", I18N_NOOP("Quechua")},
                                   {"rm", "roh", I18N_NOOP("Raeto-Romance")},
                                   {"rn", "run", I18N_NOOP("Rundi")},
                                   {"ro", "rum", I18N_NOOP("Romanian")},
                                   {"ru", "rus", I18N_NOOP("Russian")},
                                   {"rw", "kin", I18N_NOOP("Kinyarwanda")},
                                   {"sa", "san", I18N_NOOP("Sanskrit")},
                                   {"sc", "srd", I18N_NOOP("Sardinian")},
                                   {"sd", "snd", I18N_NOOP("Sindhi")},
                                   {"se", "sme", I18N_NOOP("Northern Sami")},
                                   {"sg", "sag", I18N_NOOP("Sango")},
                                   {"si", "sin", I18N_NOOP("Sinhalese")},
                                   {"sk", "slo", I18N_NOOP("Slovak")},
                                   {"sl", "slv", I18N_NOOP("Slovenian")},
                                   {"sm", "smo", I18N_NOOP("Samoan")},
                                   {"sn", "sna", I18N_NOOP("Shona")},
                                   {"so", "som", I18N_NOOP("Somali")},
                                   {"sq", "alb", I18N_NOOP("Albanian")},
                                   {"sr", "scc", I18N_NOOP("Serbian")},
                                   {"ss", "ssw", I18N_NOOP("Swati")},
                                   {"st", "sot", I18N_NOOP("Sotho, Southern")},
                                   {"su", "sun", I18N_NOOP("Sundanese")},
                                   {"sv", "swe", I18N_NOOP("Swedish")},
                                   {"sw", "swa", I18N_NOOP("Swahili")},
                                   {"ta", "tam", I18N_NOOP("Tamil")},
                                   {"te", "tel", I18N_NOOP("Telugu")},
                                   {"tg", "tgk", I18N_NOOP("Tajik")},
                                   {"th", "tha", I18N_NOOP("Thai")},
                                   {"ti", "tir", I18N_NOOP("Tigrinya")},
                                   {"tk", "tuk", I18N_NOOP("Turkmen")},
                                   {"tl", "tgl", I18N_NOOP("Tagalog")},
                                   {"tn", "tsn", I18N_NOOP("Tswana")},
                                   {"to", "ton", I18N_NOOP("Tonga")},
                                   {"tr", "tur", I18N_NOOP("Turkish")},
                                   {"ts", "tso", I18N_NOOP("Tsonga")},
                                   {"tt", "tat", I18N_NOOP("Tatar")},
                                   {"tw", "twi", I18N_NOOP("Twi")},
                                   {"ty", "tah", I18N_NOOP("Tahitian")},
                                   {"ug", "uig", I18N_NOOP("Uighur")},
                                   {"uk", "ukr", I18N_NOOP("Ukrainian")},
                                   {"ur", "urd", I18N_NOOP("Urdu")},
                                   {"uz", "uzb", I18N_NOOP("Uzbek")},
                                   {"vi", "vie", I18N_NOOP("Vietnamese")},
                                   {"vo", "vol", I18N_NOOP("VolapÃÂÃÂ¼k")},
                                   {"wo", "wol", I18N_NOOP("Wolof")},
                                   {"xh", "xho", I18N_NOOP("Xhosa")},
                                   {"yi", "yid", I18N_NOOP("Yiddish")},
                                   {"yo", "yor", I18N_NOOP("Yoruba")},
                                   {"za", "zha", I18N_NOOP("Zhuang")},
                                   {"zh", "chi", I18N_NOOP("Chinese")},
                                   {"zu", "zul", I18N_NOOP("Zulu")},
                                   {0, 0, 0}
                               };


LanguageOptions::LanguageOptions(KVTLanguages & langset, QWidget* parent) : QWidget(parent), m_langSet(langset)
{
    setupUi(this);
    m_kdeLanguagesMenu = 0;
    m_isoLanguagesMenu = 0;

    connect(b_langDel, SIGNAL(clicked()), this, SLOT(slotDeleteClicked()));
    connect(b_langNew, SIGNAL(clicked()), this, SLOT(slotNewClicked()));
    connect(b_langPixmap, SIGNAL(clicked()), SLOT(slotPixmapClicked()));
    connect(e_newName, SIGNAL(textChanged(const QString&)), this, SLOT(slotNewNameChanged(const QString&)));
    connect(d_shortName, SIGNAL(activated(const QString&)), this, SLOT(slotShortActivated(const QString&)));
    connect(e_langLong, SIGNAL(textChanged(const QString&)), this, SLOT(slotLangChanged(const QString&)));
    connect(e_shortName2, SIGNAL(textChanged(const QString&)), this, SLOT(slotShort2Changed(const QString&)));
    connect(d_kblayout, SIGNAL(activated(const QString&)), this, SLOT(slotKeyboardLayoutChanged(const QString&)));

    // Load the languages first, then the countries and create the
    // menus for the languages last, so they will have flags
    loadISO6391Data();
    loadCountryData();

    b_lang_kde->setMenu(m_kdeLanguagesMenu);

    createISO6391Menus();
    b_lang_iso1->setMenu(m_isoLanguagesMenu);

    b_langNew->setEnabled(false); // activate after data is entered

    for (int i = 0; i < (int) m_langSet.count() && i < MAX_LANGSET; i++)
        d_shortName->addItem(m_langSet.shortId(i));

    enableLangWidgets();

    if (d_shortName->count()) {
        d_shortName->setCurrentIndex(0);
        e_langLong->setText(m_langSet.longId(0));
        e_shortName2->setText(m_langSet.shortId2(0));
        if (!m_langSet.pixmapFile(0).isEmpty()) {
            QPixmap pix(m_langSet.pixmapFile(0));
            if (!pix.isNull()) {
                m_lastPix = m_langSet.pixmapFile(0);
                b_langPixmap->setText(QString());
                b_langPixmap->setIcon(QIcon(pix));
            } else {
                b_langPixmap->setText(i18n("Picture is Invalid"));
                b_langPixmap->setIcon(QIcon());
            }
        } else {
            b_langPixmap->setText(i18n("No Picture Selected"));
            b_langPixmap->setIcon(QIcon());
        }
    } else {
        b_langPixmap->setText(i18n("No Picture Selected..."));
        b_langPixmap->setEnabled(false);
        b_langPixmap->setIcon(QIcon());
    }
    m_hasChanged = false;
}


void LanguageOptions::slotDeleteClicked()
{
    if (d_shortName->count() != 0) {
        m_langSet.erase(d_shortName->currentIndex());
        emit widgetModified();
        m_hasChanged = true;
        d_shortName->removeItem(d_shortName->currentIndex());
        if (d_shortName->count() != 0)
            d_shortName->setCurrentIndex(0);
    }

    if (d_shortName->count() != 0) {
        setPixmap(m_langSet.pixmapFile(d_shortName->currentIndex()));
        e_langLong->setText(m_langSet.longId(d_shortName->currentIndex()));
        e_shortName2->setText(m_langSet.shortId2(d_shortName->currentIndex()));
    } else {
        b_langPixmap->setText(i18n("No picture selected"));
        b_langPixmap->setIcon(QIcon());
        e_langLong->setText("");
        e_shortName2->setText("");
        b_langPixmap->setEnabled(false);
    }
    enableLangWidgets();

    // doesn't work yet
    if (d_shortName->count() != 0 && d_kblayout->isEnabled()) {
        for (int i = 0; i < d_kblayout->count(); i++) {
            if (d_kblayout->itemText(i) == m_langSet.keyboardLayout(d_shortName->currentIndex())) {
                d_kblayout->setCurrentIndex(i);
                break;
            }
        }
    }
}


void LanguageOptions::slotKeyboardLayoutChanged(const QString& layout)
{
    m_langSet.setKeyboardLayout(layout, d_shortName->currentIndex());
    emit widgetModified();
    m_hasChanged = true;
}

void LanguageOptions::enableLangWidgets()
{
    bool enabled = d_shortName->count() != 0;
    b_langDel->setEnabled(enabled);
    b_langPixmap->setEnabled(enabled);
    d_shortName->setEnabled(enabled);
    e_langLong->setEnabled(enabled);
    e_shortName2->setEnabled(enabled);

    if (enabled) {
        QDBusInterface kxbk("org.kde.kxkb", "/kxkb", "org.kde.KXKB");
        QDBusReply<QStringList> reply = kxbk.call("getLayoutsList");
        if (reply.isValid()) {
            QStringList layouts = reply;
            layouts.prepend(QString());
            d_kblayout->clear();
            d_kblayout->addItems(layouts);
        } else {
            kDebug() << "kxkb dbus error" << endl;
        }
    } else {
        //kDebug() << "kxkb not enabled" << endl;
        d_kblayout->clear();
        d_kblayout->setEnabled(false);
    }
}


void LanguageOptions::slotNewNameChanged(const QString& _s)
{
    QString s = _s;
    b_langNew->setEnabled(s.simplified().length() >= 2);
}


void LanguageOptions::slotNewClicked()
{
    QString s = e_newName->text();
    d_shortName->addItem(s.simplified());
    d_shortName->setCurrentIndex(d_shortName->count()-1);
    enableLangWidgets();
    slotShortActivated(s);
    e_newName->setText("");
    e_langLong->setFocus();
}


void LanguageOptions::slotShortActivated(const QString& _id)
{
    int i = 0;
    QString id = _id.simplified();
    if (d_shortName->count() > (int) m_langSet.count()) {
        // avoid duplicates in language code
        for (i = 0; i < d_shortName->count(); i++)
            if (d_shortName->itemText(i).isEmpty()) {
                d_shortName->removeItem(d_shortName->currentIndex());
                d_shortName->setCurrentIndex(0);
            }

        for (i = 0; i < d_shortName->count()-1; i++)  // omit last
            if (id == d_shortName->itemText(i)) {
                d_shortName->removeItem(d_shortName->currentIndex());
                d_shortName->setCurrentIndex(i);
                return;
            }

        if (d_shortName->count() > m_langSet.count() && m_langSet.count() < MAX_LANGSET) {
            m_langSet.addLanguage(id, "", "");
            emit widgetModified();
            m_hasChanged = true;
        }
    }

    if (d_shortName->count() != 0) {
        b_langPixmap->setEnabled(true);
        e_langLong->setText(m_langSet.longId(d_shortName->currentIndex()));
        e_shortName2->setText(m_langSet.shortId2(d_shortName->currentIndex()));

        if (!m_langSet.pixmapFile(d_shortName->currentIndex()).isEmpty()) {
            QPixmap pix(m_langSet.pixmapFile(d_shortName->currentIndex()));
            if (!pix.isNull()) {
                b_langPixmap->setText(QString());
                b_langPixmap->setIcon(QIcon(pix));
            } else {
                b_langPixmap->setText(i18n("Picture is Invalid"));
                b_langPixmap->setIcon(QIcon());
            }
        } else {
            b_langPixmap->setText(i18n("No Picture Selected"));
            b_langPixmap->setIcon(QIcon());
        }

        QString layout = m_langSet.keyboardLayout(d_shortName->currentIndex());

        //kDebug() << "layout to select " << layout << endl;

        for (int i = 0; i < d_kblayout->count(); i++) {
            if (d_kblayout->itemText(i) == layout) {
                d_kblayout->setCurrentIndex(i);
                break;
            }
        }
        // not found
        if (d_kblayout->currentText() != layout) {
            d_kblayout->setCurrentIndex(0);
        }
    }
}


void LanguageOptions::slotLangChanged(const QString& s)
{
    if (d_shortName->count() != 0 && d_shortName->currentIndex() < m_langSet.count()) {
        m_langSet.setLongId(s, d_shortName->currentIndex());
        emit widgetModified();
        m_hasChanged = true;
    }
}


void LanguageOptions::slotShort2Changed(const QString& s)
{
    if (d_shortName->count() != 0 && d_shortName->currentIndex() < m_langSet.count()) {
        m_langSet.setShortId2(s, d_shortName->currentIndex());
        emit widgetModified();
        m_hasChanged = true;
    }
}


bool LanguageOptions::setPixmap(QString pm)
{
    if (d_shortName->count()) {
        QPixmap pix(pm);
        if (!pix.isNull()) {
            m_langSet.setPixmapFile(pm, d_shortName->currentIndex());
            b_langPixmap->setText(QString());
            b_langPixmap->setIcon(QIcon(pix));
            emit widgetModified();
            m_hasChanged = true;
            return true;
        }
    }
    return false;

}


void LanguageOptions::slotPixmapClicked()
{
    if (m_langSet.count() > 0) {
        if (m_lastPix.isNull() || QPixmap(m_lastPix).isNull()) {
            QString s;
            if (!m_langSet.shortId(d_shortName->currentIndex()).isNull()) {
                s = m_langSet.shortId(d_shortName->currentIndex());
                m_lastPix = KStandardDirs::locate("locale", "l10n/"+s+"/flag.png");
                if (m_lastPix.isNull()) {
                    m_lastPix = KStandardDirs::locate("locale", "l10n/");
                }
            } else {
                m_lastPix = KStandardDirs::locate("locale", "l10n/");
            }
        } else {
            QFileInfo fi(m_lastPix);
            m_lastPix = fi.path()+"/flag.png";
        }

        QString s = KFileDialog::getOpenFileName(m_lastPix, "*.png *.xpm *.gif *.xbm");
        if (!s.isEmpty()) {
            if (setPixmap(s))
                m_lastPix = s;
            else {
                b_langPixmap->setText(i18n("Picture is invalid"));
                b_langPixmap->setIcon(QIcon());
                KMessageBox::sorry(this, i18n("File does not contain a valid graphics format\n"));
            }
        }
    }
}

KVTLanguages LanguageOptions::getLangSet() const
{
    return m_langSet;
}


void LanguageOptions::loadCountryData()
{
    // temporary use of our locale as the global locale
    KLocale *lsave = KGlobal::locale();
    QString curr_lang = lsave->language();

//  KLocale locale("kvoctrain");
    KLocale locale(QString::null);
    locale.setLanguage(curr_lang);
    KGlobal::setLocale(&locale);

    QString sub = QString::fromLatin1("l10n/");
    QStringList regionlist = KGlobal::dirs()->findAllResources("locale", sub + QString::fromLatin1("*.desktop"));
    regionlist.sort();

    QMap<QString, Region> regions;

    foreach(QString region, regionlist) {
        QString tag = region;
        int index;

        index = tag.lastIndexOf('/');
        if (index != -1)
            tag = tag.mid(index + 1);

        index = tag.lastIndexOf('.');
        if (index != -1)
            tag.truncate(index);

        KConfig entry(region, KConfig::OnlyLocal);
        KConfigGroup group = entry.group(QString::fromLatin1("KCM Locale"));
        QString name = group.readEntry(QString::fromLatin1("Name"), i18n("without name"));

        regions.insert(tag, Region(name));
    }

    // add all languages to the list
    QStringList countrylist = KGlobal::dirs()->findAllResources("locale", sub + QString::fromLatin1("*/entry.desktop"));

    int idx = 0;
    foreach(QString country, countrylist) {
        KConfig entry(country, KConfig::OnlyLocal);
        KConfigGroup group = entry.group(QString::fromLatin1("KCM Locale"));
        QString name = group.readEntry(QString::fromLatin1("Name"), i18n("without name"));
        QString submenu = group.readEntry(QString::fromLatin1("Region"), QString());

        QString tag = country;
        int index = tag.lastIndexOf('/');
        tag.truncate(index);
        index = tag.lastIndexOf('/');
        tag = tag.mid(index+1);

        if (tag == "C")
            continue;

        QStringList all_langs = group.readEntry(QString::fromLatin1("Languages"), QString()).split(",", QString::SkipEmptyParts);
        QList<int> langs;

        QString pixmap = country;
        index = pixmap.lastIndexOf('/');
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
            //kDebug() << "Couldn't find the language for: " << *it << endl;
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
            a = regpop->addAction(QPixmap(it.value().pixmap), it.key()/*, it.value().id*/);
            a->setData(it.value().id);
        }
    }

    // restore the old global locale
    KGlobal::setLocale(lsave);
}


void LanguageOptions::slotLangFromGlobalActivated(QAction *act)
{
    int i = act->data().toInt();
    kDebug() << i << endl;
    if (countryIdMap.contains(i)) {
        Country c = countryIdMap[i];
        bool first = true;
        for (QList<int>::Iterator it = c.langs.begin(); it != c.langs.end(); ++it) {
            QString s = global_langset.shortId(*it);

            if (d_shortName->contains(s.simplified())) {
                if (first) {
                    d_shortName->setCurrentItem(s);
                    slotShortActivated(s);
                    first = false;
                }
                continue;
            }

            d_shortName->addItem(s.simplified());
            m_langSet.addLanguage(s, global_langset.longId(*it), global_langset.pixmapFile(*it), global_langset.shortId2(*it));
            emit widgetModified();
            m_hasChanged = true;

            if (first) {
                d_shortName->setCurrentIndex(d_shortName->count()-1);
                slotShortActivated(s);
                enableLangWidgets();

                e_shortName2->setText(global_langset.shortId2(*it));
                slotShort2Changed(e_shortName2->text());

                e_langLong->setText(global_langset.longId(*it));
                slotLangChanged(global_langset.longId(*it));

                setPixmap(c.pixmap);
                e_newName->setText("");
                e_langLong->setFocus();
                e_langLong->selectAll();
                first = false;
            }
        }
    }
}


void LanguageOptions::loadISO6391Data()
{
    for (unsigned id = 0; id < (int)(sizeof(kv_iso639_1) / sizeof(kv_iso639_1[0])) && kv_iso639_1[id].iso1code != 0; ++id) {
        QString s = i18n(kv_iso639_1[id].langname);
        global_langset.addLanguage(kv_iso639_1[id].iso1code, s, QString(), kv_iso639_1[id].iso2code);
    }
}

void LanguageOptions::createISO6391Menus()
{
    // To have it sorted by name
    QMap<QString, int> languages;
    for (int id = 0; id < global_langset.count(); ++id) {
        QString s = global_langset.longId(id);
        languages.insert(s, id);
    }

    m_isoLanguagesMenu = new QMenu();

    QMenu *pop = 0;
    QAction *a = 0;

    QString lang = "";
    for (QMap<QString, int>::Iterator it = languages.begin(); it != languages.end(); ++it) {
        if (it.key()[0].toUpper() != lang[0].toUpper())
            pop = m_isoLanguagesMenu->addMenu(QString(it.key()[0].toUpper()));

        lang = it.key();
        QString shortid = global_langset.shortId(it.value());
        QString short2id = global_langset.shortId2(it.value());
        lang += "\t(" + shortid + ')';
        QString pixmap = global_langset.pixmapFile(it.value());
        if (pixmap.isEmpty()) {
            a = pop->addAction(lang);
            a->setData(it.value());
        } else {
            a = pop->addAction(QPixmap(pixmap), lang);
            a->setData(it.value());
        }
        connect(pop, SIGNAL(triggered(QAction *)), this, SLOT(slotLangFromISO6391Activated(QAction *)));
    }
}

void LanguageOptions::slotLangFromISO6391Activated(QAction *act)
{
    int id = act->data().toInt();

    if (id < global_langset.count()) {
        QString shortid = global_langset.shortId(id);

        if (d_shortName->contains(shortid.simplified())) {
            d_shortName->setCurrentItem(shortid);
            slotShortActivated(shortid);
            return;
        }

        d_shortName->addItem(shortid.simplified());
        d_shortName->setCurrentIndex(d_shortName->count()-1);
        slotShortActivated(shortid);
        enableLangWidgets();

        e_shortName2->setText(global_langset.shortId2(id));
        slotShort2Changed(global_langset.shortId2(id));

        e_langLong->setText(global_langset.longId(id));
        slotLangChanged(e_langLong->text());

        setPixmap(global_langset.pixmapFile(id));
        e_newName->setText("");
        e_langLong->setFocus();
        e_langLong->selectAll();
    }
}

void LanguageOptions::updateWidgets()
{}

bool LanguageOptions::hasChanged()
{
    return m_hasChanged;
}

bool LanguageOptions::isDefault()
{
    return true;
}

void LanguageOptions::updateSettings()
{
    Prefs::setNumLangSet(m_langSet.count());

    for (int i = 0 ; i < (int) m_langSet.count(); i++) {
        LanguageSettings languageSettings(QString::number(i));
        languageSettings.setShortId(m_langSet.shortId(i));
        languageSettings.setShort2Id(m_langSet.shortId2(i));
        languageSettings.setLongId(m_langSet.longId(i));
        languageSettings.setPixmapFile(m_langSet.pixmapFile(i));
        languageSettings.setKeyboardLayout(m_langSet.keyboardLayout(i));
        languageSettings.writeConfig();
    }
    m_hasChanged = false;
}

#include "languageoptions.moc"
