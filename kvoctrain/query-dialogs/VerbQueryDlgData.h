/**********************************************************************

	--- Qt Architect generated file ---

	File: VerbQueryDlgData.h
	Last generated: Sat Oct 13 18:08:12 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef VerbQueryDlgData_included
#define VerbQueryDlgData_included

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
#include <qpushbutton.h>
#include <qlineedit.h>
#include "MyProgress.h"

class VerbQueryDlgData : public QueryDlgBase
{
    Q_OBJECT

public:

    VerbQueryDlgData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~VerbQueryDlgData();

public slots:


protected slots:


protected:
    QLabel* p1sLabel;
    QLineEdit* p1sField;
    QLabel* p1pLabel;
    QLabel* p2sLabel;
    QLineEdit* p2sField;
    QLabel* p3sfLabel;
    QLineEdit* p3sfField;
    QLabel* p3pfLabel;
    QLabel* p3smLabel;
    QLineEdit* p3smField;
    QLabel* p3pmLabel;
    QLabel* p3snLabel;
    QLineEdit* p3snField;
    QLabel* p3pnLabel;
    QLineEdit* p1pField;
    QLineEdit* p2pField;
    QLineEdit* p3pfField;
    QLineEdit* p3pmField;
    QLineEdit* p3pnField;
    QPushButton* show_all;
    QPushButton* verify;
    QPushButton* know_it;
    QPushButton* dont_know;
    QPushButton* b_edit;
    QPushButton* stop_it;
    QPushButton* options;
    QLabel* p2pLabel;
    QLabel* groupTitle;
    QLabel* countlabel;
    MyProgressBar* countbar;
    QLabel* timelabel;
    MyProgressBar* timebar;
    QLabel* progCount;
    QLabel* p1sAccel;
    QLabel* p2sAccel;
    QLabel* p3sfAccel;
    QLabel* p3smAccel;
    QLabel* p3snAccel;
    QLabel* groupVerb;

};

#endif // VerbQueryDlgData_included
