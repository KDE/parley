/***************************************************************************

                   language properties dialog page

    -----------------------------------------------------------------------

    begin         : Wed Oct 13 18:37:13 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "LangPropPage.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>

#include <KLocale>

#include <keduvocdocument.h>

LangPropPage::LangPropPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_identifierIndex = identifierIndex;

    setupUi(this);

    // articles
    KEduVocArticle articles = m_doc->identifier(m_identifierIndex).article();

    def_male->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine ));
    indef_male->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine ));

    def_female->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine ));
    indef_female->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine ));

    def_natural->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter ));
    indef_natural->setText(articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter ));

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
}


void LangPropPage::accept()
{
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
}


void LangPropPage::updateCheckBoxes()
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
        natural_c_label->setVisible(true);
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



#include "LangPropPage.moc"
