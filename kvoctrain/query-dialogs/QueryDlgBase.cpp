/***************************************************************************

                    base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Wed Feb 16 20:50:53 MET 2000

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

#include "QueryDlgBase.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include <QCloseEvent>

#include <KLocale>
#include <KRandomSequence>

QueryDlgBase::QueryDlgBase(const QString & caption, QWidget *parent) : KDialog(parent)
{
    setCaption(caption);
    setModal(false);
    setButtons(Close|User1);
    setDefaultButton(NoDefault);
    setButtonText(Close, i18n("&Stop Query"));
    setButtonGuiItem(User1, KGuiItem(i18n("&Edit Expression...")));

    QWidget *main = new QWidget(this);
    setMainWidget(main);

    m_doc = 0;
    m_expression = 0;
}


QueryDlgBase::~QueryDlgBase()
{}


void QueryDlgBase::initFocus() const
{}


bool QueryDlgBase::smartCompare(const QString& s1, const QString &s2, int) const
{
    return s1.simplified() == s2.simplified();
}


/**
 * Compare the text of a lineedit with a string, set the lineedits colors to red/green if wrong/right.
 * @param field the lineEdit
 * @param really the solution
 * @return solution is right
 */
bool QueryDlgBase::verifyField(QLineEdit *field, const QString &really)
{
    /** @todo the colors should not be hard coded here. */
    if (!field->isEnabled())
        return true;

    if (smartCompare(really, field->text(), 0)) {  // answer was right - green text
        setWidgetTextColorAndFontWeight(field, QColor(0x00, 0x80, 0x00), QFont::Bold);
        return true;
    }
    // wrong - red text
    setWidgetTextColorAndFontWeight(field, QColor(0xff, 0x00, 0x00), QFont::Bold);
    return false;  // right/wrong
}


/**
 * Set text to black, font not bold.
 * @param field
 */
void QueryDlgBase::resetField(QLineEdit *field)
{
    if (!field->isEnabled())
        return;
    // set black text
    setWidgetTextColorAndFontWeight(field, QColor(0x00, 0x00, 0x00), QFont::Normal);
}


bool QueryDlgBase::verifyField(QTextEdit *field, const QString &really, bool mixed)
{
    if (!field->isEnabled())
        return true;

    bool ret = false;
    bool equal = false;
    QStringList answerlist = really.split('\n');
    QStringList inputlist = field->toPlainText().split('\n');
    if (!mixed) // no tolerance
        equal = smartCompare(really.simplified(), field->toPlainText().simplified(), 0);
    else {
        bool all = true;
        for (int ai = 0; ai < answerlist.count(); ai++) {
            bool found = false;
            for (int ii = 0; ii < inputlist.count(); ii++) {
                if (answerlist[ai].simplified() == inputlist[ii].simplified()) {
                    found = true;
                    break;
                }
            }
            if (!found)
                all = false;
        }
        if (all)
            equal = true;
    }

    if (equal) {
        ret = true;
        setWidgetTextColorAndFontWeight(field, QColor(0x00, 0x80, 0x00), QFont::Bold);
    } else {
        setWidgetTextColorAndFontWeight(field, QColor(0xff, 0x00, 0x00), QFont::Bold);
    }
    return ret;
}


void QueryDlgBase::resetField(QTextEdit *field)
{
    if (!field->isEnabled())
        return;

    setWidgetTextColorAndFontWeight(field, QColor(0x00, 0x00, 0x00), QFont::Normal);
}


void QueryDlgBase::verifyButton(QRadioButton *radio, bool is_ok, QWidget *widget2)
{
    if (!radio->isEnabled())
        return;

    if (is_ok) {
        setWidgetTextColorAndFontWeight(radio, QColor(0x00, 0x80, 0x00), QFont::Bold);
        if (widget2 != 0) {
            setWidgetTextColorAndFontWeight(widget2, QColor(0x00, 0x80, 0x00), QFont::Bold);
        }
    } else {
        setWidgetTextColorAndFontWeight(radio, QColor(0xff, 0x00, 0x00), QFont::Bold);
        if (widget2 != 0) {
            setWidgetTextColorAndFontWeight(widget2, QColor(0xff, 0x00, 0x00), QFont::Bold);
        }
    }
}

void QueryDlgBase::resetButton(QRadioButton *radio, QWidget *widget2)
{
    if (!radio->isEnabled())
        return;

    setWidgetTextColorAndFontWeight(radio, QColor(0x00, 0x00, 0x00), QFont::Normal);
    if (widget2 != 0) {
        setWidgetTextColorAndFontWeight(widget2, QColor(0x00, 0x00, 0x00), QFont::Normal);
    }
}


QString  QueryDlgBase::getOKComment(int percent_done)
{
    return i18n("Well done, you knew the correct answer. %1% done.", percent_done);
}


QString  QueryDlgBase::getTimeoutComment(int percent_done)
{
    return i18n("You waited too long to enter the correct answer. %1% done.", percent_done);
}


QString  QueryDlgBase::getNOKComment(int percent_done)
{
    return i18n("Your answer was wrong. %1% done.", percent_done);
}


void QueryDlgBase::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    emit sigQueryChoice(StopIt);
}


void QueryDlgBase::setWidgetTextColorAndFontWeight(QWidget *widget, const QColor &color, QFont::Weight QFontWeight)
{
    QPalette qp = QPalette(widget->palette());
    qp.setColor(QPalette::Active, QPalette::Text, color);
    qp.setColor(QPalette::Inactive, QPalette::Text, color);
    widget->setPalette(qp);

    QFont ft = widget->font();
    ft.setWeight(QFontWeight);
    widget->setFont(ft);
}


int QueryDlgBase::getRandom(int range)
{
    KRandomSequence rs;
    return rs.getLong(range);
}

#include "QueryDlgBase.moc"
