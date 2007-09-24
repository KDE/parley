/***************************************************************************

                   tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin         : Sat Nov 27 20:20:34 1999

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

#ifndef TenseEntryPage_included
#define TenseEntryPage_included

#include "ui_TenseEntryPageForm.h"
#include <keduvocdocument.h>

class TenseEntryPage : public QWidget, public Ui::TenseEntryPageForm
{
    Q_OBJECT

public:
    explicit TenseEntryPage(KEduVocDocument *doc, QWidget *parent = 0);

    void setData(int row, int col);
    void commitData();
    void clear();

    bool isModified();

signals:
    void sigModified();

private slots:
    void textChanged(const QString&);
    void slotTenseSelected(int);
    void slotNextConj();

private:
    void setModified(bool mod);
    QMap <QString, KEduVocConjugation> m_conjugations;
    bool               m_modified;
    KEduVocDocument     *m_doc;
    int m_currentRow;
    int m_currentTranslation;
};

#endif // TenseEntryPage_included
