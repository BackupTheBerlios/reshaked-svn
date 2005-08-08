/****************************************************************************
** Form interface generated from reading ui file 'interface/uic/main_window_base.ui'
**
** Created: Mon Aug 8 02:20:50 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;

class MainWindowBase : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowBase( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~MainWindowBase();

    QMenuBar *MenuBarEditor;
    QPopupMenu *File;
    QPopupMenu *Track;
    QPopupMenu *popupMenu_8;
    QPopupMenu *Edit;
    QPopupMenu *popupMenu_21;
    QPopupMenu *popupMenu_26;
    QPopupMenu *Playback;
    QAction* menuNew_SongAction;
    QAction* menuOpen_SongAction;
    QAction* menuSaveAction;
    QAction* menuSave_As_Action;
    QAction* menuExitAction;
    QAction* editUndoAction;
    QAction* editRedoAction;
    QAction* editCopyAction;
    QAction* editCutAction;
    QAction* editPasteAction;
    QAction* editOverwriteAction;
    QAction* editMix_PasteAction;
    QAction* editOverwriteOverwriteAction;
    QAction* editOverwriteMixAction;
    QAction* editCopy_SpecialAction;
    QAction* editCopy_SpecialCopy_BlockAction;
    QAction* editCopy_SpecialCopy_LoopAction;
    QAction* tracknew_itemAction;
    QAction* tracknew_itemPatternAction;
    QAction* tracknew_itemAudioAction;
    QAction* tracknew_itemBusAction;
    QAction* trackRemoveAction;
    QAction* tracknew_itemAction_2;
    QAction* trackAction;
    QAction* playbackPlayAction;
    QAction* playbackPlay_BlockAction;
    QAction* playbackPlay_LoopAction;
    QAction* playbackPlay_MarkAction;
    QAction* playbackStopAction;
    QAction* trackMuteAction;
    QAction* trackSoloAction;

protected:

protected slots:
    virtual void languageChange();

};

#endif // MAINWINDOWBASE_H
