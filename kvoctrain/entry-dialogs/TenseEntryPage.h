/***************************************************************************

                   tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin         : Sat Nov 27 20:20:34 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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
    void secondPluralChanged(const QString&);
    void secondSingularChanged(const QString&);
    void thirdNSingularChanged(const QString&);
    void thirdFPluralChanged(const QString&);
    void thirdMSingularChanged(const QString&);
    void thirdFSingularChanged(const QString&);
    void slotTenseSelected(int);
    void thirdMPluralChanged(const QString&);
    void thirdNPluralChanged(const QString&);
    void firstSingularChanged(const QString&);
    void firstPluralChanged(const QString&);
    void slotThirdSCommonToggled(bool);
    void slotThirdPCommonToggled(bool);
    void slotNextConj();

private:
    void setModified(bool mod);
    KEduVocConjugation prefix;
    QMap <QString, KEduVocConjugation> m_conjugations;
    QString            selection;
    bool               m_modified;
    KEduVocDocument     *m_doc;
    int m_currentRow;
    int m_currentTranslation;
};

#endif // TenseEntryPage_included
