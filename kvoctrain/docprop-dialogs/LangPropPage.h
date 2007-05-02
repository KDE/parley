/***************************************************************************

                   language properties dialog page

    -----------------------------------------------------------------------

    begin         : Wed Oct 13 18:37:13 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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


#ifndef LangPropPage_included
#define LangPropPage_included

#include "ui_LangPropPageForm.h"

#include <keduvocgrammar.h>

class KEduVocDocument;
class KEduVocConjugation;
class KEduVocArticle;

class LangPropPage : public QWidget, public Ui::LangPropPageForm
{
    Q_OBJECT
public:
    LangPropPage(KEduVocDocument *doc, QString curr_lang, const KEduVocConjugation &conjugations,
                 const KEduVocArticle &article, QWidget *parent = 0);

    KEduVocConjugation getConjugation();
    KEduVocArticle getArticle() const
    {
        return articles;
    }

protected slots:
    void secondPluralChanged(const QString&);
    void secondSingularChanged(const QString&);
    void thirdNSingularChanged(const QString&);
    void thirdFPluralChanged(const QString&);
    void thirdMSingularChanged(const QString&);
    void thirdFSingularChanged(const QString&);
    void thirdMPluralChanged(const QString&);
    void thirdNPluralChanged(const QString&);
    void firstPluralChanged(const QString&);
    void firstSingularChanged(const QString&);
    void slotThirdSCommonToggled(bool);
    void slotThirdPCommonToggled(bool);

    void defNaturalChanged(const QString&);
    void indefMaleChanged(const QString&);
    void indefNaturalChanged(const QString&);
    void defFemaleChanged(const QString&);
    void indefFemaleChanged(const QString&);
    void defMaleChanged(const QString&);

private:
    KEduVocDocument    *doc;
    KEduVocConjugation  conjugations;
    KEduVocArticle      articles;
};

#endif // LangPropPage_included
