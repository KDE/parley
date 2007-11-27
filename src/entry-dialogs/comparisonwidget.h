/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef COMPARISONWIDGET_H
#define COMPARISONWIDGET_H

#include "ui_comparisonwidget.h"


class KEduVocExpression;
class KEduVocTranslation;
class KEduVocDocument;
class KEduVocWordType;

class ComparisonWidget : public QWidget, public Ui::comparisonWidget
{
    Q_OBJECT

public:
    ComparisonWidget(QWidget *parent = 0);

public slots:
    /**
     * To know about word types that are currently defined.
     * @param doc 
     */
    void setDocument(KEduVocDocument* doc);
    /**
     * Update the word
     * @param entry 
     * @param translation 
     */
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void slotMakeAdjectiveButton();
    void slotMakeAdverbButton();

    void slotComparativeChanged();
    void slotSuperlativeChanged();

private:
    KEduVocTranslation* m_translation;
    KEduVocDocument* m_doc;
};

#endif // ComparisonWidget_included
