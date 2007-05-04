/***************************************************************************

                             kvtnewdocumentwizardfinalpage

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kvtnewdocumentwizardfinalpage.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>

#include <KLocale>

KVTNewDocumentWizardFinalPage::KVTNewDocumentWizardFinalPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(i18n("Finished"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(i18n("Now you can start entering your vocabulary directly. "
                           "Have fun!"));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

#include "kvtnewdocumentwizardfinalpage.moc"
