/**********************************************************************

	--- Qt Architect generated file ---

	File: StatistikPageData.h
	Last generated: Thu Oct 4 17:42:18 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef StatistikPageData_included
#define StatistikPageData_included

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

class StatistikPageData : public QDialog
{
    Q_OBJECT

public:

    StatistikPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~StatistikPageData();

public slots:


protected slots:


protected:
    QLabel* num_entry;

};

#endif // StatistikPageData_included
