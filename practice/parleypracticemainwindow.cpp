/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "parleypracticemainwindow.h"

#include <KXmlGuiWindow>
#include <KLineEdit>
#include <KDebug>
#include <KStandardDirs>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QLCDNumber>
#include <KFileDialog>
#include <QPushButton>
#include <QGraphicsLinearLayout>
#include <KAction>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>

#include "input.h"
#include "prompt.h"
#include "statistics.h"
#include "stdbuttons.h"
#include "hint.h"
#include "testentrymanager.h"
#include "answervalidator.h"

#include "../../libkdeedu/keduvocdocument/keduvocexpression.h"
#include "../../libkdeedu/keduvocdocument/keduvoctranslation.h"
#include "../../libkdeedu/keduvocdocument/keduvocdocument.h"

ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
 : KXmlGuiWindow(parent)
{

    //// Basic setup ////
    m_view = new QGraphicsView;
    setCentralWidget(m_view);

    QGraphicsScene* scene = new QGraphicsScene(this);
    m_view->setScene(scene);

    scene->setSceneRect(0.0, 0.0, 600.0, 300.0);

    m_view->setSceneRect(scene->sceneRect());

    m_view->installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QGraphicsSvgItem * backgroundsvg = new QGraphicsSvgItem();
    KSvgRenderer * krenderer = new KSvgRenderer(QString("~/background.svgz"));
    backgroundsvg->setSharedRenderer(krenderer);
//    backgroundsvg->setSize(600,300);
    scene->addItem(backgroundsvg);


    //// Loading the Document -- temporary ////
    KEduVocDocument * doc = new KEduVocDocument(this);
    KUrl url = KFileDialog::getOpenUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), this, i18n("Open Vocabulary Document"));
    int code = doc->open(url);
    kDebug() << code;
    
    // this is the only object/widget the window directly keeps track of (outside of the canvas, etc).
    m_manager = new TestEntryManager(this);
    m_manager->open(doc);


    ////// Graphical Widgets Setup //////

    TextualPrompt * prompt = new TextualPrompt();
    prompt->setMinimumSize(100, 30);
    QGraphicsProxyWidget * gprompt = scene->addWidget(prompt);
    connect(m_manager, SIGNAL(signalNewText(const QString&)), prompt, SLOT(setText(const QString&)));

    
    TextualInput * input = new TextualInput();
    input->setMinimumSize(100, 30);
    QGraphicsProxyWidget * ginput = scene->addWidget(input);

    Statistics * stats = new Statistics(this);
    LCDStatistics * lcdstats = new LCDStatistics();
    QGraphicsProxyWidget * glcdstats = scene->addWidget(lcdstats);
    connect(stats, SIGNAL(signalUpdateStatisticsDisplay(Statistics*)), lcdstats, SLOT(slotUpdateStatisticsDisplay(Statistics*)));
    
    QGraphicsLinearLayout * promptAndInput = new QGraphicsLinearLayout();
    promptAndInput->setOrientation(Qt::Vertical);
    promptAndInput->addItem(gprompt);
    promptAndInput->addItem(ginput);
    
    QGraphicsWidget * gpromptAndInput = new QGraphicsWidget();
    gpromptAndInput->setLayout(promptAndInput);
    scene->addItem(gpromptAndInput);

    StdButton * stdbutton = new StdButton();
    QGraphicsProxyWidget * gstdbutton = scene->addWidget(stdbutton);



    //// Input and Validation Setup ////
    AnswerValidator * validator = new AnswerValidator(doc, this);
    validator->setLanguage(1); // TODO do this for real...
    connect(input, SIGNAL(signalInput(const QString&)), this, SLOT(slotGetInput(const QString&)));
    connect(this, SIGNAL(signalCheckInput(const QString&, const QString&)), validator, SLOT(checkUserAnswer(const QString&, const QString&)));
    connect(this, SIGNAL(signalCorrection(float, const QString&)), stats, SLOT(slotCorrection(float, const QString&)));
    

    /////////// KAction Setup /////////////

    //// Skip Action Setup ////
    KAction *skipKnownAction = new KAction(this);
    skipKnownAction->setText(i18n("Skip (Answer Known)"));
    actionCollection()->addAction("skip known", skipKnownAction);
    connect(skipKnownAction, SIGNAL(triggered()), stats, SLOT(slotSkipKnown()));

    KAction *skipUnknownAction = new KAction(this);
    skipUnknownAction->setText(i18n("Skip (Answer Not Known)"));
    actionCollection()->addAction("skip unknown", skipUnknownAction);
    connect(skipUnknownAction, SIGNAL(triggered()), stats, SLOT(slotSkipUnknown()));


    //// Hint + Hint Action Setup ////
    Hint * hint = new Hint(this);
    KAction *hintAction = new KAction(this);
    hintAction->setText(i18n("Show Hint"));
    actionCollection()->addAction("hint", hintAction);
    connect(hintAction, SIGNAL(triggered()), hint, SLOT(slotShowHint()));    
    connect(hint, SIGNAL(signalShowHint()), hint, SIGNAL(signalShowSolution())); // this is the hint for now :)
    connect(hint, SIGNAL(signalAnswerTainted(Statistics::TaintReason)), stats, SLOT(slotTaintAnswer(Statistics::TaintReason)));


    //// Show Solution Setup ////
    KAction *showSolutionAction = new KAction(this);
    showSolutionAction->setText(i18n("Show Solution"));
    actionCollection()->addAction("show solution", showSolutionAction);
    connect(showSolutionAction, SIGNAL(triggered()), stats, SLOT(slotSolutionShown()));
    connect(showSolutionAction, SIGNAL(triggered()), stdbutton, SLOT(slotSolutionShown()));
    connect(showSolutionAction, SIGNAL(triggered()), this, SLOT(slotShowSolution()));
    connect(this, SIGNAL(signalShowSolution(const QString&)), input, SLOT(slotShowSolution(const QString&)));

    //// Check Answer Setup ////
    KAction *checkAnswerAction = new KAction(this);
    checkAnswerAction->setText(i18n("Check Answer"));
    actionCollection()->addAction("check answer", checkAnswerAction);
    connect(stdbutton, SIGNAL(signalCheckAnswer()), checkAnswerAction, SIGNAL(triggered()));
    connect(checkAnswerAction, SIGNAL(triggered()), input, SLOT(emitCurrentInput()));

    //// Continue Action Setup ////
    KAction *continueAction = new KAction(this);
    continueAction->setText(i18n("Continue"));
    actionCollection()->addAction("continue", continueAction);
    connect(stdbutton, SIGNAL(signalContinue()), continueAction, SIGNAL(triggered()));
    connect(continueAction, SIGNAL(triggered()), m_manager, SLOT(slotNewEntry()));        
    



    //// Final Graphics Setup ////
    
    glcdstats->setPos(200, 100);
    gpromptAndInput->setPos(10, 10);
    gstdbutton->setPos(10, 200);
    

    setupGUI();

    // ... and we are done -- start the first question!
    m_manager->slotNewEntry();
    
    m_view->show();
}


void ParleyPracticeMainWindow::slotGetInput(const QString& input)
{
    emit signalCheckInput(m_manager->currentSolution(), input);
}

void ParleyPracticeMainWindow::slotShowSolution()
{
    emit signalShowSolution(m_manager->currentSolution());
}

bool ParleyPracticeMainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        QMatrix matrix;
        matrix.scale(resizeEvent->size().width()/800.0, resizeEvent->size().height()/600.0);
        m_view->setMatrix(matrix, false);
        return QObject::eventFilter(obj, event);
    }
    return QObject::eventFilter(obj, event);
}


