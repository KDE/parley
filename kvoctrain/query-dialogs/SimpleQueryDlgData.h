/**********************************************************************

	--- Qt Architect generated file ---

	File: SimpleQueryDlgData.h
	Last generated: Sat Oct 13 15:53:00 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef SimpleQueryDlgData_included
#define SimpleQueryDlgData_included

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
#include <qmultilinedit.h>
#include "MyProgress.h"
#include <qbuttongroup.h>

class SimpleQueryDlgData : public QueryDlgBase
{
    Q_OBJECT

public:

    SimpleQueryDlgData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~SimpleQueryDlgData();

public slots:


protected slots:


protected:
    QButtonGroup* groupName;
    QPushButton* show_more;
    QPushButton* show_all;
    QPushButton* verify;
    QPushButton* know_it;
    QPushButton* dont_know;
    QPushButton* stop_it;
    QPushButton* options;
    QLabel* progCount;
    QPushButton* b_edit;
    QLabel* queryLabel;
    QLabel* answerLabel;
    QLabel* countlabel;
    QLabel* timelabel;
    MyProgressBar* countbar;
    MyProgressBar* timebar;
    QLabel* queryField;
    QMultiLineEdit* answerField;

};

#endif // SimpleQueryDlgData_included
