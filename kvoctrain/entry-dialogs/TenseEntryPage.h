/***************************************************************************

                   tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin          : Sat Nov 27 20:20:34 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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
#include <keduvocgrammar.h>

class TenseEntryPage : public QWidget, public Ui::TenseEntryPageForm
{
  Q_OBJECT

public:
  TenseEntryPage(bool multi_sel, const KEduVocConjugation &con_prefix, const KEduVocConjugation &conjugations,
    QWidget *parent = 0);

  void setData(bool multi_sel, const KEduVocConjugation &conjugations);

  KEduVocConjugation getConjugation();

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
  KEduVocConjugation   prefix,
                conjugations;
  QString       selection;
  bool          multi_mode;
  bool          modified;
};

#endif // TenseEntryPage_included
