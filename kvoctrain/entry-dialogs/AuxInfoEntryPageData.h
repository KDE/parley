/**********************************************************************

	--- Qt Architect generated file ---

	File: AuxInfoEntryPageData.h
	Last generated: Thu Oct 4 17:25:14 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef AuxInfoEntryPageData_included
#define AuxInfoEntryPageData_included

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
#include <qmultilinedit.h>

class AuxInfoEntryPageData : public QDialog
{
    Q_OBJECT

public:

    AuxInfoEntryPageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

//  virtual ~AuxInfoEntryPageData();

public slots:


protected slots:


protected:
    QLabel* synonym_label;
    QLabel* antonym_label;
    QLabel* examp_label;
    QLabel* remark_label;
    QLabel* para_label;
    QMultiLineEdit* synonym_line;
    QMultiLineEdit* antonym_line;
    QMultiLineEdit* examp_line;
    QMultiLineEdit* remark_line;
    QMultiLineEdit* para_line;

};

#endif // AuxInfoEntryPageData_included
