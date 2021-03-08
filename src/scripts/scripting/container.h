/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTINGCONTAINER_H
#define SCRIPTINGCONTAINER_H

#include <QObject>
#include <QVariant>

#include <KEduVocContainer>

namespace Scripting
{

/**
 * @class Container
 * @brief KEduVocContainer wrapping class for Kross scripts (inherited by Lesson)
 *
 * This class should not be used directly by a script but through Lesson class which inherits all the Container's properties. See Lesson class documentation.
 *
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Container : public QObject
{
    Q_OBJECT
    /// Container name
    Q_PROPERTY(QString name READ name WRITE setName)
    /// Boolean value if lesson is in Practice or not
    Q_PROPERTY(bool inPractice READ inPractice WRITE setInPractice)
    /// Container image URL
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl)
//             Q_PROPERTY ( QVariantList childContainers READ childContainers )
public:

    explicit Container(KEduVocContainer * container = 0);

    /* copy constructor for d-pointer safe copying */
//             Container ( const Container &other );

//             Container ( const QString& name, KEduVocContainer::EnumContainerType type, KEduVocContainer *parent = 0 );

    ~Container();

    /* destructor */
//             virtual ~KEduVocContainer();

    /* assignment operator */
//             KEduVocContainer& operator= ( const KEduVocContainer& );

    /* equality operator */
//             bool operator== ( const KEduVocContainer &other );


    template <class T, class S>
    QVariantList toVariantList(QList<T*> objList) const;

    static QList<KEduVocContainer*>  flattenContainer(KEduVocContainer * root);

    KEduVocContainer * kEduVocContainer() {
        return m_container;
    }

    static bool enumToBool(KEduVocContainer::EnumEntriesRecursive recursive);
    static KEduVocContainer::EnumEntriesRecursive boolToEnum(bool recursive);

    /* set the container name
     * @param name text to set for the name
     */
    void setName(const QString &name) {
        m_container->setName(name);
    }

    /* get the container name */
    QString name() {
        return m_container->name();
    }

    void appendChildContainer(Container *child) {
        m_container->appendChildContainer(child->kEduVocContainer());
    }
    void insertChildContainer(int row, Container *child) {
        m_container->insertChildContainer(row, child->kEduVocContainer());
    }
//             void deleteChildContainer ( int row ) { m_container->deleteChildContainer ( row ); }
    void removeChildContainer(int row) {
        m_container->removeChildContainer(row);
    }
    Container *childContainer(int row) {
        return new Container(m_container->childContainer(row));
    }

    /*
     * Retrieve a child container by its name
     * Returns 0 if no container is found
     * @param name container name
     * @return the child container
     */
    Container *childContainer(const QString& name) {
        return new Container(m_container->childContainer(name));
    }

    int childContainerCount() const {
        return m_container->childContainerCount();
    }

    bool inPractice() {
        return m_container->inPractice();
    }
    void setInPractice(bool inPractice) {
        m_container->setInPractice(inPractice);
    }

    /* get the image url for this container if it exists */
    QString imageUrl() {
        return m_container->imageUrl().path();
    }

    /* set the image url for this container
     * @param url               url of the image
     */
    void setImageUrl(const QString & url) {
        m_container->setImageUrl(QUrl(url));
    }

    /* finds the container with the given name */
    KEduVocContainer * findContainer(const QString& name);

    /*
     * Removes a translation. This has to be called when a language is removed from a document.
     * @param translation
     */
    void removeTranslation(int translation) {
        return m_container->removeTranslation(translation);
    }

    /*
     * Returns a list with all the child containers (lessons)
     * @return A list of Container objects
     */
    QVariantList childContainers();


public slots:

    /** Returns an integer, the Container's unique row. Useful for Lesson::removeChildLesson() function */
    int row() const {
        return m_container->row();
    }

//             virtual Container *parent() { return new Container ( m_container->parent() ); }

    /*
     * The type of this container. @see EnumContainerType
     * @return
     */
//             KEduVocContainer::EnumContainerType containerType();

    /*
     * Set the type of container.
     * For convenience by default this is taken over from the parent, so no need to set.
     * @param type the new type
     */
//             void setContainerType ( KEduVocContainer::EnumContainerType type );

    /**
     * Returns how many expressions of language with index @p translation have grade as @p grade
     * @param translation language index
     * @param grade grade
     * @param recursive whether the search is recursive
     * @return A number that represents the number of expressions of that grade
     */
    int expressionsOfGrade(int translation, unsigned int grade, bool recursive) {
        return m_container->expressionsOfGrade(translation, grade, boolToEnum(recursive));
    }

    /**
     * Remove grades from all entries of this lessons
     * @param translation which translation to remove. -1 for all.
     * @param recursive whether to include child lessons
     */
    void resetGrades(int translation, bool recursive) {
        return m_container->resetGrades(translation, boolToEnum(recursive));
    }

    /**
     * Returns the average grade of the specified @p translation considering all the lesson entries
     * @param translation language index
     * @param recursive whether the search is recursive
     * @return A double, the average lesson grade
     */
    double averageGrade(int translation, bool recursive) {
        return m_container->averageGrade(translation, boolToEnum(recursive));
    }

protected:
    KEduVocContainer * m_container;
};

//Template functions should not be separated from their definition (must be in the header file)
template <class T, class S>
QVariantList Container::toVariantList(QList<T*> objList) const
{
    QVariantList list;
    foreach(T * t, objList) {
        QObject * obj = new S(t);
        list.push_back(QVariant::fromValue(obj));
    }
    return list;
}

}


#endif
