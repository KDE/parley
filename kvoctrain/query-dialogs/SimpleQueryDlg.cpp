/***************************************************************************

                       query dialog for properties

    -----------------------------------------------------------------------

    begin         : Sun Apr 9 18:55:34 2000

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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
#include <QKeyEvent>
#include <QWidget>

#include <kstandarddirs.h>
#include <klocale.h>

#include <kvttablemodel.h>
#include <keduvocdocument.h>

SimpleQueryDlg::SimpleQueryDlg(
    KVTQuery::QueryType querytype,
    int entry,
    int column,
    int q_cycle,
    int q_num,
    int q_start,
    KEduVocExpression *exp,
    KEduVocDocument  *doc)
        : QueryDlgBase("")
{
    mw = new Ui::SimpleQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showAllClicked()));
    connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()));
    connect(mw->answerField, SIGNAL(textChanged()), SLOT(slotAnswerChanged()));

    kv_doc = 0;
    qtimer = 0;

    KConfigGroup cg(KGlobal::config(), "SimpleQueryDialog");
    restoreDialogSize(cg);

    setQuery(querytype, entry, column, q_cycle, q_num, q_start, exp, doc);
    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

}


SimpleQueryDlg::~ SimpleQueryDlg()
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
                              KEduVocExpression *exp,
                              KEduVocDocument  *doc)
{
    //type_timeout = type_to;
    querytype = _querytype;
    kv_doc = doc;
    q_row = entry;
    queryOriginalColumn = column;
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
            answerstring = exp->synonym(column);
            mw->queryField->setAlignment(Qt::AlignVCenter);
            mw->queryField->setText(column == 0 ? exp->original() : exp->translation(column));
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::AntonymQuery: {
            mw->queryLabel->setText(i18n("Expression"));
            mw->instructionLabel->setText(i18n("Enter the antonym:"));
            setWindowTitle(i18n("Antonym Training"));
            answerstring = exp->antonym(column);
            mw->queryField->setText(column == 0 ? exp->original() : exp->translation(column));
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::ParaphraseQuery: {
            mw->queryLabel->setText(i18n("Paraphrase"));
            mw->instructionLabel->setText(i18n("Enter the word:"));
            setWindowTitle(i18n("Paraphrase Training"));
            mw->queryField->setText(exp->paraphrase(column));
            answerstring = column == 0 ? exp->original() : exp->translation(column);
            setQueryFieldWordwrap();
        }
        break;

    case KVTQuery::ExampleQuery: {
            mw->queryLabel->setText(i18n("Example sentence"));
            mw->instructionLabel->setText(i18n("Fill in the missing word:"));
            setWindowTitle(i18n("Example Training"));
            s = exp->example(column);
            answerstring = column == 0 ? exp->original().simplified() : exp->translation(column).simplified();
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
    int mqtime = Prefs::maxTimePer();

    if (mqtime > 0) {
        if (qtimer == 0) {
            qtimer = new QTimer(this);
            qtimer->setSingleShot(true);
            connect(qtimer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
        }

        if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
            timercount = mqtime;
            mw->timebar->setMaximum(timercount);
            mw->timebar->setValue(timercount);
            qtimer->start(1000);
        } else
            mw->timebar->setEnabled(false);
    } else
        mw->timebar->setEnabled(false);
    resetField(mw->answerField);
}


void SimpleQueryDlg::slotAnswerChanged()
{
    mw->verify->setDefault(true);
    resetField(mw->answerField);
}


void SimpleQueryDlg::timeoutReached()
{
    if (timercount > 0) {
        timercount--;
        mw->timebar->setValue(timercount);
        qtimer->start(1000);
    }

    if (timercount <= 0) {
        mw->timebar->setValue(0);
        if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
            showAllClicked();
            mw->dont_know->setDefault(true);
        } else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue)
            emit sigQueryChoice(Timeout);
    }
}


void SimpleQueryDlg::showMoreClicked()
{
    if (mw->answerField->toPlainText().length() < answerstring.length()) {
        mw->answerField->setText(answerstring.left(mw->answerField->toPlainText().length()+1));
        mw->dont_know->setDefault(true);
    }
    resetField(mw->answerField);
}


void SimpleQueryDlg::showAllClicked()
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


void SimpleQueryDlg::slotUser2()
{
    if (qtimer != 0)
        qtimer->stop();

    emit sigEditEntry(q_row, KV_COL_ORG+queryOriginalColumn);

    KEduVocExpression *exp = kv_doc->entry(q_row);
//   queryField->setText (exp->getTranslation(queryOriginalColumn));

    switch (querytype) {
    case KVTQuery::SynonymQuery: {
            answerstring = exp->synonym(queryOriginalColumn);
            mw->queryField->setText(queryOriginalColumn == 0 ? exp->original() : exp->translation(queryOriginalColumn));
        }
        break;

    case KVTQuery::AntonymQuery: {
            answerstring = exp->antonym(queryOriginalColumn);
            mw->queryField->setText(queryOriginalColumn == 0 ? exp->original() : exp->translation(queryOriginalColumn));
        }
        break;

    case KVTQuery::ParaphraseQuery: {
            mw->queryField->setText(exp->paraphrase(queryOriginalColumn));
            answerstring = queryOriginalColumn == 0 ? exp->original() : exp->translation(queryOriginalColumn);
        }
        break;

    case KVTQuery::ExampleQuery: {
            mw->queryField->setText(exp->example(queryOriginalColumn));
            answerstring = queryOriginalColumn == 0 ? exp->original() : exp->translation(queryOriginalColumn);
        }
        break;

    default:
        ;
    }
}


void SimpleQueryDlg::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        dontKnowClicked();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (mw->dont_know->isDefault())
            dontKnowClicked();
        else if (mw->know_it->isDefault())
            knowItClicked();
        else if (mw->show_all->isDefault())
            showAllClicked();
        else if (mw->verify->isDefault())
            verifyClicked();
        break;

    default:
        e->ignore();
        break;
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
