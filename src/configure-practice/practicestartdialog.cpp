/***************************************************************************
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicestartdialog.h"

#include "prefs.h"
#include "configure-practice/configurepracticedialog.h"
#include "keduvocdocument.h"
#include "statistics-dialogs/lessonstatistics.h"
#include "statistics-dialogs/statisticsmodel.h"

#include <KLocale>
#include <KConfig>
#include <KGlobal>

#include <QLayout>
#include <QLabel>

PracticeStartDialog::PracticeStartDialog(QWidget *parent, KEduVocDocument *doc)
 : KDialog(parent), m_doc(doc)
{
    setCaption(i18n("Start Practice"));
    setWindowIcon(KIcon("practice-start"));
    setButtons(Ok | Cancel | User1);
    setButtonFocus(Ok);
    setModal(true);

    QWidget *mainWidget = new QWidget(this);
    setMainWidget(mainWidget);
    mainWidget->setLayout(new QVBoxLayout());
    QLabel *label = new QLabel(i18n("Choose lessons to learn and start practice:"), this);
    mainWidget->layout()->addWidget(label);

    setButtonGuiItem(Ok, KGuiItem(i18n("Start Practice"), KIcon("practice-start"), i18n("Start Practice")));
    setButtonGuiItem(User1, KGuiItem(i18n("Configure Practice"), KIcon("practice-setup"), i18n("Configure practice settings")));

    StatisticsModel *statisticsModel = new StatisticsModel(this);
    statisticsModel->setDocument(doc);

    LessonStatisticsView *lessonStatistics = new LessonStatisticsView(this);
    mainWidget->layout()->addWidget(lessonStatistics);

    lessonStatistics->setModel(statisticsModel);
    lessonStatistics->expandToDepth(0);

    KConfigGroup cg(KGlobal::config(), "StartPracticeDialog");
    restoreDialogSize(cg);

    connect(this, SIGNAL(user1Clicked()), this, SLOT(configurePractice()));
}

PracticeStartDialog::~PracticeStartDialog()
{
    KConfigGroup cg(KGlobal::config(), "StartPracticeDialog");
    KDialog::saveDialogSize(cg);
}

void PracticeStartDialog::configurePractice()
{
    ConfigurePracticeDialog dialog(m_doc, this, "practice settings",  Prefs::self());
    dialog.exec();
}

#include "practicestartdialog.moc"
