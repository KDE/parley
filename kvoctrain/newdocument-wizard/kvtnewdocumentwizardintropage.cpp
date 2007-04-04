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

#include "kvtnewdocumentwizardintropage.h"

KVTNewDocumentWizardIntroPage::KVTNewDocumentWizardIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(i18n("Introduction"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(i18n("This wizard will get you started "
                            "setting up your new document.\n"
                            "First you can specifiy which languages "
                            "you will be using. Than you are done."
                            ));
    /// @todo eventually option to open a file, if this is the first time kvoc is started???
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

#include "kvtnewdocumentwizardintropage.moc"


