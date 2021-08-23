/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "guifrontend.h"

#include <QTimer>

#include <KColorScheme>
#include <QDebug>
#include <KLocalizedString>

#include "ui_practice_mainwindow.h"

#include "settings/kgametheme/kgametheme.h"
#include "comparisonmodewidget.h"
#include "conjugationmodewidget.h"
#include "flashcardmodewidget.h"
#include "mixedlettersmodewidget.h"
#include "multiplechoicemodewidget.h"
#include "themedbackgroundrenderer.h"
#include "writtenpracticewidget.h"

using namespace Practice;

GuiFrontend::GuiFrontend(QWidget *parent)
    : AbstractFrontend(parent)
    , m_widget(new ImageWidget())
    , m_ui(new Ui::PracticeMainWindow())
    , m_modeWidget(nullptr)
    , m_resultState(AbstractFrontend::AnswerWrong)
    , m_feedbackState(AbstractFrontend::AnswerWrong)
    , m_currentBox(0)
    , m_newBoxIfCorrect(0)
    , m_newBoxIfWrong(0)
    , m_themedBackgroundRenderer(new ThemedBackgroundRenderer(this, QStringLiteral("practicethemecache.bin")))
{
    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    m_ui->setupUi(m_widget);
    m_ui->centralPracticeWidget->setLayout(new QHBoxLayout());

    connect(Prefs::self(), &Prefs::configChanged, this, &GuiFrontend::setTheme);
    setTheme();

    connect(m_themedBackgroundRenderer, &ThemedBackgroundRenderer::backgroundChanged, this, &GuiFrontend::backgroundChanged);
    connect(m_widget, &ImageWidget::sizeChanged, this, &GuiFrontend::updateBackground);

    connect(m_ui->continueButton, &QPushButton::clicked, this, &GuiFrontend::continueAction);
    connect(m_ui->answerLaterButton, &QPushButton::clicked, this, &GuiFrontend::skipAction);
    connect(m_ui->hintButton, &QPushButton::clicked, this, &GuiFrontend::hintAction);
    connect(m_ui->statusToggle, &Practice::StatusToggle::toggle, this, &GuiFrontend::toggleResultState);
    connect(m_ui->countAsCorrectButton, &QPushButton::clicked, this, &GuiFrontend::countAsCorrectButtonClicked);
    connect(m_ui->countAsWrongButton, &QPushButton::clicked, this, &GuiFrontend::countAsWrongButtonClicked);

    m_ui->centralPracticeWidget->installEventFilter(this);
    m_ui->imageWidget->installEventFilter(this);
    m_ui->rightContainer->installEventFilter(this);

    QTimer::singleShot(0, this, &GuiFrontend::updateBackground);
}

GuiFrontend::~GuiFrontend()
{
    delete m_widget;
    delete m_ui;
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
    qDebug() << "setCentralWidget" << mode;
    AbstractModeWidget *newWidget = 0;
    switch (mode) {
    case Written:
        if (/*m_modeWidget->metaObject()->className() == QLatin1String("WrittenPracticeWidget")*/false) {
            qDebug() << "Written practice widget is already the central widget";
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
    case Comparison:
        newWidget = new ComparisonModeWidget(this, m_widget);
        break;
    case ExampleSentence:
        newWidget = new WrittenPracticeWidget(this, m_widget, true);
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
    updateFontColors();
}

void GuiFrontend::setLessonName(const QString& lessonName)
{
    m_ui->lessonLabel->setText(i18nc("Display of the current unit during practice", "Unit: %1",
                                     lessonName));
}

void GuiFrontend::showGrade(int preGrade, int grade)
{
    if (preGrade == 0 && grade == 0) {
        m_ui->gradeLabel->setText(i18n("New word"));
    }
    else {
        m_ui->gradeLabel->setText(i18nc("Display of the current confidence level during practice, 1st param is either initial or long term",
                                        "%1, confidence %2",
                                        grade == 0 ? i18n("initial") : i18n("long term"),
                                        QString::number(grade == 0 ? preGrade : grade)));
    }
}

void GuiFrontend::showQuestion()
{
    m_ui->answerLaterButton->setEnabled(true);
    m_ui->hintButton->setEnabled(Prefs::showHints());
    QTimer::singleShot(0, m_ui->continueButton, SLOT(setFocus()));
    m_modeWidget->showQuestion();
    setImage(m_questionImage);
}

void GuiFrontend::showSolution()
{
    m_ui->continueButton->setFocus();
    m_modeWidget->showSolution();
    m_ui->answerLaterButton->setEnabled(false);
    m_ui->hintButton->setEnabled(false);
    setImage(m_solutionImage);
}
void GuiFrontend::setSynonym(const QString& entry)
{
    m_modeWidget->setSynonym(entry);
}

void GuiFrontend::showSynonym()
{
    m_modeWidget->showSynonym();
}

void GuiFrontend::setBoxes(grade_t currentBox, grade_t newBoxIfCorrect, grade_t newBoxIfWrong)
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
    int finishedPercentage = 1.0 * finished / total * 100;
    // update progress bar
    m_ui->totalProgress->setMaximum(total);
    m_ui->totalProgress->setValue(finished);
    m_ui->totalProgress->setToolTip(i18np("You answered correctly %2 of a total of %1 word.\nYou are %3% done.",
                                          "You answered correctly %2 of a total of %1 words.\nYou are %3% done.",
                                          total, finished, finishedPercentage));
}

QFont GuiFrontend::knownLangFont() const
{
    return m_knownLangFont;
}

QFont GuiFrontend::learningLangFont() const
{
    return m_learningLangFont;
}

void GuiFrontend::setKnownLangFont(const QFont& font)
{
    m_knownLangFont = font;
}

void GuiFrontend::setLearningLangFont(const QFont& font)
{
    m_learningLangFont = font;
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

void GuiFrontend::setQuestionSound(const QUrl& soundUrl)
{
    m_modeWidget->setQuestionSound(soundUrl);
}

void GuiFrontend::setSolution(const QVariant& solution)
{
    m_modeWidget->setSolution(solution);
}

void GuiFrontend::setQuestionImage(const QUrl& image)
{
    m_questionImage = image;
}

void GuiFrontend::setSolutionImage(const QUrl& image)
{
    m_solutionImage = image;
}

void GuiFrontend::setQuestionFont(const QFont& font)
{
    m_modeWidget->setQuestionFont(font);
}

void GuiFrontend::setSolutionFont(const QFont& font)
{
    m_modeWidget->setSolutionFont(font);
}

void GuiFrontend::setImage(const QUrl& image)
{
    if (m_lastImage == image) {
        return;
    }
    QPixmap pixmap(image.path());
    if (pixmap.isNull()) {
        m_ui->imageWidget->setPixmap(m_themedBackgroundRenderer->getPixmapForId(QStringLiteral("image-placeholder"), QSize(150, 150)));
    } else {
        m_ui->imageWidget->setPixmap(pixmap);
    }
    m_lastImage = image;
}

void GuiFrontend::setSolutionPronunciation(const QString& pronunciationText)
{
    m_modeWidget->setSolutionPronunciation(pronunciationText);
}

void GuiFrontend::setSolutionSound(const QUrl& soundUrl)
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

void GuiFrontend::toggleResultState()
{
    if (resultState() == AnswerWrong) {
        setResultState(AnswerCorrect);
    } else {
        setResultState(AnswerWrong);
    }
}

void GuiFrontend::updateBackground()
{
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect(QStringLiteral("background"), QRect(QPoint(), m_widget->size()));
    m_themedBackgroundRenderer->addRect(QStringLiteral("image"), m_ui->imageWidget->frameGeometry());
    m_themedBackgroundRenderer->addRect(QStringLiteral("central"), m_ui->centralPracticeWidget->frameGeometry());
    m_themedBackgroundRenderer->addRect(QStringLiteral("buttons"), m_ui->rightContainer->frameGeometry());
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}

void GuiFrontend::updateFontColors()
{
    QPalette p(QApplication::palette());
    QColor c = m_themedBackgroundRenderer->fontColor(QStringLiteral("Outer"), p.color(QPalette::Active, QPalette::WindowText));
    p.setColor(QPalette::WindowText, c);
    m_ui->lessonLabel->setPalette(p);
    m_ui->gradeLabel->setPalette(p);

    if (m_modeWidget) {
        p = QApplication::palette();
        c = m_themedBackgroundRenderer->fontColor(QStringLiteral("Central"), p.color(QPalette::Active, QPalette::WindowText));
        p.setColor(QPalette::Active, QPalette::WindowText, c);
        p.setColor(QPalette::Inactive, QPalette::WindowText, c);
        m_modeWidget->setPalette(p);

        KColorScheme scheme(QPalette::Active);
        QPalette correctPalette = QApplication::palette();
        c = m_themedBackgroundRenderer->fontColor(QStringLiteral("Correct"), scheme.foreground(KColorScheme::PositiveText).color());
        correctPalette.setColor(QPalette::WindowText, c);
        correctPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::PositiveText).color());
        QPalette wrongPalette = QApplication::palette();
        c = m_themedBackgroundRenderer->fontColor(QStringLiteral("Wrong"), scheme.foreground(KColorScheme::NegativeText).color());
        wrongPalette.setColor(QPalette::WindowText, c);
        wrongPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::NegativeText).color());

        m_modeWidget->setResultPalettes(correctPalette, wrongPalette);
    }
}

void GuiFrontend::setTheme()
{
    m_themedBackgroundRenderer->setTheme(Prefs::theme());
    updateFontColors();
    updateBackground();
    m_widget->setContentsMargins(m_themedBackgroundRenderer->contentMargins());
    m_ui->boxesWidget->setRenderer(m_themedBackgroundRenderer);
    m_ui->statusToggle->setRenderer(m_themedBackgroundRenderer);
}
