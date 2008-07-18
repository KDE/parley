/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTINGCONTAINER_H
#define SCRIPTINGCONTAINER_H

#include <QObject>
#include <KDebug>

#include <keduvoccontainer.h>

namespace Scripting
{

    /**
    Wrapper class for KEduVocContainer

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Container : public QObject
    {
            Q_OBJECT
            Q_PROPERTY ( QString name READ name WRITE setName )
            Q_PROPERTY ( bool inPractice READ inPractice WRITE setInPractice )
            Q_PROPERTY ( QString imageUrl READ imageUrl WRITE setImageUrl )
//             Q_PROPERTY ( QVariantList childContainers READ childContainers )
        public:

            Container ( KEduVocContainer * container = 0 );

            /** copy constructor for d-pointer safe copying */
//             Container ( const Container &other );

//             Container ( const QString& name, KEduVocContainer::EnumContainerType type, KEduVocContainer *parent = 0 );

            ~Container();

            template <class T, class S>
            QVariantList toVariantList ( QList<T*> objList ) const;

            KEduVocContainer * kEduVocContainer() { return m_container; }

            static bool enumToBool ( KEduVocContainer::EnumEntriesRecursive recursive );
            static KEduVocContainer::EnumEntriesRecursive boolToEnum ( bool recursive );

            /** set the container name
             * @param name text to set for the name
             */
            void setName ( const QString &name ) { m_container->setName ( name ); }

            /** get the container name */
            QString name() { return m_container->name(); }

            void appendChildContainer ( Container *child ) { m_container->appendChildContainer ( child->kEduVocContainer() ); }
            void insertChildContainer ( int row, Container *child ) { m_container->insertChildContainer ( row,child->kEduVocContainer() ); }
            void deleteChildContainer ( int row ) { m_container->deleteChildContainer ( row ); }
            void removeChildContainer ( int row ) { m_container->removeChildContainer ( row ); }
            Container *childContainer ( int row ) { return new Container ( m_container->childContainer ( row ) ); }

            /**
             * Retrieve a child container by its name
             * Returns 0 if no container is found
             * @param name container name
             * @return the child container
             */
            Container *childContainer ( const QString& name ) { return new Container ( m_container->childContainer ( name ) ); }

            int childContainerCount() const { return m_container->childContainerCount(); }

            bool inPractice() { return m_container->inPractice(); }
            void setInPractice ( bool inPractice ) { m_container->setInPractice ( inPractice ); }

            /** get the image url for this container if it exists */
            QString imageUrl() { return m_container->imageUrl().path(); }

            /** set the image url for this container
             * @param url               url of the image
             */
            void setImageUrl ( const QString & url ) { m_container->setImageUrl ( url ); }

        public slots:



            int row() const { return m_container->row(); }
            virtual Container *parent() { return new Container ( m_container->parent() ); }

            /** destructor */
//             virtual ~KEduVocContainer();

            /** assignment operator */
//             KEduVocContainer& operator= ( const KEduVocContainer& );

            /** get a list of all entries in the container */
//virtual functions (no need to be here)
//             virtual QList < KEduVocExpression* > entries ( EnumEntriesRecursive recursive = NotRecursive ) =0;
//             virtual int entryCount ( EnumEntriesRecursive recursive = NotRecursive ) =0;
//             virtual KEduVocExpression* entry ( int row, EnumEntriesRecursive recursive = NotRecursive ) =0;

            /**
             * Removes a translation. This has to be called when a language is removed from a document.
             * @param translation
             */
            void removeTranslation ( int translation ) { return m_container->removeTranslation ( translation ); }


            /** equality operator */
//             bool operator== ( const KEduVocContainer &other );

            /**
             * The type of this container. @see EnumContainerType
             * @return
             */
//             KEduVocContainer::EnumContainerType containerType();

            /**
             * Set the type of container.
             * For convenience by default this is taken over from the parent, so no need to set.
             * @param type the new type
             */
//             void setContainerType ( KEduVocContainer::EnumContainerType type );

            int expressionsOfGrade ( int translation, unsigned int grade ) { return m_container->expressionsOfGrade ( translation,grade ); }

            /**
             * Remove grades from all entries of this lessons
             * @param translation which translation to remove. -1 for all.
             * @param recursive wether to include child lessons
             */
            void resetGrades ( int translation, bool recursive ) { return m_container->resetGrades ( translation, boolToEnum ( recursive ) ); }

            double averageGrade ( int translation ) { return m_container->averageGrade ( translation ); }

            QVariantList childContainers();

        protected:
            KEduVocContainer * m_container;
    };

    //Template functions should not be separated from their definition (must be in the header file)
    template <class T, class S>
    QVariantList Container::toVariantList ( QList<T*> objList ) const
    {
        QVariantList list;
        foreach ( T * t, objList )
        {
            QObject * obj = new S ( t );
            list.push_back ( qVariantFromValue ( obj ) );
        }
        return list;
    }

}


#endif
