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

#include "StatisticsDialog.h"

#include "prefs.h"
#include "configure-practice/configurepracticedialog.h"
#include <keduvocdocument.h>
#include "lessonstatistics.h"
#include "statisticsmodel.h"

#include <KLocale>
#include <KConfig>
#include <KGlobal>

#include <QLayout>
#include <QLabel>

StatisticsDialog::StatisticsDialog(KEduVocDocument *doc, QWidget *parent, bool showPracticeButtons) : KDialog(parent), m_doc(doc)
{
    setModal(true);

    QWidget *mainWidget = new QWidget(this);
    setMainWidget(mainWidget);
    mainWidget->setLayout(new QVBoxLayout());
    QLabel *caption = new QLabel(this);
    mainWidget->layout()->addWidget(caption);

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

    StatisticsModel *statisticsModel = new StatisticsModel(this);
    statisticsModel->setDocument(doc);

    LessonStatisticsView *lessonStatistics = new LessonStatisticsView(this);
    mainWidget->layout()->addWidget(lessonStatistics);

    lessonStatistics->setModel(statisticsModel);
    lessonStatistics->expandToDepth(0);

    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    restoreDialogSize(cg);
}

StatisticsDialog::~StatisticsDialog()
{
    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    KDialog::saveDialogSize(cg);
}

void StatisticsDialog::configurePractice()
{
    ConfigurePracticeDialog dialog(m_doc, this, "practice settings",  Prefs::self());
    dialog.exec();
}

#include "StatisticsDialog.moc"
