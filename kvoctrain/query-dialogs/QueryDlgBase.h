/***************************************************************************

                   base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef Query_Dlg_Base_H
#define Query_Dlg_Base_H

#include <QCloseEvent>
#include <QProgressBar>
#include <KDialog>

#include "query-dialogs/kvtquery.h"
#include <keduvocgrammar.h>
#include "prefs.h"

class KEduVocExpression;
class KEduVocDocument;

class QLineEdit;
class QTextEdit;
class QRadioButton;

class QueryDlgBase : public KDialog
{
Q_OBJECT
public:
    enum Result { Unknown, Known, Timeout, StopIt };

    QueryDlgBase(const QString & caption, KEduVocDocument *doc, QWidget *parent);
    virtual ~QueryDlgBase();

    void startTimer();
    void setDocument(KEduVocDocument *doc) { m_doc = doc; }

public slots:
    virtual void showSolution() = 0;
    void timeoutReached();

signals:
    void sigQueryChoice(QueryDlgBase::Result);
    void sigEditEntry(int row, int col);

protected:
    /// @todo make a base query widget so these are no longer needed, let all other query widgets add their specific elements to that base.
    virtual void setStatusText(const QString &status) = 0;
    virtual QProgressBar* timebar() = 0;

    /** compare two strings with simplified applied first */
    bool smartCompare(const QString&, const QString&) const;

    /** compare lineEdit and string, set lineEdit style */
    bool verifyField(QLineEdit *field, const QString &really);

    /** the same for QTextEdit and QRadioButton */
    bool verifyField(QTextEdit *field, const QString &really, bool mixed);
    void verifyButton(QRadioButton *radio, bool is_ok, QWidget *widget2 = 0);

    /** set the default color and font */
    void resetQueryWidget(QWidget *);

    // Show string after selecting known/unknown
    // depending on progress and randomness
    QString  getOKComment(int percent);
    QString  getNOKComment(int percent);
    QString  getTimeoutComment(int percent);

    virtual void initFocus() const;
    virtual void closeEvent(QCloseEvent*e);
    void setWidgetTextColorAndFontWeight(QWidget *widget, const QColor &color, QFont::Weight);

protected:
    int                m_row;
    int                m_queryOriginalColumn;
    int                m_queryTranslationColumn;
    KEduVocDocument   *m_doc;
    KEduVocExpression *m_expression;
    QTimer            *m_timer;
    int                m_timerCount;
};


#endif // Query_Dlg_Base_H
