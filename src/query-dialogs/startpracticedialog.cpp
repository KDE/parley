/***************************************************************************

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "startpracticedialog.h"
#include "startpracticewidget.h"
#include <KLocale>

StartPracticeDialog::StartPracticeDialog(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Start Practice"));
    setButtons(Ok|Cancel);
    setDefaultButton(Ok);
    setFaceType(List);

    m_startPracticeWidget= new StartPracticeWidget(doc, this);
    m_startPracticePage = new KPageWidgetItem(m_startPracticeWidget, i18n("Test") );
    m_startPracticePage->setHeader( i18n("Test options") );
    m_startPracticePage->setIcon( KIcon( "run_query" ) );
    addPage(m_startPracticePage);


//     typeOptPage = new WordTypeOptionPage(doc, 0);
//     addPage(typeOptPage, i18nc("word types","Types"));
//
//     tenseOptPage = new TenseOptPage(doc, 0);
//     addPage(tenseOptPage, i18n("Tenses"));
//
//     useOptPage = new UsageOptPage(doc, 0);
//     addPage(useOptPage, i18nc("usage (area) of an expression", "Usage"));

}

StartPracticeDialog::~StartPracticeDialog()
{
//     KConfigGroup cg(KGlobal::config(), "DocumentPropertiesDialog");
//     KDialog::saveDialogSize(cg);
}


void StartPracticeDialog::commitData()
{
    m_startPracticeWidget->commitData();
}

void StartPracticeDialog::accept()
{
    commitData();
    QDialog::accept();
}



#include "startpracticedialog.moc"


