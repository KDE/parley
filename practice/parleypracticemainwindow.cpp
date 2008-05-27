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

//#include "questiondisplay.h"
//#include "defaulttheme/vocabularycard.h"

#include "input.h"
#include "prompt.h"
#include "statistics.h"
#include "stdbuttons.h"
#include "hint.h"



ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
 : KXmlGuiWindow(parent)
{
    m_view = new QGraphicsView;
    setCentralWidget(m_view);
    setupGUI();

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

    //kDebug() << KStandardDirs::locate("data", "parley/images/card.svg");

    KEduVocDocument * doc = new KEduVocDocument(this);
    KUrl url = KFileDialog::getOpenUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), this, tr("Open Vocabulary Document"));
    int code = doc->open(url);
    kDebug() << code;
    
    
    TextualPrompt * prompt = new TextualPrompt();
    prompt->open(doc);
    prompt->setMinimumSize(100, 30);
    QGraphicsProxyWidget * gprompt = scene->addWidget(prompt);
    
    TextualInput * input = new TextualInput();
    prompt->setMinimumSize(100, 30);
    QGraphicsProxyWidget * ginput = scene->addWidget(input);
    
    LCDStatistics * stats = new LCDStatistics();
    QGraphicsProxyWidget * gstats = scene->addWidget(stats);
    stats->refresh();


    
    QGraphicsLinearLayout * promptAndInput = new QGraphicsLinearLayout();
    promptAndInput->setOrientation(Qt::Vertical);
    promptAndInput->addItem(gprompt);
    promptAndInput->addItem(ginput);
    
    QGraphicsWidget * gpromptAndInput = new QGraphicsWidget();
    gpromptAndInput->setLayout(promptAndInput);
    scene->addItem(gpromptAndInput);

    

    
    StdButtons * stdbuttons = new StdButtons();
    QGraphicsProxyWidget * gstdbuttons = scene->addWidget(stdbuttons);
    
    Hint * hint = new Hint(this);
    QPushButton * showAnswerButton = new QPushButton("Show Answer");
    QGraphicsProxyWidget * gshowAnswerButton = scene->addWidget(showAnswerButton);


    QGraphicsLinearLayout * buttons = new QGraphicsLinearLayout();
    buttons->addItem(gshowAnswerButton);
    buttons->addItem(gstdbuttons);

    QGraphicsWidget * gbuttons = new QGraphicsWidget();
    gbuttons->setLayout(buttons);
    scene->addItem(gbuttons);
    
    
    connect(showAnswerButton, SIGNAL(clicked()), hint, SLOT(slotShowAnswer()));
    
    connect(prompt, SIGNAL(signalAnswerChanged(KEduVocTranslation*)), input, SLOT(slotSetAnswer(KEduVocTranslation*)));
    connect(prompt, SIGNAL(signalPromptChanged(KEduVocExpression*)), stats, SLOT(slotSetPrompt(KEduVocExpression*)));
    connect(prompt, SIGNAL(signalSetFinished()), stats, SLOT(slotSetFinished()));
    // TODO sound and images

    connect(input, SIGNAL(signalCorrect()), stats, SLOT(slotCorrect()));
    connect(input, SIGNAL(signalIncorrect(Statistics::ErrorType)), stats, SLOT(slotIncorrect(Statistics::ErrorType)));
    connect(input, SIGNAL(returnPressed()), stdbuttons, SLOT(slotReturnPressed()));

    connect(stdbuttons, SIGNAL(signalCheckAnswer()), input, SLOT(slotCheckAnswer()));
    connect(stdbuttons, SIGNAL(signalSkipped(Statistics::SkipReason)), stats, SLOT(slotSkipped(Statistics::SkipReason)));
    connect(stdbuttons, SIGNAL(signalSkipped(Statistics::SkipReason)), prompt, SLOT(slotNewPrompt()));
    connect(stdbuttons, SIGNAL(signalContinue()), prompt, SLOT(slotNewPrompt()));

    connect(hint, SIGNAL(signalShowAnswer()), input, SLOT(slotShowAnswer()));
    connect(hint, SIGNAL(signalShowAnswer()), stats, SLOT(slotAnswerShown()));
    connect(hint, SIGNAL(signalShowAnswer()), stdbuttons, SLOT(slotAnswerShown()));
    
    //gprompt->setPos(30, 100);
    //ginput->setPos(60, 60);
    //gstdbuttons->setPos(200, 200);
    gstats->setPos(200, 100);
    //gshowAnswerButton->setPos(100, 200);

    gpromptAndInput->setPos(10, 10);
    gbuttons->setPos(10, 200);
    
    //kDebug() << KStandardDirs::locate("data", "defaulttheme/layout.svg");

    prompt->slotNewPrompt();

    m_view->show();
}

/*
ParleyPracticeMainWindow::~ParleyPracticeMainWindow()
{
}*/

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


