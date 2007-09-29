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

    def_male->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine ));
    indef_male->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Masculine ));

    def_female->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine ));
    indef_female->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Feminine ));

    def_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neutral ));
    indef_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neutral ));

    // personal pronouns
    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    const KEduVocConjugation::ConjugationNumber numD = KEduVocConjugation::Dual;
    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    KEduVocPersonalPronoun pronoun = m_doc->identifier(identifierIndex).personalPronouns();

    first_singular->setText(pronoun.personalPronoun(KEduVocConjugation::First, numS));
    second_singular->setText(pronoun.personalPronoun(KEduVocConjugation::Second, numS));
    thirdM_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdMale, numS));
    thirdF_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdFemale, numS));
    thirdN_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdNeutralCommon, numS));

    dualFirstLineEdit->setText(pronoun.personalPronoun(KEduVocConjugation::First, numD));
    dualSecondLineEdit->setText(pronoun.personalPronoun(KEduVocConjugation::Second, numD));
    dualThirdMaleLineEdit->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdMale, numD));
    dualThirdFemaleLineEdit->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdFemale, numD));
    dualThirdNeutralLineEdit->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdNeutralCommon, numD));

    first_plural->setText(pronoun.personalPronoun(KEduVocConjugation::First, numP));
    second_plural->setText(pronoun.personalPronoun(KEduVocConjugation::Second, numP));
    thirdM_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdMale, numP));
    thirdF_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdFemale, numP));
    thirdN_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdNeutralCommon, numP));

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
    const KEduVocArticle::ArticleNumber artSing = KEduVocArticle::Singular;
    const KEduVocArticle::ArticleNumber artDual = KEduVocArticle::Dual;
    const KEduVocArticle::ArticleNumber artPlur = KEduVocArticle::Plural;

    const KEduVocArticle::ArticleDefiniteness artDef = KEduVocArticle::Definite;
    const KEduVocArticle::ArticleDefiniteness artIndef = KEduVocArticle::Indefinite;

    KEduVocArticle article;

    article.setArticle( def_male->text(),  artSing, artDef, KEduVocArticle::Masculine );
    article.setArticle( indef_male->text(),  artSing, artIndef, KEduVocArticle::Masculine );
    article.setArticle( def_female->text(),  artSing, artDef, KEduVocArticle::Feminine );
    article.setArticle( indef_female->text(),  artSing, artIndef, KEduVocArticle::Feminine );
    article.setArticle( def_natural->text(),  artSing, artDef, KEduVocArticle::Neutral );
    article.setArticle( indef_natural->text(),  artSing, artIndef, KEduVocArticle::Neutral );

    m_doc->identifier(m_identifierIndex).setArticle( article );

    // personal pronouns
    KEduVocPersonalPronoun pronoun;
    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    const KEduVocConjugation::ConjugationNumber numD = KEduVocConjugation::Dual;
    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    pronoun.setPersonalPronoun(first_singular->text(),  KEduVocConjugation::First, numS);
    pronoun.setPersonalPronoun(second_singular->text(), KEduVocConjugation::Second, numS);
    pronoun.setPersonalPronoun(thirdM_singular->text(), KEduVocConjugation::ThirdMale, numS);
    pronoun.setPersonalPronoun(thirdF_singular->text(), KEduVocConjugation::ThirdFemale, numS);
    pronoun.setPersonalPronoun(thirdN_singular->text(), KEduVocConjugation::ThirdNeutralCommon, numS);

    pronoun.setPersonalPronoun(dualFirstLineEdit->text(), KEduVocConjugation::First, numD);
    pronoun.setPersonalPronoun(dualSecondLineEdit->text(), KEduVocConjugation::Second, numD);
    pronoun.setPersonalPronoun(dualThirdMaleLineEdit->text(), KEduVocConjugation::ThirdMale, numD);
    pronoun.setPersonalPronoun(dualThirdFemaleLineEdit->text(), KEduVocConjugation::ThirdFemale, numD);
    pronoun.setPersonalPronoun(dualThirdNeutralLineEdit->text(), KEduVocConjugation::ThirdNeutralCommon, numD);

    pronoun.setPersonalPronoun(first_plural->text(), KEduVocConjugation::First, numP);
    pronoun.setPersonalPronoun(second_plural->text(), KEduVocConjugation::Second, numP);
    pronoun.setPersonalPronoun(thirdM_plural->text(), KEduVocConjugation::ThirdMale, numP);
    pronoun.setPersonalPronoun(thirdF_plural->text(), KEduVocConjugation::ThirdFemale, numP);
    pronoun.setPersonalPronoun(thirdN_plural->text(), KEduVocConjugation::ThirdNeutralCommon, numP);

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
