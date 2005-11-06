/***************************************************************************

                   tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin          : Sat Nov 27 20:20:34 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TenseEntryPage_included
#define TenseEntryPage_included

#include "TenseEntryPageForm.h"
#include <grammarmanager.h>

class EntryDlg;

class TenseEntryPage : public QWidget, public Ui::TenseEntryPageForm
{
  Q_OBJECT

public:
  TenseEntryPage(EntryDlg *dlgbook, bool multi_sel, const Conjugation &con_prefix,
    const Conjugation &conjugations, QWidget *parent = 0);

  void setData(bool multi_sel, const Conjugation &conjugations);

  Conjugation getConjugation();

  bool isModified();
  void setModified(bool mod = true);
  void setEnabled(int enable_type);

signals:
  void sigModified();

protected:
  void updateFields();

protected slots:
  void secondPluralChanged(const QString&);
  void secondSingularChanged(const QString&);
  void thirdNSingularChanged(const QString&);
  void thirdFPluralChanged(const QString&);
  void thirdMSingularChanged(const QString&);
  void thirdFSingularChanged(const QString&);
  void slotTenseSelected(int);
  void thirdMPluralChanged(const QString&);
  void thirdNPluralChanged(const QString&);
  void firstSingularChanged(const QString&);
  void firstPluralChanged(const QString&);
  void slotThirdSCommonToggled(bool);
  void slotThirdPCommonToggled(bool);
  void slotNextConj();

protected:
  Conjugation   prefix,
                conjugations;
  QString       selection;
  bool          multi_mode;
  bool          modified;
  EntryDlg     *dlgbook;
};

#endif // TenseEntryPage_included
