/***************************************************************************

                       query dialog for properties

    -----------------------------------------------------------------------

    begin         : Sun Apr 9 18:55:34 2000

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "SimpleQueryDlg.h"

#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <KLocale>

#include <kvttablemodel.h>
#include <keduvocdocument.h>

SimpleQueryDlg::SimpleQueryDlg(KEduVocDocument *doc, QWidget *parent) : QueryDlgBase("", doc, parent)
{
    mw = new Ui::SimpleQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));
    connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()));
    connect(mw->answerField, SIGNAL(textChanged()), SLOT(slotAnswerChanged()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    KConfigGroup cg(KGlobal::config(), "SimpleQueryDialog");
    restoreDialogSize(cg);
}


SimpleQueryDlg::~SimpleQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "SimpleQueryDialog");
    KDialog::saveDialogSize(cg);
}


void SimpleQueryDlg::initFocus() const
{
    mw->answerField->setFocus();
}


void SimpleQueryDlg::setQuery(KVTQuery::QueryType _querytype,
                              int entry,
                              int column,
                              int q_cycle,
                              int q_num,
                              int q_start,
                              KEduVocDocument  *doc)
{
    querytype = _querytype;
    m_doc = doc;
    m_row = entry;
    m_expression = m_doc->entry(m_row);
    m_queryOriginalColumn = column;
    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->queryField->setFont(Prefs::tableFont());
    mw->answerField->setFont(Prefs::tableFont());
    mw->answerField->setText("");

    QString s;
    switch (querytype) {
    case KVTQuery::SynonymQuery: {
            mw->queryLabel->setText(i18n("Expression"));
            mw->instructionLabel->setText(i18n("Enter the synonym:"));
            setWindowTitle(i18n("Synonym Training"));
            answerstring = m_expression->translation(column).synonym();
            mw->queryField->setAlignment(Qt::AlignVCenter);
            mw->queryField->setText( m_expression->translation(column).text() );
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::AntonymQuery: {
            mw->queryLabel->setText(i18n("Expression"));
            mw->instructionLabel->setText(i18n("Enter the antonym:"));
            setWindowTitle(i18n("Antonym Training"));
            answerstring = m_expression->translation(column).antonym();
            mw->queryField->setText( m_expression->translation(column).text() );
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::ParaphraseQuery: {
            mw->queryLabel->setText(i18n("Paraphrase"));
            mw->instructionLabel->setText(i18n("Enter the word:"));
            setWindowTitle(i18n("Paraphrase Training"));
            mw->queryField->setText(m_expression->translation(column).paraphrase());
            answerstring = m_expression->translation(column).text();
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::ExampleQuery: {
            mw->queryLabel->setText(i18n("Example sentence"));
            mw->instructionLabel->setText(i18n("Fill in the missing word:"));
            setWindowTitle(i18n("Example Training"));
            s = m_expression->translation(column).example();
            answerstring = m_expression->translation(column).text().simplified();
            int pos = -1;
            while ((pos = s.indexOf(answerstring)) > 0) {
                s.remove(pos, answerstring.length());
                s.insert(pos, "..");
            }
            mw->queryField->setText(s);
            setQueryFieldWordwrap();
        }
        break;

    default:
        ;
    }

    mw->show_all->setDefault(true);
    s.setNum(q_cycle);
    mw->progCount->setText(s);

    mw->countbar->setMaximum(q_start);
    mw->countbar->setValue(q_start - q_num + 1);

    startTimer();

    resetQueryWidget(mw->answerField);
}


void SimpleQueryDlg::slotAnswerChanged()
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->answerField);
}


void SimpleQueryDlg::showMoreClicked()
{
    if (mw->answerField->toPlainText().length() < answerstring.length()) {
        mw->answerField->setText(answerstring.left(mw->answerField->toPlainText().length()+1));
        mw->dont_know->setDefault(true);
    }
    resetQueryWidget(mw->answerField);
}


void SimpleQueryDlg::showSolution()
{
    mw->answerField->setText(answerstring);
    verifyField(mw->answerField, answerstring, querytype == KVTQuery::SynonymQuery || querytype == KVTQuery::AntonymQuery);
    mw->dont_know->setDefault(true);
}


void SimpleQueryDlg::verifyClicked()
{
    if (verifyField(mw->answerField, answerstring, querytype == KVTQuery::SynonymQuery || querytype == KVTQuery::AntonymQuery))
        knowItClicked();
    else
        mw->dont_know->setDefault(true);
}


void SimpleQueryDlg::knowItClicked()
{
    emit sigQueryChoice(Known);
}


void SimpleQueryDlg::dontKnowClicked()
{
    emit sigQueryChoice(Unknown);
}


void SimpleQueryDlg::slotUser1()
{
    if (m_timer != 0)
        m_timer->stop();

    emit sigEditEntry(m_row, m_queryOriginalColumn);

    KEduVocExpression *exp = m_doc->entry(m_row);
//   queryField->setText (exp->getTranslation(queryOriginalColumn));

    switch (querytype) {
    case KVTQuery::SynonymQuery: {
            answerstring = exp->translation(m_queryOriginalColumn).synonym();
            mw->queryField->setText( exp->translation(m_queryOriginalColumn).text() );
        }
        break;

    case KVTQuery::AntonymQuery: {
            answerstring = exp->translation(m_queryOriginalColumn).antonym();
            mw->queryField->setText( exp->translation(m_queryOriginalColumn).text() );
        }
        break;

    case KVTQuery::ParaphraseQuery: {
            mw->queryField->setText(exp->translation(m_queryOriginalColumn).paraphrase());
            answerstring = exp->translation(m_queryOriginalColumn).text();
        }
        break;

    case KVTQuery::ExampleQuery: {
            mw->queryField->setText(exp->translation(m_queryOriginalColumn).example());
            answerstring = exp->translation(m_queryOriginalColumn).text();
        }
        break;

    default:
        ;
    }
}


void SimpleQueryDlg::setQueryFieldWordwrap()
{
    QFontMetrics fm(Prefs::tableFont());
    int w = fm.width(mw->queryField->text());
    int w2 = mainWidget()->width();
    mw->queryField->setAlignment(Qt::AlignVCenter);
    mw->queryField->setWordWrap(w > w2);
}


#include "SimpleQueryDlg.moc"
