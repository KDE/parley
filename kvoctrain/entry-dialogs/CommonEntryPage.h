/***************************************************************************

                         edit common properties

    -----------------------------------------------------------------------

    begin          : Mon Jun 28 21:02:16 1999

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

#ifndef CommonEntryPage_included
#define CommonEntryPage_included

#include "CommonEntryPageForm.h"
#include <QueryManager.h>
#include <UsageManager.h>

class PhoneticEntryPage;
class KDialogBase;

class CommonEntryPage : public QWidget, public Ui::CommonEntryPageForm
{
  Q_OBJECT

public:
  CommonEntryPage
  (
    KEduVocDocument *doc,
    bool          multi_sel,
    QString       expr,
    int           less,
    QComboBox    *lessBox,
    QString       lang,
    QString       type,
    QString       pronunce,
    QString       usage,
    QString       label,
    QueryManager &querymanager,
    bool          active,
    const QFont&  ipafont,
    QWidget* parent = 0
  );

  void setData(
    bool          multi_sel,
    QString       expr,
    int           less,
    QComboBox    *lessBox,
    QString       lang,
    QString       type,
    QString       pronunce,
    QString       usage,
    QString       label,
    QueryManager &querymanager,
    bool          active);

  bool isDirty() const;
  bool lessonDirty     () const { return lesson_dirty; }
  bool activeDirty     () const { return active_dirty; }
  bool typeDirty       () const { return type_dirty; }
  bool usageDirty      () const { return usage_dirty; }

  int getLesson        () const { return lesson; }
  QString getType      () const { return type; }
  QString getExpr      () const { return expression; }
  QString getPronunce  () const { return pronunce; }
  QString getUsageLabel() const { return usageCollection; }
  bool    getActive    () const { return entry_active; }

  bool isModified();
  void setModified(bool mod = true);
  void setEnabled(int enable_type);

signals:
  void typeSelected(const QString &);

protected:
  void setTypeBox(const QString &act_type);
  void setLessonBox(QComboBox *lessbox, int lesson);
  void setUsageBox(const QString & act_type);

protected slots:
  void slotLessonSelected(int);
  void slotExprSelected(const QString&);
  void slotTypeSelected(int);
  void slotSubTypeSelected(int);
  void slotPronunceSelected (const QString&);
  void slotUsageChanged();
  void slotActiveChanged(bool state);
  void phoneticSelected(wchar_t);
  void invokeLessDlg();
  void invokeTypeDlg();
  void invokePronDlg();
  void invokeUsageDlg();
  void slotSubDialogClosed();

signals:
  void sigModified();

protected:
  QString       pronunce;
  QString       expression;
  QString       usageCollection;
  int           lesson;
  QString       type;
  bool          lesson_dirty;
  bool          type_dirty;
  KEduVocDocument *doc;
  QueryManager &querymanager;
  bool          entry_active;
  bool          active_dirty;
  bool          usage_dirty;

  vector<TypeRelation>  all_maintypes;
  vector<TypeRelation>  all_types;
  vector<UsageRelation> usages;
  vector<QString>       current_subtypes;

  QFont                 ipafont;
  bool                  modified;
  KDialogBase         * subDialog;
};
#endif // CommonEntryPage_included
