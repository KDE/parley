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
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
//  bool newData(const QStringList &strings);

  int currentLesson() const { return m_doc->currentLesson(); }
  void setCurrentLesson(int index) { m_doc->setCurrentLesson(index); setModified(true); } //move this into cpp?
  void setModified(bool modified) { m_doc->setModified(modified); }

  void setAllLessonsInQuery();
  void setNoLessonsInQuery();

  // add a lesson - returns lesson index
  int addLesson(const QString &lessonName = QString());
  // returns whether it was possible to delete
  bool deleteLesson(int lessonIndex, int mode);

//signals:
  //void signalLessonsInQueryChanged(QList<int> intLessons);
private:
  /** The data file */
  KEduVocDocument *m_doc;

//public slots:
  //void slotLessonSelectionChanged(const QModelIndex &, const QModelIndex &);
};

#endif
