/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "languagepropertiespage.h"

#include "languagesettings.h"

#include <kinputdialog.h>
#include <kmessagebox.h>

#include "practiceold/testentrymanager.h"
#include <keduvocdocument.h>
#include <keduvoclesson.h>

#include <KLocale>
#include <KStandardDirs>
#include <KLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QtDBus>

#define TENSE_TAG ". "

LanguagePropertiesPage::LanguagePropertiesPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_identifierIndex = identifierIndex;

    setupUi(this);

    connect(localeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(localeChanged(const QString&)));
    connect(iconComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(iconChanged(int)));
    connect(identifierNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(languageNameChanged(const QString&)));

    // language codes
    QStringList codes = KGlobal::locale()->allLanguagesList();

    // qmap automatically sorts by keys
    QMap<QString, QString> languageCodeMap;
    foreach (const QString &code, codes){
        languageCodeMap[KGlobal::locale()->languageCodeToName(code)] = code;
    }
    // add the language, but also it's code as data
    foreach ( const QString &language, languageCodeMap.keys() ) {
        localeComboBox->addItem( language, languageCodeMap.value(language) );
    }

    localeComboBox->setCurrentIndex(localeComboBox->findData(
        m_doc->identifier(m_identifierIndex).locale()));

    identifierNameLineEdit->setText(m_doc->identifier(m_identifierIndex).name());
        
    // icons
    LanguageSettings currentSettings(m_doc->identifier(m_identifierIndex).locale());
    currentSettings.readConfig();
    QString currentIcon = currentSettings.icon();

    iconComboBox->addItem(i18n("No icon"));

    QStringList countrylist = KGlobal::locale()->allCountriesList();
    countrylist.sort();

    foreach(const QString &code, countrylist) {
        QString country = KGlobal::dirs()->findResource("locale",
                QString("l10n/%1/entry.desktop").arg(code));
        KConfig entry(country, KConfig::SimpleConfig);
        KConfigGroup group = entry.group("KCM Locale");
        QString name = group.readEntry("Name", i18n("without name"));

        QString pixmap = country;
        pixmap.truncate(pixmap.lastIndexOf('/'));
        pixmap += "/flag.png";

        iconComboBox->addItem(QIcon(pixmap), name, pixmap);
    }

    iconComboBox->setCurrentIndex(iconComboBox->findData(currentIcon));

    // keyboard layout
    // try to talk to kxbk - get a list of keyboard layouts

    QDBusInterface kxbk("org.kde.kxkb", "/kxkb", "org.kde.KXKB");
    QDBusReply<QStringList> reply = kxbk.call("getLayoutsList");
    if (reply.isValid()) {
        QStringList layouts = reply;
        layouts.prepend(QString());
        keyboardLayoutComboBox->clear();
        keyboardLayoutComboBox->addItems(layouts);
        keyboardLayoutComboBox->setEnabled(true);

        LanguageSettings settings(m_doc->identifier(m_identifierIndex).locale());
        settings.readConfig();

        if (!settings.keyboardLayout().isEmpty()) {
            keyboardLayoutComboBox->setCurrentIndex(keyboardLayoutComboBox->findText(settings.keyboardLayout()));
        } else {
            QDBusReply<QString> currentLayout = kxbk.call("getCurrentLayout");
            keyboardLayoutComboBox->setCurrentIndex(keyboardLayoutComboBox->findText(currentLayout));
        }
    } else {
        kDebug() << "kxkb dbus error";
        keyboardLayoutComboBox->setEnabled(false);
    }


    // articles
    KEduVocArticle articles = m_doc->identifier(m_identifierIndex).article();

    def_male->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine ));
    indef_male->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine ));

    def_female->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine ));
    indef_female->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine ));

    def_natural->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter ));
    indef_natural->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter ));


    def_male_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine ));
    indef_male_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine ));

    def_female_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine ));
    indef_female_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine ));

    def_natural_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter ));
    indef_natural_plural->setText(articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter ));


    // personal pronouns
    const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
    const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    KEduVocPersonalPronoun pronoun = m_doc->identifier(identifierIndex).personalPronouns();

    first_singular->setText(pronoun.personalPronoun(KEduVocWordFlag::First | numS));
    second_singular->setText(pronoun.personalPronoun(KEduVocWordFlag::Second | numS));
    thirdM_singular->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS));
    thirdF_singular->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS));
    thirdN_singular->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS));

    dualFirstLineEdit->setText(pronoun.personalPronoun(KEduVocWordFlag::First | numD));
    dualSecondLineEdit->setText(pronoun.personalPronoun(KEduVocWordFlag::Second | numD));
    dualThirdMaleLineEdit->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numD));
    dualThirdFemaleLineEdit->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numD));
    dualThirdNeutralLineEdit->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numD));

    first_plural->setText(pronoun.personalPronoun(KEduVocWordFlag::First | numP));
    second_plural->setText(pronoun.personalPronoun(KEduVocWordFlag::Second | numP));
    thirdM_plural->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP));
    thirdF_plural->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP));
    thirdN_plural->setText(pronoun.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP));

    maleFemaleDifferCheckBox->setChecked(pronoun.maleFemaleDifferent());
    neutralCheckBox->setChecked(pronoun.neutralExists());
    dualCheckBox->setChecked(pronoun.dualExists());
    // update shown labels etc...
    updateCheckBoxes();

    connect(maleFemaleDifferCheckBox, SIGNAL(toggled(bool)), SLOT(updateCheckBoxes()));
    connect(neutralCheckBox, SIGNAL(toggled(bool)), SLOT(updateCheckBoxes()));
    connect(dualCheckBox, SIGNAL(toggled(bool)), SLOT(updateCheckBoxes()));

    // tenses
    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTenseChosen(int)));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteTense()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyTense()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewTense()));

    m_doc = doc;

    int i = 1;
    foreach(const QString &tenseName, m_doc->identifier(m_identifierIndex).tenseList()) {
        optionsList->addItem(QString("%1").arg(i++, 2).append(TENSE_TAG).append(tenseName));
        tenseIndex.append(i);
    }

    m_currentTense = 0;
    if (optionsList->count() > 0) {
        optionsList->setCurrentRow(m_currentTense);
    }

    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);

    optionsList->setFocus();
}


void LanguagePropertiesPage::accept()
{
    if (!isEnabled()) {
        // disabled pages are deleted languages
        return;
    }

    // general
    QString locale = localeComboBox->itemData(localeComboBox->currentIndex()).toString();

    // if no locales are found (kdebase not installed?) at least fall back to using the language name as locale.
    if (locale.isEmpty()) {
        locale = identifierNameLineEdit->text();
    }
    m_doc->identifier(m_identifierIndex).setLocale( locale );
    m_doc->identifier(m_identifierIndex).setName( identifierNameLineEdit->text() );

    QString icon = iconComboBox->itemData(iconComboBox->currentIndex()).toString();

    LanguageSettings settings(locale);
    settings.setIcon(icon);
    if ( keyboardLayoutComboBox->isEnabled() ) {
        settings.setKeyboardLayout( keyboardLayoutComboBox->currentText() );
    }
    settings.writeConfig();

    // articles
    const KEduVocWordFlag::Flags artSing = KEduVocWordFlag::Singular;
    const KEduVocWordFlag::Flags artDual = KEduVocWordFlag::Dual;
    const KEduVocWordFlag::Flags artPlur = KEduVocWordFlag::Plural;

    const KEduVocWordFlag::Flags artDef = KEduVocWordFlag::Definite;
    const KEduVocWordFlag::Flags artIndef = KEduVocWordFlag::Indefinite;

    KEduVocArticle article;

    article.setArticle( def_male->text(),  artSing | artDef | KEduVocWordFlag::Masculine );
    article.setArticle( indef_male->text(),  artSing | artIndef | KEduVocWordFlag::Masculine );
    article.setArticle( def_female->text(),  artSing | artDef | KEduVocWordFlag::Feminine );
    article.setArticle( indef_female->text(),  artSing | artIndef | KEduVocWordFlag::Feminine );
    article.setArticle( def_natural->text(),  artSing | artDef | KEduVocWordFlag::Neuter );
    article.setArticle( indef_natural->text(),  artSing | artIndef | KEduVocWordFlag::Neuter );


    article.setArticle( def_male_plural->text(),  artPlur | artDef | KEduVocWordFlag::Masculine );
    article.setArticle( indef_male_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Masculine );
    article.setArticle( def_female_plural->text(),  artPlur | artDef | KEduVocWordFlag::Feminine );
    article.setArticle( indef_female_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Feminine );
    article.setArticle( def_natural_plural->text(),  artPlur | artDef | KEduVocWordFlag::Neuter );
    article.setArticle( indef_natural_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Neuter );


    m_doc->identifier(m_identifierIndex).setArticle( article );

    // personal pronouns
    KEduVocPersonalPronoun pronoun;
    const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
    const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    pronoun.setPersonalPronoun(first_singular->text(),  KEduVocWordFlag::First | numS);
    pronoun.setPersonalPronoun(second_singular->text(), KEduVocWordFlag::Second| numS);
    pronoun.setPersonalPronoun(thirdM_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS);
    pronoun.setPersonalPronoun(thirdF_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS);
    pronoun.setPersonalPronoun(thirdN_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS);

    pronoun.setPersonalPronoun(dualFirstLineEdit->text(), KEduVocWordFlag::First | numD);
    pronoun.setPersonalPronoun(dualSecondLineEdit->text(), KEduVocWordFlag::Second | numD);
    pronoun.setPersonalPronoun(dualThirdMaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numD);
    pronoun.setPersonalPronoun(dualThirdFemaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numD);
    pronoun.setPersonalPronoun(dualThirdNeutralLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| numD);

    pronoun.setPersonalPronoun(first_plural->text(), KEduVocWordFlag::First | numP);
    pronoun.setPersonalPronoun(second_plural->text(), KEduVocWordFlag::Second |  numP);
    pronoun.setPersonalPronoun(thirdM_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP);
    pronoun.setPersonalPronoun(thirdF_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP);
    pronoun.setPersonalPronoun(thirdN_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP);

    pronoun.setMaleFemaleDifferent(maleFemaleDifferCheckBox->isChecked());
    pronoun.setNeutralExists(neutralCheckBox->isChecked());
    pronoun.setDualExists(dualCheckBox->isChecked());

    m_doc->identifier(m_identifierIndex).setPersonalPronouns( pronoun );


    // tenses
    QList<int> tenseIndex;
    QStringList tenses;

    QString str;
    for (int i = 0; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        tenses.append(str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length()));
    }

    m_doc->identifier(m_identifierIndex).setTenseList(tenses);
}


void LanguagePropertiesPage::updateCheckBoxes()
{
    bool maleFemale = maleFemaleDifferCheckBox->isChecked();
    bool neutral = neutralCheckBox->isChecked();
    bool dual = dualCheckBox->isChecked();

    neutralCheckBox->setVisible(maleFemale);
    male_c_label->setVisible(maleFemale);
    female_c_label->setVisible(maleFemale);
    thirdM_singular->setVisible(maleFemale);
    thirdF_singular->setVisible(maleFemale);
    thirdM_plural->setVisible(maleFemale);
    thirdF_plural->setVisible(maleFemale);

    dualLabel->setVisible(dual);
    dualFirstLineEdit->setVisible(dual);
    dualSecondLineEdit->setVisible(dual);

    dualThirdMaleLineEdit->setVisible(dual && maleFemale);
    dualThirdFemaleLineEdit->setVisible(dual && maleFemale);

    if ( !maleFemale ) {
        natural_c_label->setVisible(false);
        thirdN_singular->setVisible(true);
        thirdN_plural->setVisible(true);
        dualThirdNeutralLineEdit->setVisible(dual);
    } else {
        natural_c_label->setVisible(neutral);
        thirdN_singular->setVisible(neutral);
        thirdN_plural->setVisible(neutral);
        dualThirdNeutralLineEdit->setVisible(dual && neutral);
    }
}


void LanguagePropertiesPage::iconChanged(int iconIndex)
{
    emit iconSelected( iconComboBox->itemData(iconIndex).toString() );
}

void LanguagePropertiesPage::localeChanged(const QString & locale)
{
    identifierNameLineEdit->setText( locale );
}


// ************** TENSES *********************

void LanguagePropertiesPage::slotTenseChosen(int index)
{
    m_currentTense = index;
}

void LanguagePropertiesPage::slotNewTense()
{
    bool ok;
    QString getTense = KInputDialog::getText(i18n("Tense Description"), i18n("Enter tense description:"), QString(), &ok, this);
    if (!ok)
        return;

    QString str;
    int i = optionsList->count() + 1;
    optionsList->addItem(QString("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    tenseIndex.append(-(i - 1));

    m_currentTense = optionsList->count();
    optionsList->setCurrentRow(i - 1);
    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}

void LanguagePropertiesPage::slotModifyTense()
{
    if (optionsList->count() != 0 && (int) optionsList->count() > m_currentTense) {
        QString str = optionsList->item(m_currentTense)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length());

        bool ok;
        QString getTense = KInputDialog::getText(i18n("Tense Description"), i18n("Enter tense description:"), str, &ok, this);
        if (!ok)
            return;

        int i = m_currentTense + 1;
        optionsList->item(m_currentTense)->setText(QString("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    }
}

void LanguagePropertiesPage::updateListBox(int start)
{
    QString str;
    for (int i = start; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length());
        optionsList->item(i)->setText(QString("%1").arg(i + 1, 2).append(TENSE_TAG).append(str));
    }
}

void LanguagePropertiesPage::slotDeleteTense()
{
    int act = m_currentTense;
    if (optionsList->count() > 0 && (int) optionsList->count() > act) {

        QString t = optionsList->item(act)->text();

        foreach (KEduVocExpression *exp, m_doc->lesson()->entries(KEduVocLesson::Recursive)) {
            for (int lang = 0; lang < m_doc->identifierCount(); lang++) {
                if ( exp->translation(lang)->conjugationTenses().contains(t)) {
                        KMessageBox::information(this, i18n("The selected user defined tense could not be deleted\nbecause it is in use."),    i18n("Deleting Tense Description"));
                        return;
                }
            }
        }

        delete optionsList->takeItem(act);
        tenseIndex.erase(tenseIndex.begin() + act);

        if ((int) optionsList->count() <= act)
            act = optionsList->count() - 1;
        else
            updateListBox(act); // update items after current

        if (act >= 0)
            optionsList->setCurrentRow(act);
    }
    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);
}

#include "languagepropertiespage.moc"
