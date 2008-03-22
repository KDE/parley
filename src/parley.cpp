/***************************************************************************

                      main part of parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#include "parley.h"

#include "practice/testentrymanager.h"
#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyview.h"
#include "vocabulary/vocabularyfilter.h"
#include "vocabulary/containerview.h"
#include "vocabulary/lessonview.h"
#include "vocabulary/wordtypeview.h"
#include "vocabulary/containermodel.h"
#include "entry-dialogs/wordtypewidget.h"
#include "entry-dialogs/multiplechoicewidget.h"
#include "entry-dialogs/comparisonwidget.h"
#include "entry-dialogs/conjugationwidget.h"
#include "entry-dialogs/declensionwidget.h"
#include "entry-dialogs/imagechooserwidget.h"
#include "entry-dialogs/audiowidget.h"
#include "entry-dialogs/browserwidget.h"

#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/parleyprefs.h"
#include "language-dialogs/languagedialog.h"
#include "language-dialogs/grammardialog.h"
#include "docprop-dialogs/TitlePage.h"
#include "configure-practice/configurepracticedialog.h"
#include "prefs.h"

#include <keduvoclesson.h>
#include <keduvocexpression.h>

#include <KLineEdit>
#include <KStatusBar>

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KToggleAction>
#include <KActionMenu>
#include <KMessageBox>
#include <KTipDialog>
#include <KCharSelect>
#include <knewstuff2/ui/knewstuffaction.h>

#include <QtCore/QTimer>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>

#include "modeltest/modeltest.h"


ParleyApp::ParleyApp(const QString& appName, const KUrl & filename) : KXmlGuiWindow(0)
{
    m_appName = appName;
    m_document = new ParleyDocument(this);
    m_searchLine = 0;
    m_searchWidget = 0;
    m_pronunciationStatusBarLabel = 0;
    m_remarkStatusBarLabel = 0;
    m_typeStatusBarLabel = 0;

    m_entryDlg = 0;

    initStatusBar();

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    initView();
    initModel();

    initDockWidgets();

    initActions();

    // these connects need the model to exist
//     QAction * actionRestoreNativeOrder = actionCollection()->action("restore_native_order");
//     m_tableView->horizontalHeader()->addAction(actionRestoreNativeOrder);
//     connect(actionRestoreNativeOrder, SIGNAL(triggered()), m_sortFilterModel, SLOT(restoreNativeOrder()));

    if ( !filename.url().isEmpty() ) {
        kDebug() << "open doc" << filename.url();
        m_document->open(filename);
        kDebug() << "open done";
    } else {
        if (m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(
                                        m_recentFilesAction->actions().count()-1)->isEnabled() )
        {
            m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->trigger();
        } else {
        kDebug() << "new doc";
            // this is probably the first time we start.
            m_document->newDocument();
            updateDocument();
        }
    }


    if (!initialGeometrySet()) {
        resize(QSize(800, 600).expandedTo(minimumSizeHint()));
    }
    setupGUI(ToolBar | Keys | StatusBar | Create);

    // save position of dock windows etc
    setAutoSaveSettings();

    // finally show tip-of-day ( if the user wants it :) )
    QTimer::singleShot( 0, this, SLOT( startupTipOfDay() ) );
}


ParleyApp::~ParleyApp()
{
}


void ParleyApp::saveOptions()
{
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
    m_downloadedFilesAction->saveEntries(KGlobal::config()->group("Downloaded Files"));

///@todo save selection per document
//     if (m_tableView) {
//         // map to original entry numbers:
//         QModelIndex sourceIndex = m_sortFilterModel->mapToSource(m_tableView->currentIndex());
//         Prefs::setCurrentRow(sourceIndex.row());
//         Prefs::setCurrentCol(sourceIndex.column());
//     }
    Prefs::self()->writeConfig();
}


void ParleyApp::slotLanguageProperties()
{
    GrammarDialog ddlg(m_document->document(), this);

    if (ddlg.exec() == KDialog::Accepted) {
        m_document->document()->setModified();
/// @todo update languages shown in the table
//         m_vocabularyModel->reset();
        slotUpdateWindowCaption();
//         slotStatusMsg(IDS_DEFAULT);
    }
}

void ParleyApp::slotUpdateWindowCaption()
{
    setCaption(m_document->document()->title(), m_document->document()->isModified());
//     slotStatusMsg(IDS_DEFAULT);
}

void ParleyApp::slotShowStatistics()
{
    KVTStatisticsDialog statisticsDialog(m_document->document(), this);
    statisticsDialog.exec();
}


// void ParleyApp::slotCleanVocabulary()
// {
//     QApplication::setOverrideCursor(Qt::WaitCursor);
//     int num = m_document->document()->cleanUp();
//     QApplication::restoreOverrideCursor();
// 
//     if (num != 0) {
//         m_tableModel->reset();
//         QString s = i18np("1 entry with the same content was found and removed.",
//                           "%1 entries with the same content were found and removed.", num);
// 
//         KMessageBox::information(this, s, i18n("Remove Duplicates"));
//     }
// }


void ParleyApp::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document(), this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}


void ParleyApp::slotApplyPreferences()
{
    m_document->enableAutoBackup(Prefs::autoBackup());

    if (m_pronunciationStatusBarLabel) {
        m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    }

    m_vocabularyView->setFont(Prefs::tableFont());
    m_vocabularyView->reset();
}


// void ParleyApp::slotStatusHelpMsg(const QString &text)
// {
//     ///////////////////////////////////////////////////////////////////
//     // change status message of whole statusbar temporary (text, msec)
//     if (pbar == 0 || !pbar->isVisible()) {
//         statusBar()->showMessage(text, 3000);
//     }
// }




void ParleyApp::slotConfigShowSearch()
{
    if (m_searchWidget) {
        m_searchWidget->setVisible(m_searchWidget->isHidden());
        Prefs::setShowSearch(m_searchWidget->isVisible());
    }
}

void ParleyApp::slotEditLanguages()
{
    LanguageDialog* languageDialog = new LanguageDialog(m_document->document(), this);
    ///@todo
    // if this becomes a KConfigDialog: connect(languageDialog, SIGNAL(settingsChanged(const QString&)), m_tableModel, SLOT(loadLanguageSettings()));
    if ( languageDialog->exec() == KDialog::Accepted ) {
         m_vocabularyModel->resetLanguages();
    }
}

void ParleyApp::slotDocumentProperties()
{
    TitlePage* titleAuthorWidget = new TitlePage(m_document->document(), this);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(this);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_document->document()->url().url()));
    if ( titleAuthorDialog->exec() == KDialog::Accepted ) {
        titleAuthorWidget->commitData();
    }
    slotUpdateWindowCaption();
    delete titleAuthorDialog;
}

void ParleyApp::configurePractice()
{
    ConfigurePracticeDialog* configurePracticeDialog;
    configurePracticeDialog = new ConfigurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());

    if ( configurePracticeDialog->exec() == ConfigurePracticeDialog::StartPractice ) {
        startPractice();
    }
}

void ParleyApp::startPractice()
{
//     hide();

    TestEntryManager testManager(m_document->document(), this);
    testManager.startPractice();

//     show();
}


bool ParleyApp::queryClose()
{
    bool erg = queryExit();
    if (erg)
        m_document->document()->setModified(false);  // avoid double query on exit via system menu
    return erg;
}


bool ParleyApp::queryExit()
{
    saveOptions();
    if (!m_document->document()->isModified()) {
        return true;
    }

    bool save = Prefs::autoSave(); //save without asking

    if (!save) {
        int exit = KMessageBox::warningYesNoCancel(this, i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
                "", KStandardGuiItem::save(), KStandardGuiItem::discard());
        if (exit == KMessageBox::Yes) {
            save = true;   // save and exit
        } else if (exit == KMessageBox::No) {
            save = false;  // don't save but exit
        } else {
            return false;  // continue work
        }
    }

    if (save) {
        m_document->save();       // save and exit
    }
    return true;
}


void ParleyApp::updateDocument()
{
///@todo we can use connect here
    m_vocabularyModel->setDocument(m_document->document());

    m_lessonModel->setDocument(m_document->document());

    // expand the root items
    m_lessonView->expandToDepth(0);

    m_wordTypeModel->setDocument(m_document->document());
    m_wordTypeView->expandToDepth(0);

    connect(m_document->document(), SIGNAL(docModified(bool)), this, SLOT(slotUpdateWindowCaption()));

    setCaption(m_document->document()->url().fileName(), false);


///@todo remove this!
// at the moment creates a new test every time a model is created. this is good because we get the basic sanity check then.
    new ModelTest(m_vocabularyModel, this);
}


void ParleyApp::initDockWidgets()
{
// Lesson dock
    QDockWidget *lessonDockWidget = new QDockWidget(i18n("Lessons"), this);
    lessonDockWidget->setObjectName("LessonDock");
    m_lessonView = new LessonView(this);
    lessonDockWidget->setWidget(m_lessonView);
    addDockWidget(Qt::LeftDockWidgetArea, lessonDockWidget);
    actionCollection()->addAction("show_lesson_dock", lessonDockWidget->toggleViewAction());

    m_lessonModel = new ContainerModel(KEduVocLesson::Lesson, this);
///@todo remove before release
    new ModelTest(m_lessonModel, this);

    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
            "With the checkboxes you can select which lessons you want to practice. \n"
            "Only checked lessons [x] will be asked in the tests!"));

    connect(m_lessonView, SIGNAL(selectedLessonChanged(KEduVocLesson*)), 
        m_vocabularyModel, SLOT(setLesson(KEduVocLesson*)));

    connect(m_lessonView, SIGNAL(signalShowContainer(KEduVocContainer*)), 
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_lessonView, SLOT(setTranslation(KEduVocExpression*, int)));


// Word types dock
    QDockWidget* wordTypeDockWidget = new QDockWidget(i18n("Word Types"), this);
    wordTypeDockWidget->setObjectName( "WordTypeDock" );
    m_wordTypeView = new WordTypeView(this);
    wordTypeDockWidget->setWidget(m_wordTypeView);
    addDockWidget( Qt::LeftDockWidgetArea, wordTypeDockWidget );

    m_wordTypeModel = new ContainerModel(KEduVocContainer::WordType, this);
    wordTypeDockWidget->setVisible(false);
    actionCollection()->addAction("show_wordtype_dock", wordTypeDockWidget->toggleViewAction());

///@todo test, should be fixed with the lesson one though
///@todo remove before release
//     new ModelTest(m_wordTypeModel, this);

    m_wordTypeView->setModel(m_wordTypeModel);

    connect(m_wordTypeView, SIGNAL(selectedWordTypeChanged(KEduVocWordType*)), 
        m_vocabularyModel, SLOT(setWordType(KEduVocWordType*)));

    connect(m_wordTypeView, SIGNAL(signalShowContainer(KEduVocContainer*)), 
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_wordTypeView, SLOT(setTranslation(KEduVocExpression*, int)));

// Conjugations
    QDockWidget *conjugationDock = new QDockWidget(i18n("Conjugation"), this);
    conjugationDock->setObjectName("ConjugationDock");
    ConjugationWidget *conjugationWidget = new ConjugationWidget(this);
    conjugationDock->setWidget(conjugationWidget);
    addDockWidget(Qt::RightDockWidgetArea, conjugationDock);
    conjugationDock->setVisible(false);
    actionCollection()->addAction("show_conjugation_dock", conjugationDock->toggleViewAction());
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)),
        conjugationWidget, SLOT(setDocument(KEduVocDocument*)));
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        conjugationWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Declensions
    QDockWidget *declensionDock = new QDockWidget(i18n("Declension"), this);
    declensionDock->setObjectName("DeclensionDock");
    DeclensionWidget *declensionWidget = new DeclensionWidget(this);
    declensionDock->setWidget(declensionWidget);
    addDockWidget(Qt::RightDockWidgetArea, declensionDock);
    actionCollection()->addAction("show_declension_dock", declensionDock->toggleViewAction());
    declensionDock->setVisible(false);
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)),
            declensionWidget, SLOT(setDocument(KEduVocDocument*)));
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            declensionWidget, SLOT(setTranslation(KEduVocExpression*, int)));


// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget(i18n("Comparison forms"), this);
    comparisonDock->setObjectName("ComparisonDock");
    ComparisonWidget *comparisonWidget = new ComparisonWidget(this);
    comparisonDock->setWidget(comparisonWidget);
    addDockWidget(Qt::RightDockWidgetArea, comparisonDock);
    actionCollection()->addAction("show_comparison_dock", comparisonDock->toggleViewAction());
    comparisonDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        comparisonWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), comparisonWidget, SLOT(setDocument(KEduVocDocument*)));


// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
    multipleChoiceDock->setObjectName("MultipleChoiceDock");
    MultipleChoiceWidget *multipleChoiceWidget = new MultipleChoiceWidget(this);
    multipleChoiceDock->setWidget(multipleChoiceWidget);
    addDockWidget(Qt::RightDockWidgetArea, multipleChoiceDock);
    actionCollection()->addAction("show_multiplechoice_dock", multipleChoiceDock->toggleViewAction());
    multipleChoiceDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        multipleChoiceWidget, SLOT(setTranslation(KEduVocExpression*, int)));


// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget(i18n("Phonetic Symbols"), this);
    charSelectDock->setObjectName("IPADock");
    KCharSelect *charSelectWidget = new KCharSelect(this,  KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    charSelectWidget->setCurrentChar(0x0250);
    charSelectDock->setWidget(charSelectWidget);
    addDockWidget(Qt::BottomDockWidgetArea, charSelectDock);
    actionCollection()->addAction("show_pronunciation_dock", charSelectDock->toggleViewAction());
    charSelectDock->setVisible(false);
    connect(charSelectWidget, SIGNAL(charSelected(const QChar &)), m_vocabularyView, SLOT(appendChar(const QChar &)));

// Image
    QDockWidget *imageDock = new QDockWidget(i18n("Image"), this);
    imageDock->setObjectName("ImageDock");
    ImageChooserWidget *imageChooserWidget = new ImageChooserWidget(this);
    imageDock->setWidget(imageChooserWidget);
    addDockWidget(Qt::RightDockWidgetArea, imageDock);
    actionCollection()->addAction("show_image_dock", imageDock->toggleViewAction());
    imageDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        imageChooserWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Sound
    QDockWidget *audioDock = new QDockWidget(i18n("Sound"), this);
    audioDock->setObjectName("AudioDock");
    AudioWidget *audioWidget = new AudioWidget(this);
    audioDock->setWidget(audioWidget);
    addDockWidget(Qt::RightDockWidgetArea, audioDock);
    actionCollection()->addAction("show_audio_dock", audioDock->toggleViewAction());
    audioDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        audioWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// browser
    QDockWidget *browserDock = new QDockWidget(i18n("Internet"), this);
    browserDock->setObjectName("BrowserDock");
    //TinyWebBrowser *browserWidget = new TinyWebBrowser(this);
    BrowserWidget *htmlPart = new BrowserWidget(browserDock);
    browserDock->setWidget(htmlPart);
    addDockWidget(Qt::RightDockWidgetArea, browserDock);
    actionCollection()->addAction("show_browser_dock", browserDock->toggleViewAction());
    browserDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            htmlPart, SLOT(setTranslation(KEduVocExpression*, int)));

// Marble
//     QDockWidget *marbleDock = new QDockWidget(i18n("Marble"), this);
//     marbleDock->setObjectName("MarbleDock");
//     MarbleWidget *marbleWidget = new MarbleWidget(this);
//     marbleDock->setWidget(marbleWidget);
//     addDockWidget(Qt::RightDockWidgetArea, marbleDock);


// Grades
//     QDockWidget *gradeDock = new QDockWidget(i18n("Grade"), this);
//     gradeDock->setObjectName("gradeDock");
//     QLabel *gradeWidget = new QLabel("grade placeholder", this);
//     gradeDock->setWidget(gradeWidget);
//     addDockWidget(Qt::RightDockWidgetArea, gradeDock);
//     connect(this, SIGNAL(signalSetData(KEduVocTranslation*)), m_declensionWidget, SLOT(setTranslation(KEduVocTranslation*)));

// actionCollection()->addAction("show_leitner_dock", ->toggleViewAction());
}

void ParleyApp::tipOfDay() {
  KTipDialog::showTip(this, "parley/tips", true);
}

void ParleyApp::startupTipOfDay() {
  KTipDialog::showTip(this, "parley/tips");
}


void ParleyApp::initActions()
{
// -- FILE --------------------------------------------------
    KAction* fileNew = KStandardAction::openNew(m_document, SLOT(slotFileNew()), actionCollection());
    fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    KAction* fileOpen = KStandardAction::open(m_document, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    KAction* fileOpenExample = new KAction(this);
    actionCollection()->addAction("file_open_example", fileOpenExample);
    fileOpenExample->setIcon(KIcon("document-open"));
    fileOpenExample->setText(i18n("Open &Example..."));
    connect(fileOpenExample, SIGNAL(triggered(bool)), m_document, SLOT(openExample()));
    fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
    fileOpenExample->setToolTip(fileOpenExample->whatsThis());
    fileOpenExample->setStatusTip(fileOpenExample->whatsThis());

    KAction* fileGHNS = KNS::standardAction(i18n("Download New Vocabularies..."), m_document, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    m_recentFilesAction = KStandardAction::openRecent(m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());
    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

    m_downloadedFilesAction = new KRecentFilesAction(KIcon("get-hot-new-stuff"), "file_open_downloaded", this);
    actionCollection()->addAction("file_open_downloaded", m_downloadedFilesAction);
    m_downloadedFilesAction->setText(i18n("Open Downloaded Vocabularies"));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    connect(m_downloadedFilesAction, SIGNAL(urlSelected(const KUrl &)), m_document, SLOT(open(const KUrl&)));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    m_downloadedFilesAction->setMaxItems(30);

    /*
    KAction* fileMerge = new KAction(this);
    actionCollection()->addAction("file_merge", fileMerge);
    fileMerge->setText(i18n("&Merge..."));
    connect(fileMerge, SIGNAL(triggered(bool)), m_document, SLOT(slotFileMerge()));
    fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
    fileMerge->setToolTip(fileMerge->whatsThis());
    fileMerge->setStatusTip(fileMerge->whatsThis());
    fileMerge->setEnabled(false); ///@todo merging files is horribly broken
    */

    KAction* fileSave = KStandardAction::save(m_document, SLOT(save()), actionCollection());
    fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    KAction* fileSaveAs = KStandardAction::saveAs(m_document, SLOT(saveAs()), actionCollection());
    fileSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
    fileSaveAs->setToolTip(fileSaveAs->whatsThis());
    fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

    KAction* filePrint = KStandardAction::print(m_document, SLOT(printFile()), actionCollection());
    filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
    filePrint->setToolTip(filePrint->whatsThis());
    filePrint->setStatusTip(filePrint->whatsThis());

    KAction* fileProperties = new KAction(this);
    actionCollection()->addAction("file_properties", fileProperties);
    fileProperties->setText(i18n("&Properties..."));
    connect(fileProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
    fileProperties->setIcon(KIcon("document-properties"));
    fileProperties->setWhatsThis(i18n("Edit document properties"));
    fileProperties->setToolTip(fileProperties->whatsThis());
    fileProperties->setStatusTip(fileProperties->whatsThis());


    KAction* fileQuit = KStandardAction::quit(this, SLOT(close()), actionCollection());
    fileQuit->setWhatsThis(i18n("Quit Parley"));
    fileQuit->setToolTip(fileQuit->whatsThis());
    fileQuit->setStatusTip(fileQuit->whatsThis());


    KAction* editLanguages =new KAction(this);
    actionCollection()->addAction("edit_languages", editLanguages);
    editLanguages->setIcon(KIcon("set-language"));
    editLanguages->setText(i18n("&Languages..."));
    connect(editLanguages, SIGNAL(triggered()),  this, SLOT(slotEditLanguages()));
    ///@todo tooltip

    KAction* editGramar = new KAction(this);
    actionCollection()->addAction("edit_grammar", editGramar);
    editGramar->setText(i18n("&Grammar..."));
    editGramar->setWhatsThis(i18n("Edit language properties (types, tenses and usages)."));
    editGramar->setToolTip(editGramar->whatsThis());
    editGramar->setStatusTip(editGramar->whatsThis());
    connect(editGramar, SIGNAL(triggered(bool)), SLOT(slotLanguageProperties()));


//     KAction* editSaveSelectedArea = new KAction(this);
//      actionCollection()->addAction("edit_save_selected_area", editSaveSelectedArea);
//     editSaveSelectedArea->setIcon(KIcon("document-save-as"));
//     editSaveSelectedArea->setText(i18n("Save E&ntries in Current Test as..."));
//     connect(editSaveSelectedArea, SIGNAL(triggered(bool)), this, SLOT(slotSaveSelection()));
//     editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the current test as a new vocabulary"));
//     editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());
//     editSaveSelectedArea->setStatusTip(editSaveSelectedArea->whatsThis());
//     ///@todo enable when/if the corresponding function is rewritten
//     editSaveSelectedArea->setEnabled(false);

// -- VOCABULARY --------------------------------------------------

    KAction* vocabCleanUp = new KAction(this);
    actionCollection()->addAction("vocab_clean_up", vocabCleanUp);
    vocabCleanUp->setIcon(KIcon("edit-clear"));
    vocabCleanUp->setText(i18n("Remove &Duplicates"));
    connect(vocabCleanUp, SIGNAL(triggered(bool)), this, SLOT(slotCleanVocabulary()));
    vocabCleanUp->setWhatsThis(i18n("Remove duplicate entries from the vocabulary"));
    vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());
    vocabCleanUp->setStatusTip(vocabCleanUp->whatsThis());

// -- PRACTICE --------------------------------------------------

    KAction* configurePractice = new KAction(this);
    configurePractice->setText(i18n("Configure Practice..."));
    configurePractice->setIcon(KIcon("practice-setup"));
    configurePractice->setWhatsThis(i18n("Set up and start a test"));
    configurePractice->setToolTip(configurePractice->whatsThis());
    configurePractice->setStatusTip(configurePractice->whatsThis());
    actionCollection()->addAction("practice_configure", configurePractice);
    connect(configurePractice, SIGNAL(triggered(bool)), SLOT(configurePractice()));

    KAction* startPractice = new KAction(this);
    startPractice->setText(i18n("Start Practice..."));
    startPractice->setIcon(KIcon("practice-start"));
    startPractice->setWhatsThis(i18n("Start a test with the last settings"));
    startPractice->setToolTip(startPractice->whatsThis());
    startPractice->setStatusTip(startPractice->whatsThis());
    actionCollection()->addAction("practice_start", startPractice);
    connect(startPractice, SIGNAL(triggered(bool)), SLOT(startPractice()));

    KAction* showStatistics = new KAction(this);
    actionCollection()->addAction("show_statistics", showStatistics);
    showStatistics->setIcon(KIcon("statistics"));
    showStatistics->setText(i18n("&Statistics..."));
    connect(showStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
    showStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
    showStatistics->setToolTip(showStatistics->whatsThis());
    showStatistics->setStatusTip(showStatistics->whatsThis());


// -- SETTINGS --------------------------------------------------
    KAction* configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    m_vocabShowSearchBarAction = actionCollection()->add<KToggleAction>("config_show_search");
    m_vocabShowSearchBarAction->setText(i18n("Show Se&arch"));
    connect(m_vocabShowSearchBarAction, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowSearch()));
    m_vocabShowSearchBarAction->setWhatsThis(i18n("Toggle display of the search bar"));
    m_vocabShowSearchBarAction->setToolTip(m_vocabShowSearchBarAction->whatsThis());
    m_vocabShowSearchBarAction->setStatusTip(m_vocabShowSearchBarAction->whatsThis());
    m_vocabShowSearchBarAction->setChecked(Prefs::showSearch());

    actionCollection()->addAction("show_vocabulary_columns_menu", m_vocabularyColumnsActionMenu);
    m_vocabularyColumnsActionMenu->setText(i18n("Vocabulary Columns"));
    m_vocabularyColumnsActionMenu->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    m_vocabularyColumnsActionMenu->setToolTip(m_vocabularyColumnsActionMenu->whatsThis());
    m_vocabularyColumnsActionMenu->setStatusTip(m_vocabularyColumnsActionMenu->whatsThis());
    m_vocabularyView->horizontalHeader()->addAction(m_vocabularyColumnsActionMenu);

    actionCollection()->addAction(KStandardAction::TipofDay,  "help_tipofday", this, SLOT( tipOfDay() ));

// -- ONLY ON RIGHT CLICK - HEADER SO FAR -------------------------------------
    ///@todo what about this one...?
    KAction *actionRestoreNativeOrder = new KAction(this);
    actionCollection()->addAction("restore_native_order", actionRestoreNativeOrder);
    actionRestoreNativeOrder->setText(i18n("Restore Native Order"));
}


void ParleyApp::initStatusBar()
{
    m_typeStatusBarLabel = new QLabel(statusBar());
    m_typeStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_typeStatusBarLabel, 150);

    m_pronunciationStatusBarLabel = new QLabel(statusBar());
    m_pronunciationStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    statusBar()->addWidget(m_pronunciationStatusBarLabel, 200);

    m_remarkStatusBarLabel = new QLabel(statusBar());
    m_remarkStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_remarkStatusBarLabel, 150);
}

void ParleyApp::initModel()
{
    m_vocabularyModel = new VocabularyModel(this);

    m_vocabularyFilter = new VocabularyFilter(this);
    m_vocabularyFilter->setSourceModel(m_vocabularyModel);
    m_vocabularyView->setModel(m_vocabularyFilter);

    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));

    connect(m_searchLine, SIGNAL(textChanged(const QString&)), m_vocabularyFilter, SLOT(setSearchString(const QString&)));

}

/**
 * This initializes the main widgets and table.
 */
void ParleyApp::initView()
{
    // Parent of all
    QWidget * mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *topLayout = new QVBoxLayout(mainWidget);
    topLayout->setMargin(KDialog::marginHint());
    topLayout->setSpacing(KDialog::spacingHint());

    m_searchLine = new KLineEdit(this);
    m_searchLine->show();
    m_searchLine->setFocusPolicy(Qt::ClickFocus);
    m_searchLine->setClearButtonShown(true);
    m_searchLine->setClickMessage(i18n("Enter search terms here"));

    m_searchLine->setToolTip(i18n("Enter space-separated search terms to find words.\n\nEnter ^abc to look for words beginning with \"abc\".\nEnter abc$ to look for words ending with \"abc\".\nEnter type:verb to search for verbs."));

    QLabel *label = new QLabel(i18n("S&earch:"), this);
    label->setBuddy(m_searchLine);
    label->show();

    m_searchWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_searchWidget);
    layout->setSpacing(KDialog::spacingHint());
    layout->setMargin(0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout(centralWidget());
    rightLayout->setSpacing(KDialog::spacingHint());
    rightLayout->setMargin(0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());

    m_vocabularyView = new VocabularyView(this);
    m_vocabularyColumnsActionMenu = m_vocabularyView->columnsActionMenu();
    rightLayout->addWidget(m_vocabularyView, 1, 0);

    topLayout->addLayout(rightLayout);
}


#include "parley.moc"

