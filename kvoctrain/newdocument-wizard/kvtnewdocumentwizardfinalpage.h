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
#ifndef KVTNEWDOCUMENTWIZARDFINALPAGE_H
#define KVTNEWDOCUMENTWIZARDFINALPAGE_H

#include <QWizardPage>
#include <QLabel>

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizardFinalPage : public QWizardPage
{
    Q_OBJECT

public:
    KVTNewDocumentWizardFinalPage(QWidget *parent = 0);

private:
    QLabel *label;
};

#endif
