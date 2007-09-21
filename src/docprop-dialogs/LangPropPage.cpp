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

LangPropPage::LangPropPage(KEduVocDocument *_doc, const KEduVocConjugation &conjug, const KEduVocArticle &art, QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(def_male,        SIGNAL(textChanged(const QString&)), this, SLOT(defMaleChanged(const QString&)));
    connect(indef_male,      SIGNAL(textChanged(const QString&)), this, SLOT(indefMaleChanged(const QString&)));
    connect(indef_female,    SIGNAL(textChanged(const QString&)), this, SLOT(indefFemaleChanged(const QString&)));
    connect(def_female,      SIGNAL(textChanged(const QString&)), this, SLOT(defFemaleChanged(const QString&)));
    connect(def_natural,     SIGNAL(textChanged(const QString&)), this, SLOT(defNaturalChanged(const QString&)));
    connect(indef_natural,   SIGNAL(textChanged(const QString&)), this, SLOT(indefNaturalChanged(const QString&)));
    connect(first_singular,  SIGNAL(textChanged(const QString&)), this, SLOT(firstSingularChanged(const QString&)));
    connect(first_plural,    SIGNAL(textChanged(const QString&)), this, SLOT(firstPluralChanged(const QString&)));
    connect(second_singular, SIGNAL(textChanged(const QString&)), this, SLOT(secondSingularChanged(const QString&)));
    connect(second_plural,   SIGNAL(textChanged(const QString&)), this, SLOT(secondPluralChanged(const QString&)));
    connect(thirdF_singular, SIGNAL(textChanged(const QString&)), this, SLOT(thirdFSingularChanged(const QString&)));
    connect(thirdF_plural,   SIGNAL(textChanged(const QString&)), this, SLOT(thirdFPluralChanged(const QString&)));
    connect(thirdM_singular, SIGNAL(textChanged(const QString&)), this, SLOT(thirdMSingularChanged(const QString&)));
    connect(thirdM_plural,   SIGNAL(textChanged(const QString&)), this, SLOT(thirdMPluralChanged(const QString&)));
    connect(thirdN_singular, SIGNAL(textChanged(const QString&)), this, SLOT(thirdNSingularChanged(const QString&)));
    connect(thirdN_plural,   SIGNAL(textChanged(const QString&)), this, SLOT(thirdNPluralChanged(const QString&)));
    connect(thirdS_common,   SIGNAL(toggled(bool)),               this, SLOT(slotThirdSCommonToggled(bool)));
    connect(thirdP_common,   SIGNAL(toggled(bool)),               this, SLOT(slotThirdPCommonToggled(bool)));

    doc = _doc;
    conjugations = conjug;


    articles = art;

    first_plural->setText(conjugations.pers1Plural());
    first_singular->setText(conjugations.pers1Singular());
    second_singular->setText(conjugations.pers2Singular());
    second_plural->setText(conjugations.pers2Plural());
    thirdM_plural->setText(conjugations.pers3MalePlural());
    thirdM_singular->setText(conjugations.pers3MaleSingular());
    thirdF_plural->setText(conjugations.pers3FemalePlural());
    thirdF_singular->setText(conjugations.pers3FemaleSingular());
    thirdN_plural->setText(conjugations.pers3NaturalPlural());
    thirdN_singular->setText(conjugations.pers3NaturalSingular());

    bool common = conjugations.pers3SingularCommon();
    thirdS_common->setChecked(common);
    thirdM_singular->setEnabled(!common);
    thirdN_singular->setEnabled(!common);

    common = conjugations.pers3PluralCommon();
    thirdP_common->setChecked(common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);


    def_male->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine ));
    indef_male->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Masculine ));

    def_female->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine ));
    indef_female->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Feminine ));

    def_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neuter ));
    indef_natural->setText(articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neuter ));

}


KEduVocConjugation LangPropPage::getConjugation()
{
//     conjugations.cleanUp();
    return conjugations;
}


void LangPropPage::firstPluralChanged(const QString& s)
{
    conjugations.setPers1Plural(s);
}


void LangPropPage::firstSingularChanged(const QString& s)
{
    conjugations.setPers1Singular(s);
}


void LangPropPage::secondSingularChanged(const QString& s)
{
    conjugations.setPers2Singular(s);
}


void LangPropPage::secondPluralChanged(const QString& s)
{
    conjugations.setPers2Plural(s);
}


void LangPropPage::thirdFPluralChanged(const QString& s)
{
    conjugations.setPers3FemalePlural(s);
}


void LangPropPage::thirdFSingularChanged(const QString& s)
{
    conjugations.setPers3FemaleSingular(s);
}


void LangPropPage::thirdMSingularChanged(const QString& s)
{
    conjugations.setPers3MaleSingular(s);
}


void LangPropPage::thirdNSingularChanged(const QString& s)
{
    conjugations.setPers3NaturalSingular(s);
}


void LangPropPage::thirdNPluralChanged(const QString& s)
{
    conjugations.setPers3NaturalPlural(s);
}


void LangPropPage::thirdMPluralChanged(const QString& s)
{
    conjugations.setPers3MalePlural(s);
}


void LangPropPage::slotThirdSCommonToggled(bool common)
{
    conjugations.setPers3SingularCommon(common);
    thirdN_singular->setEnabled(!common);
    thirdM_singular->setEnabled(!common);
}


void LangPropPage::slotThirdPCommonToggled(bool common)
{
    conjugations.setPers3PluralCommon(common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);
}


void LangPropPage::defFemaleChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine );
}


void LangPropPage::indefFemaleChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Feminine );
}


void LangPropPage::defMaleChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine );
}


void LangPropPage::indefMaleChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Masculine );
}


void LangPropPage::defNaturalChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neuter );
}


void LangPropPage::indefNaturalChanged(const QString& s)
{
    articles.setArticle(s,  KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neuter );
}

#include "LangPropPage.moc"
