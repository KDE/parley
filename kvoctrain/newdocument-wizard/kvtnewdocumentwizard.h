//
// C++ Interface: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWDOCUMENTWIZARD_H
#define KVTNEWDOCUMENTWIZARD_H

#include <QWizard>


/**
A wizard to assist in the creation of kvtml documents with KVocTrain. Helps to set up the languages of the document.

	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizard : public QWizard
{
    Q_OBJECT

public:
    KVTNewDocumentWizard(QWidget *parent = 0);

 //   ~KVTNewDocumentWizard();

 //   virtual void initializePage(int id);
    void accept();
};

#endif
