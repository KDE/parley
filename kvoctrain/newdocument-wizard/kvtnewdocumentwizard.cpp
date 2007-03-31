//
// C++ Implementation: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QLabel>
#include <QVBoxLayout>
#include <KLocale>

#include "kvtnewdocumentwizard.h"
#include "kvtnewdocumentwizardintropage.h"

KVTNewDocumentWizard::KVTNewDocumentWizard(QWidget *parent)
 : QWizard(parent)
{
    addPage(new KVTNewDocumentWizardIntroPage);
    /*
    addPage(new ClassInfoPage);
    addPage(new CodeStylePage);
    addPage(new OutputFilesPage);
    addPage(new ConclusionPage);
*/
    //setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(i18n("Create a new KVocTrain document"));
}

/*
KVTNewDocumentWizard::~KVTNewDocumentWizard()
{
}
*/
/*
void KVTNewDocumentWizard::initializePage(int id)
{
    QWizard::initializePage(id);
}
*/
void KVTNewDocumentWizard::accept()
{
    /*
    QByteArray className = field("className").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();
    */
    QDialog::accept();
}

#include "kvtnewdocumentwizard.moc"
