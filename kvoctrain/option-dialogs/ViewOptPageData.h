/**********************************************************************

	--- Qt Architect generated file ---

	File: ViewOptPageData.h
	Last generated: Sat Oct 13 18:44:38 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef ViewOptPageData_included
#define ViewOptPageData_included

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
#include <qpushbutton.h>
#include <kcolorbtn.h>
#include <qbuttongroup.h>

class ViewOptPageData : public QDialog
{
    Q_OBJECT

public:

    ViewOptPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~ViewOptPageData();

public slots:


protected slots:


protected:
    QButtonGroup* gradegroup;
    QLabel* e_font;
    QPushButton* b_choose;
    QCheckBox* c_use;
    QLabel* l_lev7;
    KColorButton* b_col7;
    QLabel* l_lev6;
    KColorButton* b_col6;
    QLabel* l_lev5;
    KColorButton* b_col5;
    QLabel* l_lev4;
    KColorButton* b_col4;
    QLabel* l_lev3;
    KColorButton* b_col3;
    QLabel* l_lev2;
    KColorButton* b_col2;
    QLabel* l_lev1;
    KColorButton* b_col1;
    QLabel* l_lev0;
    KColorButton* b_col0;

};

#endif // ViewOptPageData_included
