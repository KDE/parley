//
// C++ Interface: kvttablemodel
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTTABLEMODEL_H
#define KVTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QPixmap>

#include <keduvocdocument.h>
#include <langset.h>

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/

class KVTTableModel : public QAbstractTableModel
{
Q_OBJECT
public:

  enum KVTItemDataRole {
    LessonsRole = Qt::UserRole + 1,
    LessonRole,
    StateRole,
    GradeRole
  };

  KVTTableModel(QObject *parent = 0);

  void setDocument(KEduVocDocument * doc);
  KEduVocDocument * document() {return m_doc;}

  void setLanguages(const LangSet & languages);

  bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

private:
  KEduVocDocument * m_doc;
  QPixmap m_pixInactive;
  QPixmap m_pixInQuery;
  LangSet m_languages;

  friend class KVocTrainApp;
};

#endif
