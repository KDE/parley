/**********************************************************************

	--- Qt Architect generated file ---

	File: ArtQueryDlgData.h
	Last generated: Sat Oct 13 15:52:48 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef ArtQueryDlgData_included
#define ArtQueryDlgData_included

#include <compat_2x.h> // string type macro 

#ifdef EA_KDE2x
#define EA_SPINBOX_H <qspinbox.h>
#define EA_SPINBOX   QSpinBox
#else
#define EA_SPINBOX_H <kspinbox.h>
#define EA_SPINBOX   KSpinBox
#endif
#include "QueryDlgBase.h"
#include <qlabel.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include "MyProgress.h"
#include <qbuttongroup.h>

class ArtQueryDlgData : public QueryDlgBase
{
    Q_OBJECT

public:

    ArtQueryDlgData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~ArtQueryDlgData();

public slots:


protected slots:


protected:
    QButtonGroup* art_group;
    QRadioButton* rb_fem;
    QRadioButton* male;
    QRadioButton* natural;
    QPushButton* b_edit;
    QPushButton* show_all;
    QPushButton* verify;
    QPushButton* know_it;
    QPushButton* dont_know;
    QPushButton* stop_it;
    QPushButton* options;
    QLabel* countlabel;
    MyProgressBar* countbar;
    QLabel* timelabel;
    MyProgressBar* timebar;
    QLabel* progCount;
    QLabel* orgField;

};

#endif // ArtQueryDlgData_included
