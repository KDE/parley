//
// C++ Interface: kvtlessonmodel
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef KVTLESSONMODEL_H
#define KVTLESSONMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QString>
#include <QObject>
#include <QStringList>

class KEduVocDocument;

/** 
  * Model for the list of lessons. This is a list model for now. It obtains its data from the kvtml document.
  * QStringListModel is a bit simpler, but I want to handle the m_doc interface here to keep it simple from the outside.
  * @todo Also it automatically creates the "No lesson" virtual lesson.
  * @todo Upon select it should set the current lesson.
  */
class KVTLessonModel : public QAbstractListModel
{
  // Maybe this could be simpler - but for now it's close to the qt example and works...
  Q_OBJECT

public:
  KVTLessonModel(QObject *parent = 0) : QAbstractListModel(parent) { m_doc = 0; }

  void setDocument(KEduVocDocument * doc);
  KEduVocDocument * document() const {return m_doc;}

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;
  // change on string
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  // completely replace the old stuff
  bool newData(const QStringList &strings);

  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

private:
  /** The data file */
  KEduVocDocument *m_doc;

public slots:
  void slotLessonSelectionChanged(const QModelIndex &, const QModelIndex &);
};

#endif
