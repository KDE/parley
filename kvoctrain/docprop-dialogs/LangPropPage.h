/***************************************************************************

    $Id$

                   language properties dialog page

    -----------------------------------------------------------------------

    begin                : Wed Oct 13 18:37:13 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:38:38  arnold
    import of version 0.7.0pre8 to kde-edu


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

#include <qglobal.h>

#include "LangPropPageData.h"

#include <GrammerManager.h>

class kvoctrainDoc;
struct SpecFont_t;

class LangPropPage : public LangPropPageData
{
    Q_OBJECT

public:

    LangPropPage
    (
        SpecFont_t        *font,
        kvoctrainDoc      *doc,
        QString            curr_lang,
        const Conjugation &conjugations,
        const Article     &article,
#if QT_VERSION < 300
        const              QFont::CharSet cs,
#endif
        QWidget           *parent = NULL,
        const char        *name = NULL
    );

    Conjugation getConjugation();
    Article getArticle() const { return articles; }
#if QT_VERSION < 300
    QFont::CharSet getCharSet() const { return charset; }
#endif

protected:
    void keyPressEvent( QKeyEvent * );

public slots:
    void initFocus() const;

protected slots:

    void secondPluralChanged(const QString& );
    void secondSingularChanged(const QString& );
    void thirdNSingularChanged(const QString& );
    void thirdFPluralChanged(const QString& );
    void thirdMSingularChanged(const QString& );
    void thirdFSingularChanged(const QString& );
    void thirdMPluralChanged(const QString& );
    void thirdNPluralChanged(const QString& );
    void firstPluralChanged(const QString& );
    void firstSingularChanged(const QString& );
    void slotThirdSCommonToggled(bool);
    void slotThirdPCommonToggled(bool);

    void defNaturalChanged(const QString& );
    void indefMaleChanged(const QString& );
    void indefNaturalChanged(const QString& );
    void defFemaleChanged(const QString& );
    void indefFemaleChanged(const QString& );
    void defMaleChanged(const QString& );

    void charsetChanged(int idx);

 protected:

   kvoctrainDoc  *doc;
   Conjugation    conjugations;
   Article        articles;
#if QT_VERSION < 300
   QFont::CharSet charset;
#endif
};

#endif // LangPropPage_included
