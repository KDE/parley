/**********************************************************************

	--- Qt Architect generated file ---

	File: GroupOptPageData.h
	Last generated: Sat Oct 13 21:49:58 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef GroupOptPageData_included
#define GroupOptPageData_included

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
#include <qpushbutton.h>
#include <qcombobox.h>

class GroupOptPageData : public QDialog
{
    Q_OBJECT

public:

    GroupOptPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~GroupOptPageData();

public slots:


protected slots:


protected:
    QLabel* l_name;
    QComboBox* ps_name;
    QPushButton* ps_store;
    QPushButton* ps_recall;
    QPushButton* ps_new;
    QPushButton* ps_del;

};

#endif // GroupOptPageData_included
