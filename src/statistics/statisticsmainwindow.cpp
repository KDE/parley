/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "statisticsmainwindow.h"

#include "prefs.h"
#include "configure-practice/configurepracticedialog.h"
#include <keduvocdocument.h>
#include "lessonstatistics.h"
#include "statisticsmodel.h"
#include "parleymainwindow.h"

#include <KLocale>
#include <KConfig>
#include <KGlobal>
#include <KActionCollection>
#include <KAction>

#include <QLayout>
#include <QLabel>

using namespace Editor;

StatisticsMainWindow::StatisticsMainWindow(KEduVocDocument* doc, ParleyMainWindow* parent) 
    :KXmlGuiWindow(parent)
    ,m_mainWindow(parent)
    ,m_doc(doc)
{
    // KXmlGui
    setXMLFile("statisticsui.rc");
    setObjectName("Statistics");
    
    QVBoxLayout *layout = new QVBoxLayout();
    
    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
    
    QLabel *caption = new QLabel(this);
    layout->addWidget(caption);
    caption->setText(i18n("Statistics for %1", m_doc->title()));

    /*
    if (showPracticeButtons) {
        // This enables the start and configure practice buttons
        setCaption(i18n("Start Practice"));
        setWindowIcon(KIcon("practice-start"));
        setButtons(Ok | Cancel | User1);
        setButtonFocus(Ok);
        setButtonGuiItem(Ok, KGuiItem(i18n("Start Practice"), KIcon("practice-start"), i18n("Start Practice")));
        setButtonGuiItem(User1, KGuiItem(i18n("Configure Practice"), KIcon("practice-setup"), i18n("Configure practice settings")));
        caption->setText(i18n("Select the lessons to practice:"));
        connect(this, SIGNAL(user1Clicked()), this, SLOT(configurePractice()));
    } else {
        // just an ok button, no start/configure practice
        setCaption(i18n("Document Statistics"));
        setButtons(Ok);
        caption->setText(i18n("Average grades for each lesson:"));
    }
    */

    m_statisticsModel = new StatisticsModel(this);

    m_lessonStatistics = new LessonStatisticsView(this);
    layout->addWidget(m_lessonStatistics);

    setDocument(doc);
    
    initActions();
    
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);  
}

StatisticsMainWindow::~StatisticsMainWindow()
{
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void StatisticsMainWindow::setDocument(KEduVocDocument* doc)
{
    m_doc = doc;
    m_statisticsModel->setDocument(doc);
    m_lessonStatistics->setModel(m_statisticsModel);
    m_lessonStatistics->expandToDepth(0);
}

void StatisticsMainWindow::initActions()
{
    KAction* editDocument = new KAction(this);
    editDocument->setText(i18n("Edit..."));
    editDocument->setIcon(KIcon("document-edit"));
    editDocument->setHelpText(i18n("Edit the document"));
    actionCollection()->addAction("document_edit", editDocument);
    connect(editDocument, SIGNAL(triggered()), m_mainWindow, SLOT(showEditor()));
    
    KAction* startPractice = new KAction(this);
    startPractice->setText(i18n("Start Practice..."));
    startPractice->setIcon(KIcon("practice-start"));
    startPractice->setWhatsThis(i18n("Start a test"));
    startPractice->setToolTip(startPractice->whatsThis());
    startPractice->setStatusTip(startPractice->whatsThis());
    actionCollection()->addAction("practice_start", startPractice);
    connect(startPractice, SIGNAL(triggered(bool)), m_mainWindow, SLOT(startPractice()));
    
    KAction* configurePractice = new KAction(this);
    configurePractice->setText(i18n("Configure Practice..."));
    configurePractice->setIcon(KIcon("practice-setup"));
    configurePractice->setWhatsThis(i18n("Change practice settings"));
    configurePractice->setToolTip(configurePractice->whatsThis());
    configurePractice->setStatusTip(configurePractice->whatsThis());
    actionCollection()->addAction("practice_configure", configurePractice);
    connect(configurePractice, SIGNAL(triggered(bool)), m_mainWindow, SLOT(configurePractice()));
}

void StatisticsMainWindow::configurePractice()
{
    ConfigurePracticeDialog dialog(m_doc, this, "practice settings",  Prefs::self());
    dialog.exec();
}

#include "statisticsmainwindow.moc"
