/***************************************************************************

                    base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Wed Feb 16 20:50:53 MET 2000

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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

#include "practicedialog.h"
#include "entry-dialogs/EntryDlg.h"
#include <KLocale>
#include <KColorScheme>
#include <Phonon/MediaObject>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
#include <Phonon/Global>
#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include <QCloseEvent>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsItem>

PracticeDialog::PracticeDialog(const QString & caption, KEduVocDocument *doc, QWidget *parent) : KDialog(parent)
{
    setCaption(caption);
    setModal(false);
    setButtons(0);
    setDefaultButton(NoDefault);

    QWidget *main = new QWidget(this);
    setMainWidget(main);

    m_doc = doc;
    m_entry = 0;
    m_answerTimer = 0;
    m_showSolutionTimer = 0;

    m_player = 0;
}


PracticeDialog::~PracticeDialog()
{
    if ( m_player ) {
        m_player->deleteLater();
    }
}


bool PracticeDialog::smartCompare(const QString& s1, const QString &s2) const
{
    return s1.simplified() == s2.simplified();
}


/**
 * Compare the text of a lineedit with a string, set the lineedits colors to red/green if wrong/right.
 * @param field the lineEdit
 * @param really the solution
 * @return solution is right
 */
bool PracticeDialog::verifyField(QLineEdit *field, const QString &really)
{
    kDebug() << "Compare: " << field->text() << really;

    if (!field->isEnabled()) {
        return true;
    }

    if (smartCompare(really, field->text())) {  // answer was right - green text
        setWidgetStyle(field, PositiveResult);
        return true;
    }

    // wrong - red text
    setWidgetStyle(field, NegativeResult);
    return false;  // right/wrong
}


QString  PracticeDialog::getOKComment(int percent_done)
{
    return i18n("Well done, you knew the correct answer. %1% done.", percent_done);
}


QString  PracticeDialog::getTimeoutComment(int percent_done)
{
    return i18n("You waited too long to enter the correct answer. %1% done.", percent_done);
}


QString  PracticeDialog::getNOKComment(int percent_done)
{
    return i18n("Your answer was wrong. %1% done.", percent_done);
}


void PracticeDialog::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    emit sigQueryChoice(StopIt);
}


void PracticeDialog::timeoutReached()
{
    // update every second, count down m_answerTimerCount
    if (m_answerTimerCount > 0) {
        m_answerTimerCount--;
        timebar()->setValue(m_answerTimerCount);
        m_answerTimer->start(1000);
    }

    if (m_answerTimerCount <= 0) {
        timebar()->setValue(0);
        if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Show) {
            showSolution();
        } else if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::Continue) {
            emit sigQueryChoice(Timeout); ///@todo check if this works - esp with 3x timeout
            continueButtonClicked();
        }
    }
}


void PracticeDialog::startAnswerTimer()
{
    if (Prefs::queryTimeout() == Prefs::EnumQueryTimeout::NoTimeout) {
        kDebug() << "Prefs::queryTimeout() == Prefs::EnumQueryTimeout::NoTimeout ->NO TIMEOUT!";
        return;
    }

    int mqtime = Prefs::maxTimePer();
    if (mqtime > 0) {
        if (m_answerTimer == 0) {
            m_answerTimer = new QTimer(this);
            m_answerTimer->setSingleShot(true);
            connect(m_answerTimer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
        }

        if (Prefs::queryTimeout() != Prefs::EnumQueryTimeout::NoTimeout) {
            m_answerTimerCount = mqtime;
            timebar()->setMaximum(m_answerTimerCount);
            timebar()->setValue(m_answerTimerCount);
            m_answerTimer->start(1000);
        } else
            timebar()->setEnabled(false);
    } else {
        timebar()->setEnabled(false);
    }
}


void PracticeDialog::stopAnswerTimer()
{
    if ( m_answerTimer ) {
        m_answerTimer->stop();
    }
}

void PracticeDialog::setEntry(TestEntry * entry)
{
    m_entry = entry;
    m_testType = Prefs::testType();
    startAnswerTimer();
    m_answerTainted = false;
}

void PracticeDialog::editEntry()
{
    if (m_answerTimer != 0) {
        m_answerTimer->stop();
    }

    EntryDlg* entryDlg = new EntryDlg(0 , m_doc);

    QList<int> entry;
    entry.append(m_entry->m_index);
    entryDlg->setData(entry, Prefs::toIdentifier());
    entryDlg->exec();

    // punish with a don't know
    kDebug() << "Edit entry. For now count this attempt as wrong!";
    emit sigQueryChoice(Wrong);
    continueButtonClicked();
}

void PracticeDialog::skipKnown()
{
    emit sigQueryChoice(SkipKnown);
    continueButtonClicked();
}

void PracticeDialog::skipUnknown()
{
    emit sigQueryChoice(SkipUnknown);
    continueButtonClicked();
}

void PracticeDialog::resultCorrect()
{
//     audioPlayCorrect();
    if (!m_answerTainted) {
        emit sigQueryChoice(Correct);
    } else {
        kDebug() << "Correct answer but with help (counts as wrong).";
        emit sigQueryChoice(Wrong);
    }
}

void PracticeDialog::resultWrong()
{
    emit sigQueryChoice(Wrong);
}

void PracticeDialog::audioPlayFromIdentifier()
{
    KUrl file = m_entry->exp->translation(Prefs::fromIdentifier()).soundUrl();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void PracticeDialog::audioPlayToIdentifier()
{
    KUrl file = m_entry->exp->translation(Prefs::toIdentifier()).soundUrl();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void PracticeDialog::audioPlayFile(const KUrl & soundFile)
{
    kDebug() << "Attempting to play sound: " << soundFile;

    if (!m_player) {
        m_player = new Phonon::MediaObject(this);
        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);
        createPath(m_player, audioOutput);
    } else {
        m_player->stop();
    }
    m_player->setCurrentSource(soundFile);
    m_player->play();
}


void PracticeDialog::imageShowFile(QGraphicsView * view, const QString & url)
{
    kDebug() << "show image:" << url;
    if ( !view->scene() ) {
        view->setScene(new QGraphicsScene());
    }

    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap(url));

    qreal scale = qMin(view->width()/pixmapItem->boundingRect().width(), view->height()/pixmapItem->boundingRect().height());

    pixmapItem->scale( scale, scale );

    pixmapItem->translate( -pixmapItem->boundingRect().width()/2.0, -pixmapItem->boundingRect().height()/2.0 );

    foreach ( QGraphicsItem* item, view->scene()->items() ) {
        view->scene()->removeItem(item);
        delete item;
    }
    view->scene()->setSceneRect( 0.0, 0.0, 1.0, 1.0 );
    view->scene()->addItem(pixmapItem);
}

void PracticeDialog::imageShowFromEntry(QGraphicsView * view, const TestEntry * entry)
{
    if ( Prefs::practiceImagesEnabled() ) {
        QString url = entry->exp->translation(Prefs::fromIdentifier()).imageUrl().toLocalFile();
        if ( url.isEmpty() ) {
            url = entry->exp->translation(Prefs::toIdentifier()).imageUrl().toLocalFile();
        }
        if ( url.isEmpty() ) {
            view->setVisible(false);
        } else {
            view->setVisible(true);
            imageShowFile( view, url );
        }
    }
}

void PracticeDialog::setAnswerTainted(bool tainted)
{
    m_answerTainted = tainted;
}

bool PracticeDialog::answerTainted()
{
    return m_answerTainted;
}

void PracticeDialog::setWidgetStyle(QWidget * widget, WidgetStyle style)
{
    QColor color;
    QFont ft = widget->font();

    switch (style) {
    case Default:
        ft.setWeight(QFont::Normal);
        color = QColor(0x00, 0x00, 0x00);
        break;
    case PositiveResult:
        ft.setWeight(QFont::Bold);
        color = QColor(0x00, 0x80, 0x00);
        break;
    case NegativeResult:
        ft.setWeight(QFont::Bold);
        color = QColor(0x8C, 0x25, 0x25);
        break;
    }

    QPalette qp = QPalette(widget->palette());
    qp.setColor(QPalette::Active, QPalette::Text, color);
    qp.setColor(QPalette::Inactive, QPalette::Text, color);
    qp.setColor(QPalette::Active, QPalette::WindowText, color);
    qp.setColor(QPalette::Inactive, QPalette::WindowText, color);
    widget->setPalette(qp);
    widget->setFont(ft);
}


void PracticeDialog::startShowSolutionTimer()
{
    if ( Prefs::showSolutionTime() > 0 ) {
        if (m_showSolutionTimer == 0) {
            m_showSolutionTimer = new QTimer(this);
        }
        m_showSolutionTimer->setSingleShot(true);
        connect(m_showSolutionTimer, SIGNAL(timeout()), SLOT(continueButtonClicked()));
        m_showSolutionTimer->start(Prefs::showSolutionTime() * 1000);
    }
}


void PracticeDialog::continueButtonClicked()
{
    if ( m_showSolutionTimer ) {
        m_showSolutionTimer->stop();
    }
    emit nextEntry();
}

#include "practicedialog.moc"
