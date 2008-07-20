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
#include "answervalidatorold.h"
#include "languagesettings.h"

#include <keduvocdocument.h>

#include <KLocale>
#include <KPassivePopup>
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
#include <QDBusInterface>

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

    m_player = 0;
    m_validator = new AnswerValidatorOld(m_doc);
    m_validator->setLanguage(Prefs::solutionLanguage());

    if ( !m_validator->spellcheckerAvailable() ) {
        KPassivePopup* pop = new KPassivePopup(this);
        pop->setTimeout(10000);
        pop->setView(i18nc("@popupmessage", "Either the language set up is incorrect or no spellchecker was installed for %1 (%2).", m_doc->identifier(Prefs::solutionLanguage()).name(), m_doc->identifier(Prefs::solutionLanguage()).locale()), i18nc("@title of a popup", "No Spell Checker Available"));
        pop->show();
    }
}


PracticeDialog::~PracticeDialog()
{
    if ( m_player ) {
        m_player->deleteLater();
    }
    delete m_validator;
}


void PracticeDialog::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    signalResult(TestEntryManager::StopPractice);
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
        if (Prefs::practiceTimeoutMode() == Prefs::EnumPracticeTimeoutMode::Show) {
            showSolution();
        } else if (Prefs::practiceTimeoutMode() == Prefs::EnumPracticeTimeoutMode::Continue) {
            signalResult(TestEntryManager::Timeout); ///@todo check if this works - esp with 3x timeout
            continueButtonClicked();
        }
    }
}


void PracticeDialog::startAnswerTimer()
{
    if (!Prefs::practiceTimeout()) {
        return;
    }

    int mqtime = Prefs::practiceTimeoutTimePerAnswer();
    if (mqtime > 0) {
        if (m_answerTimer == 0) {
            m_answerTimer = new QTimer(this);
            m_answerTimer->setSingleShot(true);
            connect(m_answerTimer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
        }
        m_answerTimerCount = mqtime;
        timebar()->setMaximum(m_answerTimerCount);
        timebar()->setValue(m_answerTimerCount);
        m_answerTimer->start(1000);
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
    m_validator->setTestEntry(m_entry);
    m_testType = Prefs::testType();
    startAnswerTimer();
    m_answerTainted = false;

    QString locale = m_doc->identifier(Prefs::solutionLanguage()).locale();

    if(!locale.isEmpty()) {
        LanguageSettings settings(locale);
        settings.readConfig();
        QString layout = settings.keyboardLayout();
        if(!layout.isEmpty()) {
            QDBusInterface kxkb( "org.kde.kxkb", "/kxkb", "org.kde.KXKB" );
            if (kxkb.isValid()) {
                kxkb.call( "setLayout", layout );
            }
        }
    }
}

void PracticeDialog::skipKnown()
{
    signalResult(TestEntryManager::SkipKnown);
    continueButtonClicked();
}

void PracticeDialog::skipUnknown()
{
    signalResult(TestEntryManager::SkipUnknown);
    continueButtonClicked();
}

void PracticeDialog::resultCorrect()
{
//     audioPlayCorrect();
    if (!m_answerTainted) {
        signalResult(TestEntryManager::Correct);
    } else {
        kDebug() << "Correct answer but with help (counts as wrong).";
        signalResult(TestEntryManager::Wrong);
    }
}

void PracticeDialog::resultWrong()
{
    signalResult(TestEntryManager::Wrong);
}

void PracticeDialog::audioPlayFromIdentifier()
{
    KUrl file = m_entry->entry()->translation(Prefs::questionLanguage())->soundUrl();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void PracticeDialog::audioPlayToIdentifier()
{
    KUrl file = m_entry->entry()->translation(Prefs::solutionLanguage())->soundUrl();
    if ( !file.isEmpty() ) {
        audioPlayFile(file);
    }
}

void PracticeDialog::audioPlayFile(const KUrl & soundFile)
{
    kDebug() << "Attempting to play sound: " << soundFile;

    if (!m_player) {
        m_player = new Phonon::MediaObject(this);
        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::NoCategory, this);
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

void PracticeDialog::imageShowFromEntry(QGraphicsView * view)
{
    if ( Prefs::practiceImagesEnabled() ) {
        QString url = m_entry->entry()->translation(Prefs::questionLanguage())->imageUrl().toLocalFile();
        if ( url.isEmpty() ) {
            url = m_entry->entry()->translation(Prefs::solutionLanguage())->imageUrl().toLocalFile();
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
    if ( Prefs::showSolutionAfterAnswer() ) {
        if (m_showSolutionTimer == 0) {
            m_showSolutionTimer = new QTimer(this);
        }
        m_showSolutionTimer->setSingleShot(true);
        connect(m_showSolutionTimer, SIGNAL(timeout()), SLOT(continueButtonClicked()));
        m_showSolutionTimer->start(Prefs::showSolutionAfterAnswerTime() * 1000);
    }
}


void PracticeDialog::continueButtonClicked()
{
    if ( m_showSolutionTimer ) {
        m_showSolutionTimer->stop();
    }
    emit showSolutionFinished();
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


#include "practicedialog.moc"
