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
