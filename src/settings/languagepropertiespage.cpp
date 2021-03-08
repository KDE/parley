/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "languagepropertiespage.h"

#include "languagesettings.h"
#include "documentproperties.h"
#include <KMessageBox>

#include <QDebug>
#include <KLocalizedString>
#include <QLineEdit>
#include <QInputDialog>
#include <QCheckBox>
#include <QLabel>
#include <QDBusInterface>
#include <QDBusReply>
#include <sonnet/speller.h>

#include <KEduVocDocument>
#include <KEduVocExpression>
#include <KEduVocLesson>

#include <parleydocument.h>

#define TENSE_TAG ". "

LanguagePropertiesPage::LanguagePropertiesPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent)
    : QWidget(parent), m_doc(doc), m_identifierIndex(identifierIndex)
{
    setupUi(this);

    connect(localeComboBox, QOverload<const QString &>::of(&KComboBox::currentTextChanged),
            this, &LanguagePropertiesPage::localeChanged);
    connect(downloadGrammarButton, &QPushButton::clicked, this, &LanguagePropertiesPage::downloadGrammar);

    // qmap automatically sorts by keys
    QMap<QString, QString> languageCodeMap = DocumentProperties::localeLangsMap();

    // add the language, but also it's code as data
    foreach(const QString & language, languageCodeMap.keys()) {
        localeComboBox->addItem(language, languageCodeMap.value(language));
    }

    if (m_identifierIndex < m_doc->identifierCount()) {
        localeComboBox->setCurrentIndex(localeComboBox->findData(
                                            m_doc->identifier(m_identifierIndex).locale()));

        identifierNameLineEdit->setText(m_doc->identifier(m_identifierIndex).name());
    }

    int index = (m_identifierIndex < m_doc->identifierCount() ? m_identifierIndex : 0);
    LanguageSettings settings(m_doc->identifier(index).locale());
    settings.load();

    // fonts
    editorFont->setFont(settings.editorFont());
    practiceFont->setFont(settings.practiceFont());

    // keyboard layout
    // try to talk to kxbk - get a list of keyboard layouts
    QDBusInterface kxbk(QStringLiteral("org.kde.keyboard"), QStringLiteral("/Layouts"), QStringLiteral("org.kde.KeyboardLayouts"));
    QDBusReply<QStringList> reply = kxbk.call(QStringLiteral("getLayoutsList"));
    if (reply.isValid()) {
        QStringList layouts = reply;
        layouts.prepend(QString());
        keyboardLayoutComboBox->clear();
        keyboardLayoutComboBox->addItems(layouts);
        keyboardLayoutComboBox->setEnabled(true);

        QDBusReply<QString> currentLayout = kxbk.call(QStringLiteral("getCurrentLayout"));
        keyboardLayoutComboBox->setCurrentIndex(keyboardLayoutComboBox->findText(currentLayout));
        if (m_identifierIndex < m_doc->identifierCount()) {
            if (!settings.keyboardLayout().isEmpty()) {
                keyboardLayoutComboBox->setCurrentIndex(keyboardLayoutComboBox->findText(settings.keyboardLayout()));
            }
        }
    } else {
        qDebug() << "kxkb dbus error";
        keyboardLayoutComboBox->setEnabled(false);
        keyboardLayoutComboBox->addItem(i18n("No KDE keyboard selector found."));
    }

    Sonnet::Speller speller;
    QMap<QString, QString> dicts = speller.availableDictionaries();
    QMap<QString, QString>::const_iterator iter = dicts.constBegin();
    while (iter != dicts.constEnd()) {
        spellcheckerComboBox->addItem(iter.key(), iter.value());
        ++iter;
    }
    spellcheckerComboBox->model()->sort(0);
    spellcheckerComboBox->setCurrentIndex(spellcheckerComboBox->findData(settings.spellChecker()));
    loadGrammarFromDocument();
}

void LanguagePropertiesPage::setLanguageIdentifierIndex(int newIndex)
{
    m_identifierIndex = newIndex;
}

namespace DocumentHelper
{
void fetchGrammar(KEduVocDocument* doc, int languageIndex);
}

void LanguagePropertiesPage::downloadGrammar()
{
    DocumentHelper::fetchGrammar(m_doc, m_identifierIndex);
    loadGrammarFromDocument();
}

void LanguagePropertiesPage::loadGrammarFromDocument()
{
    if (m_identifierIndex < m_doc->identifierCount()) {
        // articles
        KEduVocArticle articles = m_doc->identifier(m_identifierIndex).article();

        def_male->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine));
        indef_male->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine));

        def_female->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine));
        indef_female->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine));

        def_natural->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter));
        indef_natural->setText(articles.article(KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter));


        def_male_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine));
        indef_male_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine));

        def_female_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine));
        indef_female_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine));

        def_natural_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter));
        indef_natural_plural->setText(articles.article(KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter));

        // personal pronouns
        const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
        const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
        const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

        KEduVocPersonalPronoun pronoun = m_doc->identifier(m_identifierIndex).personalPronouns();

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
    }

    // update shown labels etc...
    updateCheckBoxes();

    connect(maleFemaleDifferCheckBox, &QCheckBox::toggled, this, &LanguagePropertiesPage::updateCheckBoxes);
    connect(neutralCheckBox, &QCheckBox::toggled, this, &LanguagePropertiesPage::updateCheckBoxes);
    connect(dualCheckBox, &QCheckBox::toggled, this, &LanguagePropertiesPage::updateCheckBoxes);

    // tenses
    connect(tenseList, &QListWidget::currentRowChanged, this, &LanguagePropertiesPage::slotTenseChosen);
    connect(deleteButton, &QPushButton::clicked, this, &LanguagePropertiesPage::slotDeleteTense);
    connect(modifyButton, &QPushButton::clicked, this, &LanguagePropertiesPage::slotModifyTense);
    connect(newButton, &QPushButton::clicked, this, &LanguagePropertiesPage::slotNewTense);

    if (m_identifierIndex < m_doc->identifierCount()) {
        int i = 1;
        foreach(const QString & tenseName, m_doc->identifier(m_identifierIndex).tenseList()) {
            tenseList->addItem(QStringLiteral("%1").arg(i++, 2).append(TENSE_TAG).append(tenseName));
            tenseIndex.append(i);
        }
    }

    m_currentTense = 0;
    if (tenseList->count() > 0) {
        tenseList->setCurrentRow(m_currentTense);
    }

    modifyButton->setEnabled(tenseList->count() > 0);
    deleteButton->setEnabled(tenseList->count() > 0);
    tenseList->setFocus();
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
    m_doc->identifier(m_identifierIndex).setLocale(locale);
    m_doc->identifier(m_identifierIndex).setName(identifierNameLineEdit->text());

    LanguageSettings settings(locale);

    settings.setEditorFont(editorFont->font());
    settings.setPracticeFont(practiceFont->font());

    if (keyboardLayoutComboBox->isEnabled()) {
        settings.setKeyboardLayout(keyboardLayoutComboBox->currentText());
    }
    int index = spellcheckerComboBox->currentIndex();
    settings.setSpellChecker(spellcheckerComboBox->itemData(index).toString());
    settings.save();

    // articles
    const KEduVocWordFlag::Flags artSing = KEduVocWordFlag::Singular;
//    const KEduVocWordFlag::Flags artDual = KEduVocWordFlag::Dual;
    const KEduVocWordFlag::Flags artPlur = KEduVocWordFlag::Plural;

    const KEduVocWordFlag::Flags artDef = KEduVocWordFlag::Definite;
    const KEduVocWordFlag::Flags artIndef = KEduVocWordFlag::Indefinite;

    KEduVocArticle article;

    article.setArticle(def_male->text(),  artSing | artDef | KEduVocWordFlag::Masculine);
    article.setArticle(indef_male->text(),  artSing | artIndef | KEduVocWordFlag::Masculine);
    article.setArticle(def_female->text(),  artSing | artDef | KEduVocWordFlag::Feminine);
    article.setArticle(indef_female->text(),  artSing | artIndef | KEduVocWordFlag::Feminine);
    article.setArticle(def_natural->text(),  artSing | artDef | KEduVocWordFlag::Neuter);
    article.setArticle(indef_natural->text(),  artSing | artIndef | KEduVocWordFlag::Neuter);


    article.setArticle(def_male_plural->text(),  artPlur | artDef | KEduVocWordFlag::Masculine);
    article.setArticle(indef_male_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Masculine);
    article.setArticle(def_female_plural->text(),  artPlur | artDef | KEduVocWordFlag::Feminine);
    article.setArticle(indef_female_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Feminine);
    article.setArticle(def_natural_plural->text(),  artPlur | artDef | KEduVocWordFlag::Neuter);
    article.setArticle(indef_natural_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Neuter);

    m_doc->identifier(m_identifierIndex).setArticle(article);

    // personal pronouns
    KEduVocPersonalPronoun pronoun;
    const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
    const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    pronoun.setPersonalPronoun(first_singular->text(),  KEduVocWordFlag::First | numS);
    pronoun.setPersonalPronoun(second_singular->text(), KEduVocWordFlag::Second | numS);
    pronoun.setPersonalPronoun(thirdM_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS);
    pronoun.setPersonalPronoun(thirdF_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS);
    pronoun.setPersonalPronoun(thirdN_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS);

    pronoun.setPersonalPronoun(dualFirstLineEdit->text(), KEduVocWordFlag::First | numD);
    pronoun.setPersonalPronoun(dualSecondLineEdit->text(), KEduVocWordFlag::Second | numD);
    pronoun.setPersonalPronoun(dualThirdMaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numD);
    pronoun.setPersonalPronoun(dualThirdFemaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numD);
    pronoun.setPersonalPronoun(dualThirdNeutralLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numD);

    pronoun.setPersonalPronoun(first_plural->text(), KEduVocWordFlag::First | numP);
    pronoun.setPersonalPronoun(second_plural->text(), KEduVocWordFlag::Second |  numP);
    pronoun.setPersonalPronoun(thirdM_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP);
    pronoun.setPersonalPronoun(thirdF_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP);
    pronoun.setPersonalPronoun(thirdN_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP);

    pronoun.setMaleFemaleDifferent(maleFemaleDifferCheckBox->isChecked());
    pronoun.setNeutralExists(neutralCheckBox->isChecked());
    pronoun.setDualExists(dualCheckBox->isChecked());

    m_doc->identifier(m_identifierIndex).setPersonalPronouns(pronoun);


    // tenses
    QList<int> tenseIndex;
    QStringList tenses;

    QString str;
    for (int i = 0; i < (int) tenseList->count(); i++) {
        str = tenseList->item(i)->text();
        tenses.append(str.mid(str.indexOf(TENSE_TAG) + QStringLiteral(TENSE_TAG).length()));
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

    if (!maleFemale) {
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

void LanguagePropertiesPage::localeChanged(const QString & locale)
{
    identifierNameLineEdit->setText(locale);
}


// ************** TENSES *********************

void LanguagePropertiesPage::slotTenseChosen(int index)
{
    m_currentTense = index;
}

void LanguagePropertiesPage::slotNewTense()
{
    bool ok;
    QString getTense = QInputDialog::getText(this, i18n("Tense Name"), i18n("Enter name of tense:"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;

    QString str;
    int i = tenseList->count() + 1;
    tenseList->addItem(QStringLiteral("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    tenseIndex.append(-(i - 1));

    m_currentTense = tenseList->count();
    tenseList->setCurrentRow(i - 1);
    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}

void LanguagePropertiesPage::slotModifyTense()
{
    if (tenseList->count() != 0 && (int) tenseList->count() > m_currentTense) {
        QString str = tenseList->item(m_currentTense)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QStringLiteral(TENSE_TAG).length());

        bool ok;
        QString getTense = QInputDialog::getText(this, i18n("Tense Name"), i18n("Enter name of tense:"), QLineEdit::Normal, str, &ok);
        if (!ok)
            return;

        int i = m_currentTense + 1;
        tenseList->item(m_currentTense)->setText(QStringLiteral("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    }
}

void LanguagePropertiesPage::updateListBox(int start)
{
    QString str;
    for (int i = start; i < (int) tenseList->count(); i++) {
        str = tenseList->item(i)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QStringLiteral(TENSE_TAG).length());
        tenseList->item(i)->setText(QStringLiteral("%1").arg(i + 1, 2).append(TENSE_TAG).append(str));
    }
}

void LanguagePropertiesPage::slotDeleteTense()
{
    int act = m_currentTense;
    if (tenseList->count() > 0 && (int) tenseList->count() > act) {

        QString t = tenseList->item(act)->text();

        foreach(KEduVocExpression * exp, m_doc->lesson()->entries(KEduVocLesson::Recursive)) {
            for (int lang = 0; lang < m_doc->identifierCount(); lang++) {
                if (exp->translation(lang)->conjugationTenses().contains(t)) {
                    KMessageBox::information(this, i18n("The selected user defined tense could not be deleted\nbecause it is in use."),    i18n("Deleting Tense Description"));
                    return;
                }
            }
        }

        delete tenseList->takeItem(act);
        tenseIndex.erase(tenseIndex.begin() + act);

        if ((int) tenseList->count() <= act)
            act = tenseList->count() - 1;
        else
            updateListBox(act); // update items after current

        if (act >= 0)
            tenseList->setCurrentRow(act);
    }
    modifyButton->setEnabled(tenseList->count() > 0);
    deleteButton->setEnabled(tenseList->count() > 0);
}
