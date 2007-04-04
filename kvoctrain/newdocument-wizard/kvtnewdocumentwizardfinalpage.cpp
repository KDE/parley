//
// C++ Implementation: kvtnewdocumentwizardintropage
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <KLocale>

#include "kvtnewdocumentwizardfinalpage.h"

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


