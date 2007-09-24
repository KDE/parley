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

    def_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neuter ));
    indef_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neuter ));

    // personal pronouns
    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    KEduVocPersonalPronoun pronoun = m_doc->identifier(identifierIndex).personalPronouns();

    first_singular->setText(pronoun.personalPronoun(KEduVocConjugation::First, numS));
    second_singular->setText(pronoun.personalPronoun(KEduVocConjugation::Second, numS));
    thirdM_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdMale, numS));
    thirdF_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdFemale, numS));
    thirdN_singular->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdNeuterCommon, numS));
    first_plural->setText(pronoun.personalPronoun(KEduVocConjugation::First, numP));
    second_plural->setText(pronoun.personalPronoun(KEduVocConjugation::Second, numP));
    thirdM_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdMale, numP));
    thirdF_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdFemale, numP));
    thirdN_plural->setText(pronoun.personalPronoun(KEduVocConjugation::ThirdNeuterCommon, numP));

    neuterExists( pronoun.neuterExists() );
    maleFemaleDiffer( pronoun.maleFemaleDifferent() );
    maleFemaleDifferCheckBox->setChecked(pronoun.maleFemaleDifferent());
    neuterCheckBox->setChecked(pronoun.neuterExists());
    connect(maleFemaleDifferCheckBox, SIGNAL(toggled(bool)), SLOT(maleFemaleDiffer(bool)));
    connect(neuterCheckBox, SIGNAL(toggled(bool)), SLOT(neuterExists(bool)));
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
    article.setArticle( def_natural->text(),  artSing, artDef, KEduVocArticle::Neuter );
    article.setArticle( indef_natural->text(),  artSing, artIndef, KEduVocArticle::Neuter );

    m_doc->identifier(m_identifierIndex).setArticle( article );

    // personal pronouns
    KEduVocPersonalPronoun pronoun;
    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    pronoun.setPersonalPronoun(first_singular->text(),  KEduVocConjugation::First, numS);
    pronoun.setPersonalPronoun(second_singular->text(), KEduVocConjugation::Second, numS);
    pronoun.setPersonalPronoun(thirdM_singular->text(), KEduVocConjugation::ThirdMale, numS);
    pronoun.setPersonalPronoun(thirdF_singular->text(), KEduVocConjugation::ThirdFemale, numS);
    pronoun.setPersonalPronoun(thirdN_singular->text(), KEduVocConjugation::ThirdNeuterCommon, numS);

    pronoun.setPersonalPronoun(first_plural->text(), KEduVocConjugation::First, numP);
    pronoun.setPersonalPronoun(second_plural->text(), KEduVocConjugation::Second, numP);
    pronoun.setPersonalPronoun(thirdM_plural->text(), KEduVocConjugation::ThirdMale, numP);
    pronoun.setPersonalPronoun(thirdF_plural->text(), KEduVocConjugation::ThirdFemale, numP);
    pronoun.setPersonalPronoun(thirdN_plural->text(), KEduVocConjugation::ThirdNeuterCommon, numP);

    pronoun.setMaleFemaleDifferent(maleFemaleDifferCheckBox->isChecked());
    pronoun.setNeuterExists(neuterCheckBox->isChecked());
    m_doc->identifier(m_identifierIndex).setPersonalPronouns( pronoun );
}

void LangPropPage::maleFemaleDiffer(bool diff)
{
    neuterCheckBox->setVisible(diff);
    male_c_label->setVisible(diff);
    female_c_label->setVisible(diff);
    thirdM_singular->setVisible(diff);
    thirdF_singular->setVisible(diff);
    thirdM_plural->setVisible(diff);
    thirdF_plural->setVisible(diff);

    if ( !diff ) {
        natural_c_label->setVisible(true);
        thirdN_singular->setVisible(true);
        thirdN_plural->setVisible(true);
    } else {
        natural_c_label->setVisible(neuterCheckBox->isChecked());
        thirdN_singular->setVisible(neuterCheckBox->isChecked());
        thirdN_plural->setVisible(neuterCheckBox->isChecked());
    }
}

void LangPropPage::neuterExists(bool exists)
{
    natural_c_label->setVisible(exists);
    thirdN_singular->setVisible(exists);
    thirdN_plural->setVisible(exists);
}

#include "LangPropPage.moc"
