/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "guifrontend.h"

#include <QtCore/QTimer>

#include <kcolorscheme.h>
#include <kstandarddirs.h>
#include <kdebug.h>

#include "ui_practice_mainwindow.h"

#include "settings/kgametheme/kgametheme.h"
#include "conjugationmodewidget.h"
#include "flashcardmodewidget.h"
#include "mixedlettersmodewidget.h"
#include "multiplechoicemodewidget.h"
#include "themedbackgroundrenderer.h"
#include "writtenpracticewidget.h"

using namespace Practice;

GuiFrontend::GuiFrontend(QWidget* parent)
    : AbstractFrontend(parent), m_modeWidget(0), m_lastImage("invalid"), m_currentBox(0), m_newBoxIfCorrect(0), m_newBoxIfWrong(0)
{
    m_widget = new ImageWidget();
    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    m_ui = new Ui::PracticeMainWindow();
    m_ui->setupUi(m_widget);
    m_ui->centralPracticeWidget->setLayout(new QHBoxLayout());

    m_themedBackgroundRenderer = new ThemedBackgroundRenderer(this);
    
    connect(Prefs::self(), SIGNAL(configChanged()), this, SLOT(setTheme()));
    connect(Prefs::self(), SIGNAL(configChanged()), this, SLOT(updateBackground()));
    setTheme();
    
    m_widget->setContentsMargins(m_themedBackgroundRenderer->contentMargins());
    m_ui->boxesWidget->setRenderer(m_themedBackgroundRenderer);
    m_ui->statusToggle->setRenderer(m_themedBackgroundRenderer);
    connect(m_themedBackgroundRenderer, SIGNAL(backgroundChanged(QPixmap)), this, SLOT(backgroundChanged(QPixmap)));
    connect(m_widget, SIGNAL(sizeChanged()), this, SLOT(updateBackground()));

    connect(m_ui->continueButton, SIGNAL(clicked()), this, SIGNAL(continueAction()));
    connect(m_ui->answerLaterButton, SIGNAL(clicked()), this, SIGNAL(skipAction()));
    connect(m_ui->hintButton, SIGNAL(clicked()), this, SIGNAL(hintAction()));
    connect(m_ui->statusToggle, SIGNAL(toggle()), this, SLOT(resultToggleClicked()));
    connect(m_ui->countAsCorrectButton, SIGNAL(clicked()), this, SLOT(countAsCorrectButtonClicked()));
    connect(m_ui->countAsWrongButton, SIGNAL(clicked()), this, SLOT(countAsWrongButtonClicked()));

    m_ui->centralPracticeWidget->installEventFilter(this);
    m_ui->imageWidget->installEventFilter(this);
    m_ui->rightContainer->installEventFilter(this);

    QTimer::singleShot(0, this, SLOT(updateBackground()));
}

GuiFrontend::~GuiFrontend()
{
    delete m_widget;
}

QVariant GuiFrontend::userInput()
{
    return m_modeWidget->userInput();
}

QWidget* GuiFrontend::widget()
{
    return m_widget;
}

void GuiFrontend::setMode(Mode mode)
{
    kDebug() << "setCentralWidget" << mode;
    AbstractModeWidget *newWidget = 0;
    switch(mode) {
        case Written:
            if (/*m_modeWidget->metaObject()->className() == QLatin1String("WrittenPracticeWidget")*/false) {
                kDebug() << "Written practice widget is already the central widget";
                break;
            }
            newWidget = new WrittenPracticeWidget(this, m_widget);
            break;
        case MultipleChoice:
            newWidget = new MultiplechoiceModeWidget(this, m_widget);
            break;
        case FlashCard:
            newWidget = new FlashCardModeWidget(this, m_widget);
            break;
        case MixedLetters:
            newWidget = new MixedLettersModeWidget(this, m_widget);
            break;
        case Conjugation:
            newWidget = new ConjugationModeWidget(this, m_widget);
            break;
        default:
            Q_ASSERT("Practice Mode Invalid" == 0);
    }
    if (newWidget) {
        m_ui->centralPracticeWidget->layout()->addWidget(newWidget);
        delete m_modeWidget;
        m_modeWidget = newWidget;
        connect(m_modeWidget, SIGNAL(continueAction()), m_ui->continueButton, SLOT(animateClick()));
    }
    m_ui->buttonStack->setCurrentIndex(0);
}

void GuiFrontend::setLessonName(const QString& lessonName)
{
    m_ui->lessonLabel->setText(i18nc("Display of the current lesson during practice", "Lesson: %1", lessonName));
}

void GuiFrontend::showQuestion()
{
    m_ui->answerLaterButton->setEnabled(true);
    m_ui->hintButton->setEnabled(true);
    m_ui->continueButton->setFocus();
    m_modeWidget->showQuestion();
}

void GuiFrontend::showSolution()
{
    m_ui->continueButton->setFocus();
    m_modeWidget->showSolution();
    m_ui->answerLaterButton->setEnabled(false);
    m_ui->hintButton->setEnabled(false);
}
void GuiFrontend::setSynonym(const QString& entry)
{
    m_modeWidget->setSynonym(entry); 
}

void GuiFrontend::showSynonym()
{
    m_modeWidget->showSynonym();
}


void GuiFrontend::setBoxes(int currentBox, int newBoxIfCorrect, int newBoxIfWrong)
{
    m_ui->boxesWidget->setBoxes(currentBox, 0);
    m_currentBox = currentBox;
    m_newBoxIfCorrect = newBoxIfCorrect;
    m_newBoxIfWrong = newBoxIfWrong;
}

void GuiFrontend::backgroundChanged(const QPixmap &pixmap)
{
    m_widget->setPixmap(pixmap);
}

void GuiFrontend::showSetResultButtons(bool show)
{
    m_ui->buttonStack->setCurrentIndex(int(show));
    if (show) {
        m_ui->countAsCorrectButton->setFocus();
    }
}

bool GuiFrontend::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Resize)
        updateBackground();
    return AbstractFrontend::eventFilter(object, event);
}

void GuiFrontend::setFinishedWordsTotalWords(int finished, int total)
{
    // update progress bar
    m_ui->totalProgress->setMaximum(total);
    m_ui->totalProgress->setValue(finished);
    m_ui->totalProgress->setToolTip(i18n("You answered %1 of a total of %2 words.\nYou are %3% done.", finished, total, finished/total*100));
}

void GuiFrontend::setHint(const QVariant& hint)
{
    m_modeWidget->setHint(hint);
}

void GuiFrontend::setQuestion(const QVariant& question)
{
    m_modeWidget->setQuestion(question);
}


void GuiFrontend::setQuestionPronunciation(const QString& pronunciationText)
{
    m_modeWidget->setQuestionPronunciation(pronunciationText);
}

void GuiFrontend::setQuestionSound(const KUrl& soundUrl)
{
    m_modeWidget->setQuestionSound(soundUrl);
}

void GuiFrontend::setSolution(const QVariant& solution)
{
    m_modeWidget->setSolution(solution);
}

void GuiFrontend::setQuestionImage(const KUrl& image)
{
    setImage(image);
}

void GuiFrontend::setSolutionImage(const KUrl& image)
{
    if (image.path().isEmpty()) {
        return;
    }
    setImage(image);
}

void GuiFrontend::setImage(const KUrl& image)
{
    if (m_lastImage == image) {
        return;
    }    
    QPixmap pixmap(image.path());
    if (pixmap.isNull()) {
        m_ui->imageWidget->setPixmap(m_themedBackgroundRenderer->getPixmapForId("image-placeholder", QSize(150, 150)));
    } else {
        m_ui->imageWidget->setPixmap(pixmap);
    }
    m_lastImage = image;
}

void GuiFrontend::setSolutionPronunciation(const QString& pronunciationText)
{
    m_modeWidget->setSolutionPronunciation(pronunciationText);
}

void GuiFrontend::setSolutionSound(const KUrl& soundUrl)
{
    m_modeWidget->setSolutionSound(soundUrl);
}

void GuiFrontend::setFeedback(const QVariant& feedback)
{
    m_modeWidget->setFeedback(feedback);
}

void GuiFrontend::setFeedbackState(ResultState feedbackState)
{
    m_feedbackState = feedbackState;
    m_modeWidget->setFeedbackState(feedbackState);
}

void GuiFrontend::setResultState(ResultState resultState)
{
    // TODO: temporary text labels instead of graphics
//    m_ui->statusImageLabel->setFont(QFont("", 80, QFont::Bold));
    m_ui->statusToggle->setResultState(resultState);
    switch (resultState) {
    case AbstractFrontend::QuestionState:
    case AbstractFrontend::AnswerSynonym:
//        m_ui->statusImageLabel->setText("?");
//        m_ui->toggleButton->setEnabled(false);
//        m_ui->toggleButton->setText(QString(0x2717)+QChar(0x2192)+QChar(0x2713));
        m_ui->boxesWidget->setBoxes(m_currentBox);
        break;
    case AbstractFrontend::AnswerCorrect:
//        m_ui->statusImageLabel->setText(QChar(0x2713));
//        m_ui->toggleButton->setEnabled(true);
//        m_ui->toggleButton->setText(QString(0x2713)+QChar(0x2192)+QChar(0x2717));
        m_ui->boxesWidget->setBoxes(m_newBoxIfCorrect, m_currentBox);
        break;

//        m_ui->statusImageLabel->setText("?");
//        m_ui->toggleButton->setEnabled(true);
//        m_ui->toggleButton->setText(QString(0x2713)+QChar(0x2192)+QChar(0x2717));
//        m_ui->boxesWidget->setBoxes(m_currentBox);
//        break;
    case AbstractFrontend::AnswerWrong:
//        m_ui->statusImageLabel->setText(QChar(0x2717));
//        m_ui->toggleButton->setEnabled(true);
//        m_ui->toggleButton->setText(QString(0x2717)+QChar(0x2192)+QChar(0x2713));
        m_ui->boxesWidget->setBoxes(m_newBoxIfWrong, m_currentBox);
        break;
    }

    m_resultState = resultState;
    m_modeWidget->setResultState(resultState);
}

AbstractFrontend::ResultState GuiFrontend::resultState()
{
    return m_resultState;
}

void GuiFrontend::countAsCorrectButtonClicked()
{
    m_resultState = AnswerCorrect;
    emit continueAction();
}

void GuiFrontend::countAsWrongButtonClicked()
{
    m_resultState = AnswerWrong;
    emit continueAction();
}

void GuiFrontend::resultToggleClicked()
{
    if (resultState() == AnswerWrong) {
        setResultState(AnswerCorrect);
    } else {
        setResultState(AnswerWrong);
    }
}

void GuiFrontend::updateBackground()
{
    m_themedBackgroundRenderer->setSize(m_widget->size());
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect("image", m_ui->imageWidget->frameGeometry());
    m_themedBackgroundRenderer->addRect("central", m_ui->centralPracticeWidget->frameGeometry());
    m_themedBackgroundRenderer->addRect("buttons", m_ui->rightContainer->frameGeometry());
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}

void GuiFrontend::setTheme()
{
    KGameTheme theme;
    theme.load(Prefs::theme());
    m_themedBackgroundRenderer->setSvgFilename(theme.graphics());
}

#include "guifrontend.moc"
