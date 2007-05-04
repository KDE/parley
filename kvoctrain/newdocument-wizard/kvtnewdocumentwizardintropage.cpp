/***************************************************************************

                             kvtnewdocumentwizardintropage

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

#include "kvtnewdocumentwizardintropage.h"

#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>

#include <KLocale>

KVTNewDocumentWizardIntroPage::KVTNewDocumentWizardIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(i18n("Introduction"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(i18n("This wizard will get you started "
                            "setting up your new document.\n\n"
                            "With KVocTrain you can practice anything that comes in pairs. For example foreign languages and their translation to your language.\n\n"
                            "To get started it is necessary to set up what you would like to practice.\n\n"
                            "On the next page you can choose to use languages or anything else. In that case you can simply define yourself what it is you practice by typing a title for it."
                            ));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

#include "kvtnewdocumentwizardintropage.moc"
