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
#include "answervalidator.h"
#include "entry-dialogs/EntryDlg.h"

#include <KLocale>
#include <KMessageBox>
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

#define MAX_QUERY_TIMEOUT 3

PracticeDialog::PracticeDialog(const QString & caption, KEduVocDocument *doc, QWidget *parent) : KDialog(parent)
{
    // dialog without buttons
    setCaption(caption);
    setModal(false);
    setButtons(0);
    setDefaultButton(NoDefault);

    QWidget *main = new QWidget(this);
    setMainWidget(main);

    m_doc = doc;
    m_answerTimer = 0;
    m_showSolutionTimer = 0;
    num_practiceTimeout = 0;

    m_player = 0;
    m_validator = new AnswerValidator(m_doc);
}


PracticeDialog::~PracticeDialog()
{
    if ( m_player ) {
        m_player->deleteLater();
    }
    delete m_validator;
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
    slotResult(StopIt);
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
        if (Prefs::practiceTimeout() == Prefs::EnumPracticeTimeout::Show) {
            showSolution();
        } else if (Prefs::practiceTimeout() == Prefs::EnumPracticeTimeout::Continue) {
            slotResult(Timeout); ///@todo check if this works - esp with 3x timeout
            continueButtonClicked();
        }
    }
}


void PracticeDialog::startAnswerTimer()
{
    if (Prefs::practiceTimeout() == Prefs::EnumPracticeTimeout::NoTimeout) {
        kDebug() << "Prefs::practiceTimeout() == Prefs::EnumPracticeTimeout::NoTimeout ->NO TIMEOUT!";
        return;
    }

    int mqtime = Prefs::maxTimePer();
    if (mqtime > 0) {
        if (m_answerTimer == 0) {
            m_answerTimer = new QTimer(this);
            m_answerTimer->setSingleShot(true);
            connect(m_answerTimer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
        }

        if (Prefs::practiceTimeout() != Prefs::EnumPracticeTimeout::NoTimeout) {
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
    m_validator->setTestEntry(m_entry, Prefs::solutionLanguage());
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
    entryDlg->setData(entry, Prefs::solutionLanguage());
    entryDlg->exec();

    // punish with a don't know
    kDebug() << "Edit entry. For now count this attempt as wrong!";
    slotResult(Wrong);
    continueButtonClicked();
}

void PracticeDialog::skipKnown()
{
    slotResult(SkipKnown);
    continueButtonClicked();
}

void PracticeDialog::skipUnknown()
{
    slotResult(SkipUnknown);
    continueButtonClicked();
}

void PracticeDialog::resultCorrect()
{
//     audioPlayCorrect();
    if (!m_answerTainted) {
        slotResult(Correct);
    } else {
        kDebug() << "Correct answer but with help (counts as wrong).";
        slotResult(Wrong);
    }
}

void PracticeDialog::resultWrong()
{
    slotResult(Wrong);
}

void PracticeDialog::audioPlayFromIdentifier()
{
    KUrl file = m_entry->exp->translation(Prefs::questionLanguage()).soundUrl();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void PracticeDialog::audioPlayToIdentifier()
{
    KUrl file = m_entry->exp->translation(Prefs::solutionLanguage()).soundUrl();
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
        view->setScene(new QGraphicsScene(view));
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
        QString url = entry->exp->translation(Prefs::questionLanguage()).imageUrl().toLocalFile();
        if ( url.isEmpty() ) {
            url = entry->exp->translation(Prefs::solutionLanguage()).imageUrl().toLocalFile();
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
    case PositiveResult:
        ft.setWeight(QFont::Bold);
        color = QColor(0x00, 0x80, 0x00);
        break;
    case NegativeResult:
        ft.setWeight(QFont::Bold);
        color = QColor(0x8C, 0x25, 0x25);
        break;
    case HintStyle:
        ft.setWeight(QFont::Bold);
        color = QColor(0x25, 0x25, 0xFF);
        break;
    case Default:
    default:
        ft.setWeight(QFont::Normal);
        color = QColor(0x00, 0x00, 0x00);
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
    nextEntry();
}


double PracticeDialog::verifyAnswer(const QString & userAnswer)
{
    m_validator->checkUserAnswer(userAnswer);
    return m_entry->lastPercentage();
}


double PracticeDialog::verifyAnswer(const QString & solution, const QString & userAnswer)
{
    m_validator->checkUserAnswer(solution, userAnswer);
    return m_entry->lastPercentage();
}


void PracticeDialog::nextEntry()
{
    // get a new entry
    m_entry = m_entryManager->nextEntry();
    if ( m_entry == 0 ) {
        accept();
        return;
    }

    setEntry(m_entry);
    setProgressCounter(m_entryManager->totalEntryCount()-m_entryManager->activeEntryCount(), m_entryManager->totalEntryCount());
}


void PracticeDialog::slotResult(PracticeDialog::Result res)
{
kDebug() << "result: " << res;
    m_doc->setModified();

    // handle the result (upgrade grades etc)
    m_entryManager->result(res);

    // check if stop was requested
    if ( res == PracticeDialog::StopIt ) {
        accept();
        return;
    }

    if ( res == PracticeDialog::Timeout ) {
        // too many timeouts in a row will hold the test
        if (++num_practiceTimeout >= MAX_QUERY_TIMEOUT) {
            const QString not_answered = i18n(
                "The test dialog was not answered several times in a row.\n"
                "It is assumed that there is currently nobody in front of "
                "the screen, and for that reason the query is stopped.");

            KMessageBox::information(this, not_answered, i18n("Stopping Test"));
        }
    } else {
        num_practiceTimeout = 0;
    }
}

int PracticeDialog::exec()
{
    nextEntry();
    return KDialog::exec();
}

void PracticeDialog::setTestEntryManager(TestEntryManager * testEntryManager)
{
    m_entryManager = testEntryManager;
}

#include "practicedialog.moc"
