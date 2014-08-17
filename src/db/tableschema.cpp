/* This file is part of the KDE project
   Copyright (C) 2003 Joseph Wenninger <jowenn@kde.org>
   Copyright (C) 2003-2007 Jarosław Staniek <staniek@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#include "tableschema.h"
#include "driver.h"
#include "connection.h"
#include "lookupfieldschema.h"

#include <assert.h>
#include <kdebug.h>

namespace KexiDB
{

//! @internal
class TableSchema::Private
{
public:
    Private()
            : anyNonPKField(0) {
    }

    ~Private() {
        clearLookupFields();
    }

    void clearLookupFields() {
        qDeleteAll(lookupFields);
        lookupFields.clear();
    }

    Field *anyNonPKField;
    QHash<const Field*, LookupFieldSchema*> lookupFields;
    QVector<LookupFieldSchema*> lookupFieldsList;
};
}
//-------------------------------------


using namespace KexiDB;

TableSchema::TableSchema(const QString& name)
        : FieldList(true)
        , SchemaData(KexiDB::TableObjectType)
        , m_query(0)
        , d( new Private )
        , m_isKexiDBSystem(false)
{
    m_name = name;
    init();
}

TableSchema::TableSchema(const SchemaData& sdata)
        : FieldList(true)
        , SchemaData(sdata)
        , m_query(0)
        , d( new Private )
        , m_isKexiDBSystem(false)
{
    init();
}

TableSchema::TableSchema()
        : FieldList(true)
        , SchemaData(KexiDB::TableObjectType)
        , m_query(0)
        , d( new Private )
        , m_isKexiDBSystem(false)
{
    init();
}

TableSchema::TableSchema(const TableSchema& ts, bool copyId)
        : FieldList(static_cast<const FieldList&>(ts))
        , SchemaData(static_cast<const SchemaData&>(ts))
        , d( new Private )
{
    init(ts, copyId);
}

TableSchema::TableSchema(const TableSchema& ts, int setId)
        : FieldList(static_cast<const FieldList&>(ts))
        , SchemaData(static_cast<const SchemaData&>(ts))
        , d( new Private )
{
    init(ts, false);
    m_id = setId;
}

// used by Connection
TableSchema::TableSchema(Connection *conn, const QString & name)
        : FieldList(true)
        , SchemaData(KexiDB::TableObjectType)
        , m_conn(conn)
        , m_query(0)
        , d( new Private )
        , m_isKexiDBSystem(false)
{
    assert(conn);
    m_name = name;
    init();
}

TableSchema::~TableSchema()
{
    if (m_conn)
        m_conn->removeMe(this);
    qDeleteAll(m_indices);
    delete m_query;
    delete d;
}

void TableSchema::init()
{
//Qt 4 m_indices.setAutoDelete( true );
    m_pkey = new IndexSchema(this);
    m_indices.append(m_pkey);
}

void TableSchema::init(const TableSchema& ts, bool copyId)
{
    m_conn = ts.m_conn;
    m_query = 0; //not cached
    m_isKexiDBSystem = false;
    m_name = ts.m_name;
//Qt 4 m_indices.setAutoDelete( true );
    m_pkey = 0; //will be copied
    if (!copyId)
        m_id = -1;

    //deep copy all members
    foreach(IndexSchema* otherIdx, ts.m_indices) {
        IndexSchema *idx = new IndexSchema(
            *otherIdx, *this /*fields from _this_ table will be assigned to the index*/);
        if (idx->isPrimaryKey()) {//assign pkey
            m_pkey = idx;
        }
        m_indices.append(idx);
    }

    Field::ListIterator tsIter(ts.fieldsIterator());
    Field::ListIterator iter(fieldsIterator());
    for (; iter != fieldsIteratorConstEnd(); ++tsIter, ++iter) {
        const LookupFieldSchema *lookup = ts.lookupFieldSchema(**tsIter);
        if (lookup) {
            d->lookupFields.insert(*iter, new LookupFieldSchema(*lookup));
        }
    }
}

void TableSchema::setPrimaryKey(IndexSchema *pkey)
{
    if (m_pkey && m_pkey != pkey) {
        if (m_pkey->fieldCount() == 0) {//this is empty key, probably default - remove it
            m_indices.removeOne(m_pkey);
            delete m_pkey;
        }
        else {
            m_pkey->setPrimaryKey(false); //there can be only one pkey..
            //thats ok, the old pkey is still on indices list, if not empty
        }
//  m_pkey=0;
    }

    if (!pkey) {//clearing - set empty pkey
        pkey = new IndexSchema(this);
    }
    m_pkey = pkey; //todo
    m_pkey->setPrimaryKey(true);
    d->anyNonPKField = 0; //for safety
}

FieldList& TableSchema::insertField(uint index, Field *field)
{
    assert(field);
    FieldList::insertField(index, field);
    if (!field || index > (uint)m_fields.count())
        return *this;
    field->setTable(this);
    field->m_order = index; //m_fields.count();
    //update order for next next fields
    uint fieldsCount = m_fields.count();
    for (uint i = index + 1; i < fieldsCount; i++)
        m_fields.at(i)->m_order = i;

    //Check for auto-generated indices:
    IndexSchema *idx = 0;
    if (field->isPrimaryKey()) {// this is auto-generated single-field unique index
        idx = new IndexSchema(this);
        idx->setAutoGenerated(true);
        idx->addField(field);
        setPrimaryKey(idx);
    }
    if (field->isUniqueKey()) {
        if (!idx) {
            idx = new IndexSchema(this);
            idx->setAutoGenerated(true);
            idx->addField(field);
        }
        idx->setUnique(true);
    }
    if (field->isIndexed()) {// this is auto-generated single-field
        if (!idx) {
            idx = new IndexSchema(this);
            idx->setAutoGenerated(true);
            idx->addField(field);
        }
    }
    if (idx)
        m_indices.append(idx);
    return *this;
}

bool TableSchema::removeField(KexiDB::Field *field)
{
    LookupFieldSchema* lookup = d->lookupFields.take(field);
    if (!FieldList::removeField(field)) {
        return false;
    }
    if (d->anyNonPKField && field == d->anyNonPKField) //d->anyNonPKField will be removed!
        d->anyNonPKField = 0;
    delete lookup;
    return true;
}

#if 0 //original  
KexiDB::FieldList& TableSchema::addField(KexiDB::Field* field)
{
    assert(field);
    FieldList::addField(field);
    field->setTable(this);
    field->m_order = m_fields.count();
    //Check for auto-generated indices:

    IndexSchema *idx = 0;
    if (field->isPrimaryKey()) {// this is auto-generated single-field unique index
        idx = new IndexSchema(this);
        idx->setAutoGenerated(true);
        idx->addField(field);
        setPrimaryKey(idx);
    }
    if (field->isUniqueKey()) {
        if (!idx) {
            idx = new IndexSchema(this);
            idx->setAutoGenerated(true);
            idx->addField(field);
        }
        idx->setUnique(true);
    }
    if (field->isIndexed()) {// this is auto-generated single-field
        if (!idx) {
            idx = new IndexSchema(this);
            idx->setAutoGenerated(true);
            idx->addField(field);
        }
    }
    if (idx)
        m_indices.append(idx);
    return *this;
}
#endif

void TableSchema::clear()
{
    m_indices.clear();
    d->clearLookupFields();
    FieldList::clear();
    SchemaData::clear();
    m_conn = 0;
}

/*
void TableSchema::addPrimaryKey(const QString& key)
{
  m_primaryKeys.append(key);
}*/

/*QStringList TableSchema::primaryKeys() const
{
  return m_primaryKeys;
}

bool TableSchema::hasPrimaryKeys() const
{
  return !m_primaryKeys.isEmpty();
}
*/

//const QString& TableSchema::name() const
//{
// return m_name;
//}

//void TableSchema::setName(const QString& name)
//{
// m_name=name;
/* ListIterator it( m_fields );
  Field *field;
  for (; (field = it.current())!=0; ++it) {

  int fcnt=m_fields.count();
  for (int i=0;i<fcnt;i++) {
    m_fields[i].setTable(name);
  }*/
//}

/*KexiDB::Field TableSchema::field(unsigned int id) const
{
  if (id<m_fields.count()) return m_fields[id];
  return KexiDB::Field();
}

unsigned int TableSchema::fieldCount() const
{
  return m_fields.count();
}*/

QString TableSchema::debugString() const
{
    return debugString(true);
}

QString TableSchema::debugString(bool includeTableName) const
{
    QString s;
    if (includeTableName)
        s = QString("TABLE ") + schemaDataDebugString() + "\n";
    s.append(FieldList::debugString());

    foreach(Field *f, m_fields) {
        LookupFieldSchema *lookupSchema = lookupFieldSchema(*f);
        if (lookupSchema)
            s.append(QLatin1String("\n") + lookupSchema->debugString(f->name()));
    }
    return s;
}

Connection* TableSchema::connection() const
{
    return (Connection*)m_conn;
}

void TableSchema::setKexiDBSystem(bool set)
{
    if (set)
        m_native = true;
    m_isKexiDBSystem = set;
}

void TableSchema::setNative(bool set)
{
    if (m_isKexiDBSystem && !set) {
        KexiDBWarn << "TableSchema::setNative(): cannot set native off"
        " when KexiDB system flag is set on!";
        return;
    }
    m_native = set;
}

QuerySchema* TableSchema::query()
{
    if (m_query)
        return m_query;
    m_query = new QuerySchema(*this);   //it's owned by me
    return m_query;
}

Field* TableSchema::anyNonPKField()
{
    if (!d->anyNonPKField) {
        Field *f = 0;
        for (QListIterator<Field*> it(m_fields); it.hasPrevious();) {
            f = it.previous();
            if (!f->isPrimaryKey() && (!m_pkey || !m_pkey->hasField(f)))
                break;
        }
        d->anyNonPKField = f;
    }
    return d->anyNonPKField;
}

bool TableSchema::setLookupFieldSchema(const QString& fieldName, LookupFieldSchema *lookupFieldSchema)
{
    Field *f = field(fieldName);
    if (!f) {
        KexiDBWarn << "TableSchema::setLookupFieldSchema(): no such field '" << fieldName
        << "' in table " << name();
        return false;
    }
    delete d->lookupFields.take(f);
    if (lookupFieldSchema) {
        d->lookupFields.insert(f, lookupFieldSchema);
    }
    d->lookupFieldsList.clear(); //this will force to rebuid the internal cache
    return true;
}

LookupFieldSchema *TableSchema::lookupFieldSchema(const Field& field) const
{
    return d->lookupFields.value(&field);
}

LookupFieldSchema *TableSchema::lookupFieldSchema(const QString& fieldName)
{
    Field *f = TableSchema::field(fieldName);
    if (!f)
        return 0;
    return lookupFieldSchema(*f);
}

QVector<LookupFieldSchema*> TableSchema::lookupFields() const
{
    if (d->lookupFields.isEmpty())
        return QVector<LookupFieldSchema*>();
    if (!d->lookupFields.isEmpty() && !d->lookupFieldsList.isEmpty())
        return d->lookupFieldsList; //already updated
    //update
    d->lookupFieldsList.clear();
    d->lookupFieldsList.resize(d->lookupFields.count());
    uint i = 0;
    foreach(Field* f, m_fields) {
        QHash<const Field*, LookupFieldSchema*>::ConstIterator itMap = d->lookupFields.constFind(f);
        if (itMap != d->lookupFields.constEnd()) {
            d->lookupFieldsList[i] = itMap.value();
            i++;
        }
    }
    return d->lookupFieldsList;
}

//--------------------------------------

InternalTableSchema::InternalTableSchema(const QString& name)
        : TableSchema(name)
{
}

InternalTableSchema::InternalTableSchema(const TableSchema& ts)
        : TableSchema(ts, false)
{
}

InternalTableSchema::~InternalTableSchema()
{
}

