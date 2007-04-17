//
// C++ Interface: kvtnewdocumentwizardintropage
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef KVTNEWDOCUMENTWIZARDINTROPAGE_H
#define KVTNEWDOCUMENTWIZARDINTROPAGE_H

#include <QWizardPage>
#include <QLabel>

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardIntroPage : public QWizardPage
{
    Q_OBJECT

public:
    KVTNewDocumentWizardIntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

#endif
