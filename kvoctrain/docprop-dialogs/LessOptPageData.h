/**********************************************************************

	--- Qt Architect generated file ---

	File: LessOptPageData.h
	Last generated: Thu Oct 4 17:26:33 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef LessOptPageData_included
#define LessOptPageData_included

#include <compat_2x.h> // string type macro 

#ifdef EA_KDE2x
#define EA_SPINBOX_H <qspinbox.h>
#define EA_SPINBOX   QSpinBox
#else
#define EA_SPINBOX_H <kspinbox.h>
#define EA_SPINBOX   KSpinBox
#endif
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qbuttongroup.h>

class LessOptPageData : public QDialog
{
    Q_OBJECT

public:

    LessOptPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~LessOptPageData();

public slots:


protected slots:


protected:
    QButtonGroup* lesson_group;
    QListBox* lessonList;
    QPushButton* b_new;
    QPushButton* b_modify;
    QPushButton* b_delete;
    QPushButton* b_cleanup;

};

#endif // LessOptPageData_included
