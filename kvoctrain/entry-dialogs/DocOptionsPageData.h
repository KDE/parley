/**********************************************************************

	--- Qt Architect generated file ---

	File: DocOptionsPageData.h
	Last generated: Sun Sep 30 11:33:49 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef DocOptionsPageData_included
#define DocOptionsPageData_included

#include <compat_2x.h> // string type macro 

#ifdef EA_KDE2x
#define EA_SPINBOX_H <qspinbox.h>
#define EA_SPINBOX   QSpinBox
#else
#define EA_SPINBOX_H <kspinbox.h>
#define EA_SPINBOX   KSpinBox
#endif
#include <qdialog.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>

class DocOptionsPageData : public QDialog
{
    Q_OBJECT

public:

    DocOptionsPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~DocOptionsPageData();

public slots:


protected slots:


protected:
    QCheckBox* docsort;
    QComboBox* c_coding;
    QLabel* l_coding;

};

#endif // DocOptionsPageData_included
