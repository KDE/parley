/***************************************************************************

                    dialog when in query mode

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "writtenpracticedialog.h"

#include "kvttablemodel.h"
#include <keduvocdocument.h>

#include <KComboBox>
#include <KLineEdit>
#include <KLocale>

#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRegExp>
#include <QTimer>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QByteArray>
#include <QList>


WrittenPracticeDialog::WrittenPracticeDialog(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Written Practice"), doc, parent)
{
    mw = new Ui::WrittenPracticeDialog();
    mw->setupUi(mainWidget());

    mw->stopPracticeButton->setIcon( KIcon("list-remove") );
    mw->editEntryButton->setIcon( KIcon("edit") );
    mw->verify->setIcon(KIcon("ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));
    connect(mw->editEntryButton, SIGNAL(clicked()), SLOT(editEntry()));

    connect(mw->typeCheckBox, SIGNAL(toggled(bool)), SLOT(slotTypeClicked(bool)));
    connect(mw->commentCheckBox, SIGNAL(toggled(bool)), SLOT(slotRemClicked(bool)));
    connect(mw->falsefriendCheckBox, SIGNAL(toggled(bool)), SLOT(slotFalseFriendClicked(bool)));
    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));
    connect(mw->show_more, SIGNAL(clicked()), SLOT(showMoreClicked()));

    mw->show_more->setVisible(Prefs::showMore());
    mw->know_it->setVisible(Prefs::iKnow());

    // only shown when the solution is displayed
    mw->continueButton->setVisible(false);
    mw->continueButton->setIcon(KIcon("ok"));
    // connecting to SIGNAL nextEntry - emits the signal!
    connect(mw->continueButton, SIGNAL(clicked()), SIGNAL(nextEntry()));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    bool split = Prefs::split();

    // rip apart translations into multiple line edits
    fields = Prefs::fields();
    if (! split || fields < 1) {
        fields = 1;
    } else {
        if (fields > 10) {
            fields = 10;
        }
    }
    // add the layout to contain the parts
    QVBoxLayout * vb = new QVBoxLayout(mw->TranslationFrame);
    vb->setMargin(0);
    vb->setSpacing(KDialog::spacingHint());
    // create widgets - I wonder if this crap is used by anyone - holy cow
    bool suggestions = Prefs::suggestions();
    if (suggestions) {
        for (int i = 0; i < fields; i ++) {
            // create a bunch of comboboxes (to hold the dropdown hints)
            KComboBox * combo = new KComboBox(mw->TranslationFrame);
            transCombos.append(combo);
            combo->setObjectName(QString("transCombo%1").arg(i));
            combo->setEditable(false);
            QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Minimum);
            pol.setHeightForWidth(combo->sizePolicy().hasHeightForWidth());
            combo->setSizePolicy(pol);
            combo->setEditable(true);
            combo->setInsertPolicy(QComboBox::NoInsert);
            combo->setDuplicatesEnabled(false);
            vb->addWidget(combo);
            connect(combo, SIGNAL(editTextChanged(const QString&)), SLOT(slotTransChanged(const QString&)));
            connect(combo->lineEdit(), SIGNAL(editingFinished()), SLOT(slotTransLostFocus()));
        }
    } else {
        for (int i = 0; i < fields; i ++) {
            // normal line edits if we don't have suggestions on
            KLineEdit * line = new KLineEdit(mw->TranslationFrame);
            transFields.append(line);
            line->setObjectName(QString("transField%1").arg(i));
            QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Minimum);
            pol.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
            line->setSizePolicy(pol);
            vb->addWidget(line);
            connect(line, SIGNAL(textChanged(const QString&)), SLOT(slotTransChanged(const QString&)));
            connect(line, SIGNAL(lostFocus()), SLOT(slotTransLostFocus()));
        }
    }

    if (suggestions) {
        // take all entries from the doc and put them in a list (great)
        for (int i = 0; i < m_doc->entryCount(); i ++) {
            KEduVocExpression* expr = m_doc->entry(i);
            if (split) {
                vocabulary += extractTranslations(
                    expr->translation(Prefs::toIdentifier()).text());
            } else {
                vocabulary.append(expr->translation(Prefs::toIdentifier()).text());
            }
            if (Prefs::swapDirection()) {
                if (split) {
                    vocabulary += extractTranslations(
                        expr->translation(Prefs::fromIdentifier()).text() );
                } else {
                    vocabulary.append( expr->translation(Prefs::fromIdentifier()).text());
                }
            }
        }
        vocabulary.sort();
        // I guess this takes out doubles? great :(
        for (int k = 1; k < vocabulary.count(); k ++) {
            if (vocabulary [k - 1] == vocabulary [k]) {
                vocabulary.removeAt(k--);
            }
        }
    }

kDebug() << " added " << vocabulary.count() << " suggestions";

    mw->imageGraphicsView->setVisible(false);

    KConfigGroup cg(KGlobal::config(), "RandomQueryDialog");
    restoreDialogSize(cg);
}


WrittenPracticeDialog::~WrittenPracticeDialog()
{
    KConfigGroup cg(KGlobal::config(), "RandomQueryDialog");
    KDialog::saveDialogSize(cg);
}


void WrittenPracticeDialog::setEntry( TestEntry* entry )
{
    PracticeDialog::setEntry(entry);
    mw->continueButton->setVisible(false);

    QString trans = entry->exp->translation( Prefs::toIdentifier() ).text();
    if (Prefs::split())
        translations = extractTranslations(trans);
    else
        translations = QStringList(trans);

    mw->timebar->setVisible(Prefs::showCounter());
    mw->timelabel->setVisible(Prefs::showCounter());
    int i;
    int k;
    if (Prefs::suggestions()) {
        for (i = 0; i < fields; i ++) {
            transCombos.at(i)->clearEditText();
            setWidgetStyle(transCombos.at(i)->lineEdit());
        }
        for (k = 0; k < translations.count(); k ++)
            transCombos.at(k)->show();
        for (i = k; i < fields; i ++)
            transCombos.at(i)->hide();
    } else {
        for (i = 0; i < fields; i ++) {
            transFields.at(i)->clear();
            transFields.at(i)->setFont(Prefs::tableFont());
            setWidgetStyle(transFields.at(i));
        }

        for (k = 0; k < translations.count(); k ++) {
            transFields.at(k)->show();
        }
        for (i = k; i < fields; i ++)
            transFields.at(i)->hide();
    }
    mw->verify->setEnabled(true);
    mw->orgField->setFont(Prefs::tableFont());
    mw->orgField->setText(entry->exp->translation( Prefs::fromIdentifier() ).text());
    mw->show_all->setDefault(true);

    mw->progCount->setText( QString::number(entry->statisticCount()) );

    mw->commentLabel->hide();
    mw->falseFriendLabel->hide();
    mw->typeLabel->hide();

    mw->commentCheckBox->setChecked(false);
    mw->falsefriendCheckBox->setChecked(false);
    mw->typeCheckBox->setChecked(false);
    setHintFields();

    mw->status->clear();
    suggestion_hint = false;

    if (Prefs::suggestions()) {
        transCombos.at(0)->setFocus();
    }
    else {
        transFields.at(0)->setFocus();
    }

    imageShowFromEntry( mw->imageGraphicsView, entry );
}


QStringList WrittenPracticeDialog::extractTranslations(const QString &_trans)
{
    QString trans = _trans;
    QRegExp full_stop("^(.*[^\\. ])\\.* *$");
    //full_stop.setMinimal (true);
    if (full_stop.indexIn(trans) >= 0)
        trans = full_stop.cap(1);
    int i;
    QStringList translations;
    if (Prefs::periods())
        for (i = fields - 1; i > 0; i --) {
            QString regexp("^ *");
            for (int j = 0; j < i; j ++)
                regexp += "([^ ][^.]*)\\.[. ]*";
            regexp += "([^. ].*)$";
            QRegExp regex(regexp);
            if (regex.indexIn(trans) >= 0) {
                translations = regex.capturedTexts();
                translations.removeFirst();
                break;
            }
        }
    if (Prefs::colons() && translations.count() <= 1) {
        translations.clear();
        for (i = fields - 1; i > 0; i --) {
            QString regexp("^ *");
            for (int j = 0; j < i; j ++)
                regexp += "([^ ][^:]*):[: ]*";
            regexp += "([^: ].*)$";
            QRegExp regex(regexp);
            if (regex.indexIn(trans) >= 0) {
                translations = regex.capturedTexts();
                translations.removeFirst();
                break;
            }
        }
    }
    if (Prefs::semicolons() && translations.count() <= 1) {
        translations.clear();
        for (i = fields - 1; i > 0; i --) {
            QString regexp("^ *");
            for (int j = 0; j < i; j ++)
                regexp += "([^ ][^;]*);[; ]*";
            regexp += "([^; ].*)$";
            QRegExp regex(regexp);
            if (regex.indexIn(trans) >= 0) {
                translations = regex.capturedTexts();
                translations.removeFirst();
                break;
            }
        }
    }
    if (Prefs::commas() && translations.count() <= 1) {
        translations.clear();
        for (i = fields - 1; i > 0; i --) {
            QString regexp("^ *");
            for (int j = 0; j < i; j ++)
                regexp += "([^ ][^,]*),[, ]*";
            regexp += "([^, ].*)$";
            QRegExp regex(regexp);
            if (regex.indexIn(trans) >= 0) {
                translations = regex.capturedTexts();
                translations.removeFirst();
                break;
            }
        }
    }
    if (translations.count() <= 1)
        translations = QStringList(trans);
    return translations;
}


void WrittenPracticeDialog::verifyClicked()
{
    QStringList trans(translations);
    int i;
    int j;
    if (Prefs::suggestions()) {
        QList<KComboBox*> combos(transCombos);
        for (i = combos.count() - 1; i >= translations.count(); i --)
            combos.removeAt(i);
        for (i = 0; i < combos.count(); i ++)
            for (j = 0; j < trans.count(); j ++)
                if (smartCompare(trans[j], combos.at(i)->currentText())) {
                    verifyField(combos.at(i)->lineEdit(), trans[j]);
                    trans.removeAt(j);
                    combos.removeAt(i --);
                    break;
                }
        if (trans.count() == 0) {
            int percent = ((int)((double)mw->countbar->value()/mw->countbar->maximum() * 100.0));
            mw->status->setText(getOKComment(percent));
            resultCorrect();
            emit nextEntry();
        } else {
            for (i = 0; i < combos.count(); i ++) {
                verifyField(combos.at(i)->lineEdit(), "a\na");
            }  // always fail
            mw->status->setText(getNOKComment((int)((double)mw->countbar->value()/mw->countbar->maximum() * 100.0)));
            mw->dont_know->setDefault(true);
            setAnswerTainted();
        }
    } else {
        QList<KLineEdit*> fields(transFields);
        for (i = fields.count() - 1; i >= translations.count(); i --)
            fields.removeAt(i);
        for (i = 0; i < fields.count(); i ++) {
            for (j = 0; j < trans.count(); j ++) {
                if (smartCompare(trans[j], fields.at(i)->text())) {
                    verifyField(fields.at(i), "a\na");  // always fail
                    trans.removeAt(j);
                    fields.removeAt(i --);
                    break;
                }
            }
        }

        if (trans.count() == 0) {
            mw->status->setText(getOKComment((int)((double)mw->countbar->value()/mw->countbar->maximum() * 100.0)));
            resultCorrect();
            emit nextEntry();
        } else {
            for (i = 0; i < fields.count(); i ++) {
                verifyField(fields.at(i), trans[i]);
            }
            mw->status->setText(getNOKComment((int)((double)mw->countbar->value()/mw->countbar->maximum() * 100.0)));
            mw->dont_know->setDefault(true);
            setAnswerTainted();
        }
    }
    suggestion_hint = false;
}


void WrittenPracticeDialog::showMoreClicked()
{
    setAnswerTainted();
    if (Prefs::suggestions()) {
        for (int i = 0; i < translations.count(); i ++) {
            KComboBox* combo = transCombos.at(i);
            if (! smartCompare(combo->currentText(), translations[i])) {
                int length = combo->currentText().length() + 1;
                if (length >= translations[i].length()) {
                    combo->setEditText(translations[i]);
                    verifyField(combo->lineEdit(), translations[i]);
                    mw->verify->setEnabled(false);
                } else {
                    combo->setEditText(translations[i].left(length));
                    setWidgetStyle(combo->lineEdit());
                }
                mw->dont_know->setDefault(true);
                break;
            }
        }
    } else {
        for (int i = 0; i < translations.count(); i ++) {
            KLineEdit* field = transFields.at(i);
            if (! smartCompare(field->text(), translations[i])) {
                int length = 1;
                while ( field->text().startsWith( translations[i].left(length) ) ) {
                    kDebug() << " length " << length << "text " << field->text();
                    length++;
                }
                if (length >= translations[i].length()) {
                    field->setText(translations[i]);
                    verifyField(field, translations[i]);
                    mw->verify->setEnabled(false);
                } else {
                    field->setText(translations[i].left(length));
                    setWidgetStyle(field);
                }
                mw->dont_know->setFocus();
                mw->dont_know->setDefault(true);
                break;
            }
        }
    }
    mw->status->clear();
    suggestion_hint = false;
}


void WrittenPracticeDialog::showSolution()
{
    if (Prefs::suggestions())
        for (int i = 0; i < translations.count(); i ++) {
            transCombos.at(i)->setEditText(translations[i]);
            verifyField(transCombos.at(i)->lineEdit(), translations[i]);
        }
    else
        for (int i = 0; i < translations.count(); i ++) {
            transFields.at(i)->setText(translations[i]);
            verifyField(transFields.at(i), translations[i]);
        }
    mw->verify->setEnabled(false);
    mw->dont_know->setDefault(true);
    mw->status->clear();
    suggestion_hint = false;
}


void WrittenPracticeDialog::slotTransChanged(const QString&)
{
    mw->verify->setDefault(true);
    bool suggestions = Prefs::suggestions();
    KComboBox* combo = qobject_cast<KComboBox*>(sender());
    KLineEdit* senderedit = qobject_cast<KLineEdit*>(sender());

    if (suggestions && combo) {
        QLineEdit* edit = qobject_cast<QLineEdit*>(combo->lineEdit());
        if ( !edit ) {
            return; // cast failed
        }
        setWidgetStyle(edit);
        suggestion_hint = ! edit->text().isEmpty() && edit->text().length() <= 10;
        if (suggestion_hint)
            mw->status->setText(i18n("Press F5 for a list of translations starting with '%1'\n"
                                       "Press F6 for a list of translations containing '%1'", edit->text()));
        else {
            mw->status->clear();
        }
    } else {
        if ( (!suggestions) && senderedit) {
            setWidgetStyle(senderedit);
        }
    }
}

void WrittenPracticeDialog::slotTransLostFocus()
{
    if (suggestion_hint) {
        mw->status->clear();
    }
    suggestion_hint = false;
}


void WrittenPracticeDialog::setHintFields()
{
    bool hasComment =
        !m_entry->exp->translation(Prefs::fromIdentifier()).comment().isEmpty();
    mw->commentLabel->setVisible(hasComment);
    mw->commentCheckBox->setVisible(hasComment);

    bool hasFalseFriend = !m_entry->exp->translation(Prefs::fromIdentifier()).falseFriend(Prefs::toIdentifier()).isEmpty();
    mw->falseFriendLabel->setVisible(hasFalseFriend);
    mw->falsefriendCheckBox->setVisible(hasFalseFriend);

    bool hasType = !m_entry->exp->translation(Prefs::fromIdentifier()).type().isEmpty();
    mw->typeLabel->setVisible(hasType);
    mw->typeCheckBox->setVisible(hasType);

    // hide an empty hint box
    mw->hintGroupBox->setVisible((hasComment||hasFalseFriend||hasType));
}


void WrittenPracticeDialog::slotFalseFriendClicked(bool show)
{
    if ( show ) {
        mw->falseFriendLabel->setText(m_entry->exp->translation(Prefs::fromIdentifier()).falseFriend(Prefs::toIdentifier()));
    } else {
        mw->falseFriendLabel->setText(QString());
    }
}

void WrittenPracticeDialog::slotRemClicked(bool show)
{
    if ( show ) {
        mw->commentLabel->setText(m_entry->exp->translation(Prefs::fromIdentifier()).comment());
    } else {
        mw->commentLabel->setText(QString());
    }
}

void WrittenPracticeDialog::slotTypeClicked(bool show)
{
    if ( show ) {
        mw->typeLabel->setText(m_entry->exp->translation(Prefs::fromIdentifier()).type());
    } else {
        mw->typeLabel->setText(QString());
    }
}


void WrittenPracticeDialog::keyPressEvent(QKeyEvent *e)
{
    if (Prefs::suggestions()) {
        KComboBox* combo = 0;
        if (e->key() == Qt::Key_F4 || e->key() == Qt::Key_F5 || e->key() == Qt::Key_F6)
            for (int i = 0; i < translations.count(); i ++)
                if (transCombos.at(i)->hasFocus()) {
                    combo = transCombos.at(i);
                    break;
                }
        switch (e->key()) {

        case Qt::Key_F5:
            if (combo && !combo->currentText().isEmpty()) {
                QString curText = combo->currentText();
                combo->clear();
                combo->addItems(vocabulary.filter(QRegExp(QString("^%1").arg(curText))));
                combo->setEditText(curText);
                combo->showPopup();
            }
            break;
        case Qt::Key_F6:
            if (combo && !combo->currentText().isEmpty()) {
                QString curText = combo->currentText();
                combo->clear();
                combo->addItems(vocabulary.filter(curText, Qt::CaseInsensitive));
                combo->setEditText(curText);
                combo->showPopup();
            }
            break;
       case Qt::Key_F4:
            if (combo)
                combo->showPopup();
            break;
        }
    }

    switch (e->key()) {
    case Qt::Key_Escape:
        skipUnknown();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (mw->dont_know->isDefault())
            skipUnknown();
        else if (mw->know_it->isDefault())
            skipKnown();
        else if (mw->show_all->isDefault())
            showSolution();
        else if (mw->verify->isDefault())
            verifyClicked();
        break;

    default:
        e->ignore();
        break;
    }
}

void WrittenPracticeDialog::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}

#include "writtenpracticedialog.moc"
