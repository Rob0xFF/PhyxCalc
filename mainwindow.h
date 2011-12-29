#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "3 pre-alpha"

#include <QMainWindow>
#include <QList>
#include <QSettings>
#include <QEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QToolButton>
#include "lineparser.h"
#include "unitloader.h"
#include "exportdialog.h"
#include "flowlayout.h"
#include "settingsdialog.h"
#include "global.h"

typedef struct {
    QString     name;
    QString     path;
    LineParser  *lineParser;
    QTextEdit   *expressionEdit;
    bool        isUnchanged;

} Document;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow  *ui;
    QAction *configureAction;           /// configure action for slim mode

    QList<Document*> documentList;
    UnitLoader      *unitLoader;
    int             activeTab;
    AppSettings     appSettings;
    QStringList     recentDocuments;

    void addNewTab();
    void loadSettings();
    void saveSettings();
    bool saveDocument(Document *document, bool force = false, bool exit = false);
    void openDocument(QString fileName, bool newTab = true);

    void loadAllDocks();
    void loadDock(QString name, QStringList items);

    void updateRecentDocuments();
    void addRecentDocument(QString name);

    void initializeGUI();

    void switchLayout(int number);          ///< switches the Layout, 0 = normal, 1 = slim

private slots:
    void tabChanged(int index);
    bool closeTab(int index);
    bool closeAllTabs();

    void documentModified();

    void dockButtonPressed(QAbstractButton *button);
    void dockWidgetPressed(QListWidgetItem *item);

    void openRecentDocument();

    void on_actionNew_Tab_triggered();
    void on_actionExport_triggered();

    void on_clearVariablesButton_clicked();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionSave_All_triggered();

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionClose_Other_triggered();

    void on_actionRecalculate_All_triggered();

    void on_actionRecalculate_from_Line_triggered();
    void on_action_Slim_Mode_triggered();
};

#endif // MAINWINDOW_H
