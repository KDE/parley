/***************************************************************************

    $Id$

                   base class for query dialogs

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef Query_Dlg_Base_H
#define Query_Dlg_Base_H

#include <qstring.h>
#include <qdialog.h>
#include <time.h>
#include <stdlib.h>

#include <QueryManager.h>

class kvoctrainExpr;
class kvoctrainDoc;
class kvoctrainApp;

class QLineEdit;
class QMultiLineEdit;
class QLabel;
class QRadioButton;

class QueryDlgBase : public QDialog
{
  Q_OBJECT
public:

   enum Result { Unknown, Known, Timeout, StopIt };

   QueryDlgBase ( QWidget * parent=0, const char * name=0, bool modal = true);

   bool smartCompare (const QString&, const QString&, int level) const;

   bool verifyField(QLineEdit *field, const QString &really);
   void resetField (QLineEdit *field);

   bool verifyField(QMultiLineEdit *field, const QString &really,
                    bool mixed);
   void resetField (QMultiLineEdit *field);

   void verifyButton(QRadioButton *radio, bool is_ok, QWidget *widget2 = 0);
   void resetButton (QRadioButton *radio, QWidget *widget2 = 0);

   int getRandom(int range)
     {
//        srand((unsigned int)time((time_t *)NULL));
        return (int) (range * ((1.0*rand())/RAND_MAX));
     }

public slots:
   virtual void initFocus() const;

signals:
   void sigOptions();
   void sigEditEntry(int row, int col);

protected:

   struct RB_Label {
     RB_Label (QRadioButton* _rb, QLabel *_label)
       : rb(_rb), label(_label) {}
 
      QRadioButton  *rb;
      QLabel        *label;
   };

   kvq_timeout_t  type_timeout;
   int            q_row,
                  q_ocol,
                  q_tcol;
   kvoctrainApp  *kv_app;
   kvoctrainDoc  *kv_doc;
   kvoctrainExpr *kv_exp;
   QString        translation;
   bool           showCounter;
   QTimer        *qtimer;
   int            timercount;
};


#endif // Query_Dlg_Base_H
