#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    unitLoader = new UnitLoader();
    unitLoader->load();

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));

    connect(ui->actionExit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->actionClose_All, SIGNAL(triggered()),
            this, SLOT(closeAllTabs()));

    initializeGUI();
    addNewTab();
    loadAllDocks();
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!closeAllTabs())
        event->ignore();
    else
    {
        saveSettings();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == documentList.at(activeTab)->expressionEdit) {
        if (event->type() == QEvent::KeyPress)
        {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

             if (keyEvent->key() == Qt::Key_Return)
             {
                 if (keyEvent->modifiers() != Qt::ControlModifier)
                    documentList.at(activeTab)->lineParser->parseLine();
                 else
                    documentList.at(activeTab)->lineParser->insertNewLine(true);

                 return true;
             }
        }
        return QMainWindow::eventFilter(obj, event);
     } else {
         // standard event processing
         return QMainWindow::eventFilter(obj, event);
     }
}

void MainWindow::loadSettings()
{
    QSettings settings("settings/settings.ini", QSettings::IniFormat);

    settings.beginGroup("window");
        this->restoreState(settings.value("state", QByteArray()).toByteArray());
        this->restoreGeometry(settings.value("geometry", QByteArray()).toByteArray());
        ui->action_Slim_Mode->setChecked(settings.value("slimMode", true).toBool());
        on_action_Slim_Mode_triggered();
    settings.endGroup();

    settings.beginGroup("variableDock");
        ui->variableTable->setColumnWidth(0, settings.value("column1Width",80).toInt());
        ui->variableTable->setColumnWidth(1, settings.value("column2Width",80).toInt());
        ui->variableTable->setColumnWidth(2, settings.value("column3Width",80).toInt());
    settings.endGroup();

    settings.beginGroup("constantDock");
        ui->constantsTable->setColumnWidth(0, settings.value("column1Width",80).toInt());
        ui->constantsTable->setColumnWidth(1, settings.value("column2Width",80).toInt());
        ui->constantsTable->setColumnWidth(2, settings.value("column3Width",80).toInt());
    settings.endGroup();

    settings.beginGroup("output");
        settings.beginGroup("numbers");
            appSettings.output.numbers.decimalPrecision = settings.value("decimalPrecision", 6).toInt();
            appSettings.output.numbers.format = settings.value("format", 'g').toInt();
        settings.endGroup();
    settings.endGroup();
    settings.beginGroup("lineParser");
        settings.beginGroup("expression");
            appSettings.lineParser.expression.outputWithNumbers = settings.value("outputWithNumbers", false).toBool();
            appSettings.lineParser.expression.outputResult = settings.value("outputResult", false).toBool();
        settings.endGroup();
    settings.endGroup();

    recentDocuments = settings.value("recentDocuments", QStringList()).toStringList();
    updateRecentDocuments();
}

void MainWindow::saveSettings()
{
    QSettings settings("settings/settings.ini", QSettings::IniFormat);

    settings.beginGroup("window");
        settings.setValue("state",this->saveState());
        settings.setValue("geometry", this->saveGeometry());
        settings.setValue("slimMode", ui->action_Slim_Mode->isChecked());
    settings.endGroup();

    settings.beginGroup("variableDock");
        settings.setValue("column1Width",ui->variableTable->columnWidth(0));
        settings.setValue("column2Width",ui->variableTable->columnWidth(1));
        settings.setValue("column3Width",ui->variableTable->columnWidth(2));
    settings.endGroup();

    settings.beginGroup("constantDock");
        settings.setValue("column1Width",ui->constantsTable->columnWidth(0));
        settings.setValue("column2Width",ui->constantsTable->columnWidth(1));
        settings.setValue("column3Width",ui->constantsTable->columnWidth(2));
    settings.endGroup();

    settings.beginGroup("output");
        settings.beginGroup("numbers");
            settings.setValue("decimalPrecision", appSettings.output.numbers.decimalPrecision);
            settings.setValue("format", appSettings.output.numbers.format);
        settings.endGroup();
    settings.endGroup();
    settings.beginGroup("lineParser");
        settings.beginGroup("expression");
            settings.setValue("outputWithNumbers", appSettings.lineParser.expression.outputWithNumbers);
            settings.setValue("outputResult", appSettings.lineParser.expression.outputResult);
        settings.endGroup();
    settings.endGroup();

    settings.setValue("recentDocuments", recentDocuments);
}

void MainWindow::initializeGUI()
{
    //initialize variable Dock
    QTableWidgetItem *newItem;

    ui->variableTable->setColumnCount(3);
    ui->variableTable->setRowCount(0);
    ui->variableTable->setSortingEnabled(true);

    QFont font;
    font.setWeight(QFont::Bold);

    newItem = new QTableWidgetItem(tr("Variable"));
    newItem->setFont(font);
    ui->variableTable->setHorizontalHeaderItem(0, newItem);
    newItem = new QTableWidgetItem(tr("Value"));
    newItem->setFont(font);
    ui->variableTable->setHorizontalHeaderItem(1, newItem);
    newItem = new QTableWidgetItem(tr("Unit"));
    newItem->setFont(font);
    ui->variableTable->setHorizontalHeaderItem(2, newItem);

    connect(ui->actionVariables, SIGNAL(toggled(bool)),
            ui->variablesDock, SLOT(setVisible(bool)));
    connect(ui->variablesDock, SIGNAL(visibilityChanged(bool)),
            ui->actionVariables, SLOT(setChecked(bool)));

    //initialize constant Dock

    ui->constantsTable->setColumnCount(3);
    ui->constantsTable->setRowCount(0);
    ui->constantsTable->setSortingEnabled(true);

    newItem = new QTableWidgetItem(tr("Constant"));
    newItem->setFont(font);
    ui->constantsTable->setHorizontalHeaderItem(0, newItem);
    newItem = new QTableWidgetItem(tr("Value"));
    newItem->setFont(font);
    ui->constantsTable->setHorizontalHeaderItem(1, newItem);
    newItem = new QTableWidgetItem(tr("Unit"));
    newItem->setFont(font);
    ui->constantsTable->setHorizontalHeaderItem(2, newItem);

    connect(ui->actionConstants, SIGNAL(toggled(bool)),
            ui->constantsDock, SLOT(setVisible(bool)));
    connect(ui->constantsDock, SIGNAL(visibilityChanged(bool)),
            ui->actionConstants, SLOT(setChecked(bool)));

    //initialize special buttons
    QMenu *configureMenu = new QMenu();
    configureMenu->addMenu(ui->menuFile);
    configureMenu->addMenu(ui->menuEdit);
    configureMenu->addMenu(ui->menuCalculation);
    configureMenu->addMenu(ui->menuWindow);
    configureMenu->addMenu(ui->menuTools);
    configureMenu->addMenu(ui->menuHelp);
    ui->actionConfigure_and_control->setMenu(configureMenu);
    QToolButton *configureButton = new QToolButton();
    configureButton->setText(tr("Configure"));
    configureButton->setIcon(QIcon(":/icons/configure"));
    configureButton->setPopupMode(QToolButton::InstantPopup);
    configureButton->setMenu(configureMenu);

    //initialize Main Toolbar
    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionSave_As);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionUndo);
    ui->mainToolBar->addAction(ui->actionRedo);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionRecalculate_All);
    ui->mainToolBar->addAction(ui->actionRecalculate_from_Line);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionExport);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionClose);
    ui->mainToolBar->addSeparator();
    configureAction = ui->mainToolBar->addWidget(configureButton);
}

void MainWindow::switchLayout(int number)
{
    if (number == 0)    //normal layout
    {
        ui->mainToolBar->setIconSize(QSize(24,24));
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->menuBar->setVisible(true);
        configureAction->setVisible(false);
    }
    else                //slim layout
    {
         ui->mainToolBar->setIconSize(QSize(16,16));
         ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
         ui->menuBar->setVisible(false);
         configureAction->setVisible(true);
    }
}

void MainWindow::addNewTab()
{
    QWidget *newTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();

    activeTab = documentList.size();

    Document *newDocument = new Document;
    newDocument->expressionEdit = new QTextEdit(newTab);
    newDocument->lineParser = new LineParser(this);
    newDocument->lineParser->setUnitLoader(unitLoader);
    newDocument->lineParser->setVariableTable(ui->variableTable);
    newDocument->lineParser->setConstantsTable(ui->constantsTable);
    newDocument->lineParser->setCalculationEdit(newDocument->expressionEdit);
    newDocument->lineParser->setAppSettings(&appSettings);
    newDocument->isUnchanged = true;
    newDocument->name = "";
    newDocument->path = "";
    documentList.append(newDocument);

    newDocument->expressionEdit->installEventFilter(this);
    connect(newDocument->expressionEdit, SIGNAL(textChanged()),
            this, SLOT(documentModified()));

    layout->addWidget(newDocument->expressionEdit);
    layout->setMargin(0);
    newTab->setLayout(layout);

    ui->tabWidget->addTab(newTab,QIcon(),tr("Untitled"));
    ui->tabWidget->setCurrentIndex(activeTab);
}

bool MainWindow::closeTab(int index)
{
    Document *document = documentList.at(index);

    //if saving fails abort
    if (!saveDocument(document, false, true))
        return false;

    //never close the last tab
    if (ui->tabWidget->count() != 1)
    {
        ui->tabWidget->removeTab(index);
        delete document->lineParser;
        delete document->expressionEdit;
        documentList.removeAt(index);;
        delete document;
    }
    else
    {
        document->name = "";
        document->path = "";
        document->expressionEdit->clear();
        document->lineParser->clearAllVariables();
        document->isUnchanged = true;
        ui->tabWidget->setTabText(0, tr("Untitled"));
    }

    return true;
}

bool MainWindow::closeAllTabs()
{
    for (int i = documentList.size()-1; i > -1; i--)
    {
       if (!closeTab(i))
           return false;
    }

    return true;
}

void MainWindow::tabChanged(int index)
{
    activeTab = index;
    documentList.at(activeTab)->lineParser->showVariables();
    documentList.at(activeTab)->lineParser->showConstants();
}

bool MainWindow::saveDocument(Document *document, bool force, bool exit)
{
    //check wheter the document is unchanged or not
    if (document->isUnchanged && !force)
        return true;

    //exit = if tab is closed
    if (exit)
    {
        //ask the user to save the document
        QMessageBox messageBox(this);
        messageBox.setText(tr("The document has been modified."));
        messageBox.setInformativeText(tr("Do you want to save your changes?"));
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.button(QMessageBox::Discard)->setText(tr("Do Not Save"));
        messageBox.button(QMessageBox::Discard)->setIcon(QIcon());
        messageBox.button(QMessageBox::Cancel)->setText(tr("Do Not Close"));
        messageBox.setDefaultButton(QMessageBox::Save);
        int returnCode = messageBox.exec();

        switch (returnCode)
        {
            case QMessageBox::Save:     break;
            case QMessageBox::Discard:  return true;
            case QMessageBox::Cancel:   return false;
        }
    }

    //ask for a filename
    if (document->name.isEmpty() || force)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::homePath() + "/" + document->name, tr("Text files (*.txt)"));

        if (fileName.isEmpty())
            return false;
        else
        {
            int pos = fileName.lastIndexOf("/");
            document->path = fileName.left(pos+1);
            document->name = fileName.mid(pos+1);
            document->isUnchanged = false;
        }
    }

    //Save the file
    QString text = document->expressionEdit->toPlainText().trimmed();
    QFile file(document->path + document->name);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(text.toLocal8Bit());
        file.close();

        document->isUnchanged = true;
        ui->tabWidget->setTabText(documentList.indexOf(document), document->name);

        addRecentDocument(file.fileName());
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("An Error occured, can't save file %1").arg(file.fileName()));
    }

    return true;
}

void MainWindow::openDocument(QString fileName, bool newTab)
{
    Document *document;

    if (newTab)
        addNewTab();

    document = documentList.at(activeTab);

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QString text = QString::fromLocal8Bit(file.readAll());
        document->expressionEdit->setText(text);
        file.close();

        int pos = fileName.lastIndexOf("/");
        document->path = fileName.left(pos+1);
        document->name = fileName.mid(pos+1);
        document->isUnchanged = true;

        ui->tabWidget->setTabText(activeTab, document->name);

        addRecentDocument(fileName);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't open file %1").arg(fileName));
    }
}

void MainWindow::documentModified()
{
    Document *document = documentList.at(activeTab);
    if (document->isUnchanged)
    {
        document->isUnchanged = false;
        ui->tabWidget->setTabText(activeTab, ui->tabWidget->tabText(activeTab) + "*");
    }
}

void MainWindow::dockButtonPressed(QAbstractButton *button)
{
    documentList.at(activeTab)->expressionEdit->insertPlainText(button->toolTip().split(":").at(1).trimmed());
    documentList.at(activeTab)->expressionEdit->setFocus();
}

void MainWindow::dockWidgetPressed(QListWidgetItem *item)
{
    documentList.at(activeTab)->expressionEdit->insertPlainText(item->toolTip().split(":").at(1).trimmed());
    documentList.at(activeTab)->expressionEdit->setFocus();
}

void MainWindow::loadDock(QString name, QStringList items)
{
    //int row;
    //int column;
    //int rowSize = 4;
    int maximumWidth = 0;
    int currentWidth;

    QDockWidget *dockWidget = new QDockWidget(name, this);
    QWidget *widget = new QWidget(dockWidget);
    //QButtonGroup *buttonGroup = new QButtonGroup(this);
    //FlowLayout *flowLayout = new FlowLayout;
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::TopToBottom, widget);
    boxLayout->setMargin(0);
    QAction *menuAction = new QAction(ui->menuDocks);

    QListWidget *listWidget = new QListWidget(widget);
    listWidget->setViewMode(QListView::IconMode);
    //listWidget->setIconSize(QSize(16,16));
    //listWidget->setSpacing(5);
    listWidget->setResizeMode(QListView::Adjust);

    QFont font = listWidget->font();
    font.setPointSize(12);
    listWidget->setFont(font);

    dockWidget->setObjectName(name);
    this->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    for (int i = 0; i < items.size(); i++)
    {
        QString widgetName, toolTip, function;
        QStringList list;

        //split the item and read name and tooltip
        list = items.at(i).split(";");
        if (list.size() == 2)       //for lists with size 2 just copy the second item
            list.append(list.at(1));
        if (list.size() < 3)
            return;
        widgetName = list.at(1);
        toolTip = list.at(0);
        function = list.at(2);

        //create a new button
        /*QPushButton *button = new QPushButton(buttonName);
        button->setToolTip(toolTip + ": " + function);
        buttonGroup->addButton(button);*/

        //create a new list widget item
        QListWidgetItem *item = new QListWidgetItem(widgetName);
        item->setToolTip(toolTip + ": " + function);
        listWidget->addItem(item);

        //calculate row and column
        /*row = i / rowSize;
        column = i % rowSize;*/

        //flowLayout->addWidget(button);

        //save the maximum width
        QFontMetrics fontMetrics = listWidget->fontMetrics();
        currentWidth = fontMetrics.width(widgetName + "M");
        if (currentWidth > maximumWidth)
            maximumWidth = currentWidth;
    }

    //set the minimum width
    //foreach (QAbstractButton *button, buttonGroup->buttons())
    //    button->setMinimumWidth(maximumWidth);
    for (int i = 0; i < listWidget->count(); i++)
    {
        listWidget->item(i)->setSizeHint(QSize(maximumWidth, 20));
    }

    //put the dock togheter
    dockWidget->setWidget(widget);
    widget->setLayout(boxLayout);
    boxLayout->addWidget(listWidget);

    //add an menu entry for the dock
    menuAction->setCheckable(true);
    menuAction->setText(name);
    ui->menuDocks->addAction(menuAction);

    //connect all sorts of signals and slots
    /*connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(dockButtonPressed(QAbstractButton*)));*/
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(dockWidgetPressed(QListWidgetItem*)));
    connect(menuAction, SIGNAL(toggled(bool)),
            dockWidget, SLOT(setVisible(bool)));

    connect(dockWidget, SIGNAL(visibilityChanged(bool)),
            menuAction, SLOT(setChecked(bool)));
}

void MainWindow::loadAllDocks()
{
    //Load docks.txt
    QFile file("./settings/docks.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QStringList dockNameList = QString::fromUtf8(file.readAll()).split("\n");

        for (int i = 0; i < dockNameList.size(); i++)
        {
            QStringList list = dockNameList.at(i).split(";");

            QFile dockFile("./settings/" + list.at(1));
            if (dockFile.open(QIODevice::ReadOnly))
            {
                //Load the file and split it into string list
                loadDock(list.at(0), QString::fromUtf8(dockFile.readAll()).split("\n"));

                dockFile.close();
            }
        }

        file.close();
        return;
    }
    else
        return;
}

void MainWindow::updateRecentDocuments()
{
    //remove old actions
    for (int i = ui->menuOpen_Recent->actions().size()-1; i >= 0 ; i--)
    {
        QAction *action = ui->menuOpen_Recent->actions().at(i);
        ui->menuOpen_Recent->removeAction(action);
        action->deleteLater();
    }

    //and add new
    for (int i = recentDocuments.size()-1; i >= 0 ; i--)
    {
        QAction *recentDocumentAction = new QAction(this);
        QString fileName = recentDocuments.at(i).mid(recentDocuments.at(i).lastIndexOf('/')+1);
        recentDocumentAction->setText(tr("%1 [%2]").arg(fileName).arg(recentDocuments.at(i)));
        ui->menuOpen_Recent->addAction(recentDocumentAction);

        connect(recentDocumentAction, SIGNAL(triggered()),
                this, SLOT(openRecentDocument()));
    }
}

void MainWindow::addRecentDocument(QString name)
{
    if (!recentDocuments.contains(name))
        recentDocuments.append(name);
    else
    {
        recentDocuments.move(recentDocuments.indexOf(name), recentDocuments.size()-1);
    }

    if (recentDocuments.size() > 10)
        recentDocuments.removeFirst();

    updateRecentDocuments();
}

void MainWindow::openRecentDocument()
{
    QAction *action = (QAction*)QObject::sender();
    QString fileName = action->text().mid(action->text().indexOf('[')+1);
    fileName.chop(1);

    openDocument(fileName, true);
}

void MainWindow::on_clearVariablesButton_clicked()
{
    documentList.at(activeTab)->lineParser->clearAllVariables();
}

void MainWindow::on_actionNew_Tab_triggered()
{
    addNewTab();
}

void MainWindow::on_actionExport_triggered()
{
    ExportDialog *exportDialog = new ExportDialog(this);
    exportDialog->setText(documentList.at(activeTab)->lineParser->exportFormelEditor());
    exportDialog->show();
}

void MainWindow::on_actionSave_triggered()
{
    saveDocument(documentList.at(activeTab));
}

void MainWindow::on_actionSave_As_triggered()
{
    saveDocument(documentList.at(activeTab), true);
}

void MainWindow::on_actionSave_All_triggered()
{
    for (int i = 0; i < documentList.size(); i++)
    {
        saveDocument(documentList.at(i));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text files (*.txt)"));
    if (!fileName.isEmpty())
    {
        openDocument(fileName);
    }
}

void MainWindow::on_actionClose_triggered()
{
    closeTab(activeTab);
}

void MainWindow::on_actionClose_Other_triggered()
{
    for (int i = documentList.size()-1; i > -1; i--)
    {
        if (i != activeTab)
            closeTab(i);
    }
}

void MainWindow::on_actionUndo_triggered()
{
    documentList.at(activeTab)->expressionEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    documentList.at(activeTab)->expressionEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    documentList.at(activeTab)->expressionEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    documentList.at(activeTab)->expressionEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    documentList.at(activeTab)->expressionEdit->paste();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog settingsDialog;
    settingsDialog.setAppSettings(&appSettings);
    settingsDialog.exec();
    settingsDialog.getAppSettings(&appSettings);

    for (int i = 0; i < documentList.size(); i++)
    {
        documentList.at(i)->lineParser->setAppSettings(&appSettings);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About PhyxCalc"),
                       tr("<h2>PhyxCalc %1</h2>"
                          "Based on Qt 4.7.4 <br><br>"
                          "Copyright 2011 by Christian Schirm and Alexander R&ouml;ssler"
                          "<br><br>"
                          "PhyxCalc is free software: you can redistribute it and/or modify<br>"
                          "it under the terms of the GNU General Public License as published by<br>"
                          "the Free Software Foundation, either version 3 of the License, or<br>"
                          "(at your option) any later version.<br>"
                          "<br>"
                          "PhyxCalc is distributed in the hope that it will be useful,<br>"
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of<br>"
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>"
                          "GNU General Public License for more details.<br>"
                          "<br>"
                          "You should have received a copy of the GNU General Public License<br>"
                          "along with PhyxCalc.  If not, see <a href='http://www.gnu.org/licenses'>http://www.gnu.org/licenses</a>"
                          "<br><br>"
                          "For questions, improvements or bugs visit: <a href='https://sourceforge.net/p/phyxcalc/'>https://sourceforge.net/p/phyxcalc/</a>").arg(VERSION));
}

void MainWindow::on_actionRecalculate_All_triggered()
{
    documentList.at(activeTab)->lineParser->parseAll();
}

void MainWindow::on_actionRecalculate_from_Line_triggered()
{
    documentList.at(activeTab)->lineParser->parseFromCurrentPosition();
}

void MainWindow::on_action_Slim_Mode_triggered()
{
    switchLayout(ui->action_Slim_Mode->isChecked());
}
