#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    m_datasets = NULL;

    plotGrid = new QwtPlotGrid();
    plotGrid->setPen(QPen(Qt::lightGray));
    plotGrid->attach(ui->qwtPlot);

    plotZoomer = new QwtPlotZoomer(ui->qwtPlot->canvas());

    setButtonColor(ui->colorBackgroundButton,Qt::white);
    setButtonColor(ui->colorAxisTicksButton,Qt::black);
    setButtonColor(ui->colorAxisFontButton,Qt::black);
    setButtonColor(ui->colorGridButton,Qt::lightGray);

    connect(ui->saveButton, SIGNAL(clicked()),
            this, SLOT(saveImage()));
    connect(ui->saveButton_2, SIGNAL(clicked()),
            this, SLOT(saveVector()));
    connect(ui->printButton, SIGNAL(clicked()),
            this, SLOT(printPlot()));

    connect(ui->settingsLegendCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsTitleCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsXTitleCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsYTitleCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsLineThicknessSpin, SIGNAL(valueChanged(int)),
            this, SLOT(updateSettings()));
    connect(ui->settingsTitleEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateSettings()));
    connect(ui->settingsLegendPositionCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateSettings()));
    connect(ui->settingsXTitleEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateSettings()));
    connect(ui->settingsYTitleEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateSettings()));
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::updateDatasetList()
{
    ui->datasetList->clear();

    if (m_datasets != NULL)
    {
        for (int i = 0; i < m_datasets->size(); i++)
        {
            QListWidgetItem *item = new QListWidgetItem(m_datasets->at(i)->name);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            ui->datasetList->addItem(item);
        }
    }
}

void PlotWindow::updateSettings()
{
    if (ui->settingsTitleCheck->isChecked())
        ui->qwtPlot->setTitle(ui->settingsTitleEdit->text());
    else
        ui->qwtPlot->setTitle("");

    plotGrid->setVisible(ui->settingsGridCheck->isChecked());

    if (ui->settingsLegendCheck->isChecked())
    {
        ui->qwtPlot->insertLegend(NULL);
        legend = new QwtLegend();

        switch (ui->settingsLegendPositionCombo->currentIndex())
        {
        case 0: ui->qwtPlot->insertLegend(legend,QwtPlot::LeftLegend);
                break;
        case 1: ui->qwtPlot->insertLegend(legend,QwtPlot::RightLegend);
                break;
        case 2: ui->qwtPlot->insertLegend(legend,QwtPlot::BottomLegend);
                break;
        case 3: ui->qwtPlot->insertLegend(legend,QwtPlot::TopLegend);
                break;
        }
    }
    else
        ui->qwtPlot->insertLegend(NULL);

    if (ui->settingsXTitleCheck->isChecked())
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, ui->settingsXTitleEdit->text());
    else
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "");

    if (ui->settingsYTitleCheck->isChecked())
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, ui->settingsYTitleEdit->text());
    else
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "");

    //update line settings
    for (int i = 0; i < plotCurves.size(); i++)
    {
        QPen pen = plotCurves.at(i)->pen();
        pen.setWidth(ui->settingsLineThicknessSpin->value());
        plotCurves.at(i)->setPen(pen);
    }

    ui->qwtPlot->setCanvasBackground(QColor(ui->colorBackgroundButton->toolTip()));
    plotGrid->setPen(QPen(QColor(ui->colorGridButton->toolTip())));

    QPalette palette = ui->qwtPlot->axisWidget(QwtPlot::yLeft)->palette();
    palette.setColor(QPalette::WindowText, QColor(ui->colorAxisTicksButton->toolTip()));
    palette.setColor(QPalette::Text, QColor(ui->colorAxisFontButton->toolTip()));
    ui->qwtPlot->axisWidget(QwtPlot::yLeft)->setPalette(palette);
    palette = ui->qwtPlot->axisWidget(QwtPlot::xBottom)->palette();
    palette.setColor(QPalette::WindowText, QColor(ui->colorAxisTicksButton->toolTip()));
    palette.setColor(QPalette::Text, QColor(ui->colorAxisFontButton->toolTip()));
    ui->qwtPlot->axisWidget(QwtPlot::xBottom)->setPalette(palette);

    ui->qwtPlot->replot();
}

void PlotWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot"), QDir::homePath() + "/" + ui->qwtPlot->title().text(), tr("PNG Images (*.png)"));

    if (!fileName.isEmpty())
    {
        int width = ui->qwtPlot->width();
        int height = ui->qwtPlot->height();
        QPixmap pixmap(width, height);
        pixmap.fill(Qt::transparent);

        ui->qwtPlot->print(pixmap);
        pixmap.save(fileName);
    }
}

void PlotWindow::saveVector()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot"), QDir::homePath() + "/" + ui->qwtPlot->title().text(), tr("SVG Documents (*.svg)"));
#ifdef QT_SVG_LIB
    if ( !fileName.isEmpty() )
    {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(QSize(ui->qwtPlot->width(), ui->qwtPlot->height()));

        ui->qwtPlot->print(generator);
    }
#endif
}

void PlotWindow::printPlot()
{
    QPrinter printer;

    QString docName = ui->qwtPlot->title().text();
    if (!docName.isEmpty())
    {
        docName.replace (QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName (docName);
    }

    printer.setCreator("PhyxCalc");
    printer.setOrientation(QPrinter::Landscape);

    QPrintDialog dialog(&printer);
    if (dialog.exec())
    {
        QwtPlotPrintFilter filter;
        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            int options = QwtPlotPrintFilter::PrintAll;
            options &= ~QwtPlotPrintFilter::PrintBackground;
            options |= QwtPlotPrintFilter::PrintFrameWithScales;
            filter.setOptions(options);
        }
        ui->qwtPlot->print(printer, filter);
    }
}

void PlotWindow::plotDataset(int index)
{
    if (m_datasets->size() <= index)
        return;

    PhyxVariableManager::PhyxDataset *dataset = (*m_datasets)[index];

    //convert data
    QVector<double> x;
    QVector<double> y;

    for (int i = 0; i < dataset->data.at(0).size(); i++)
    {
        x.append(static_cast<PhyxFloatDataType>(dataset->data.at(0).at(i).real()));
        y.append(static_cast<PhyxFloatDataType>(dataset->data.at(1).at(i).real()));
    }

    QwtPlotCurve *plotCurve = new QwtPlotCurve(dataset->name);
    plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    plotCurve->setPen(QPen(qRgb(qrand() % 256,qrand() % 256,qrand() % 256)));
    plotCurve->setData(x,y);
    plotCurve->attach(ui->qwtPlot);
    plotCurves.append(plotCurve);
}

void PlotWindow::deletePlots()
{
    for (int i = plotCurves.size()-1; i >= 0; i--)
    {
        QwtPlotCurve *plot = plotCurves[i];
        plotCurves.removeAt(i);
        plot->detach();
        delete plot;
    }
}

void PlotWindow::setButtonColor(QPushButton *button, QColor color)
{
    button->setStyleSheet(QString("border-color: rgb(0, 0, 0);\nborder: 2px solid;\nbackground-color: rgba(%1,%2,%3,%4)")
                                                       .arg(color.red())
                                                       .arg(color.green())
                                                       .arg(color.blue())
                                                       .arg(color.alpha()));
    button->setToolTip(color.name());
}

void PlotWindow::on_datasetList_itemSelectionChanged()
{
    //get selected indexes
    QModelIndexList indexes = ui->datasetList->selectionModel()->selectedIndexes();

    QList<int> indexList;
    foreach(QModelIndex index, indexes)
    {
        indexList.append(index.row());
    }

    deletePlots();  //delete previous plots

    for (int i = 0; i < indexList.size(); i++)
    {
        plotDataset(indexList.at(i));
    }

    updateSettings();
}

void PlotWindow::on_datasetList_itemChanged(QListWidgetItem *item)
{
    int index = item->listWidget()->row(item);
    m_datasets->at(index)->name = item->text();
}

void PlotWindow::on_colorBackgroundButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorBackgroundButton->toolTip()),
                                          this,
                                          tr("Select Foreground Color"));
    if (color.isValid())
        setButtonColor(ui->colorBackgroundButton, color);

    updateSettings();
}

void PlotWindow::on_colorBackgroundDeleteButton_clicked()
{
    setButtonColor(ui->colorBackgroundButton, QColor(255,255,255));
    updateSettings();
}

void PlotWindow::on_colorGridButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorGridButton->toolTip()),
                                          this,
                                          tr("Select Foreground Color"));
    if (color.isValid())
        setButtonColor(ui->colorGridButton, color);

    updateSettings();
}

void PlotWindow::on_colorGridDeleteButton_clicked()
{
    setButtonColor(ui->colorGridButton, Qt::lightGray);
    updateSettings();
}

void PlotWindow::on_colorAxisTicksButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorAxisTicksButton->toolTip()),
                                          this,
                                          tr("Select Foreground Color"));
    if (color.isValid())
        setButtonColor(ui->colorAxisTicksButton, color);

    updateSettings();
}

void PlotWindow::on_colorAxisTicksDeleteButton_clicked()
{
    setButtonColor(ui->colorAxisTicksButton, QColor(0,0,0));
    updateSettings();
}

void PlotWindow::on_colorAxisFontButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorAxisFontButton->toolTip()),
                                          this,
                                          tr("Select Foreground Color"));
    if (color.isValid())
        setButtonColor(ui->colorAxisFontButton, color);

    updateSettings();
}

void PlotWindow::on_colorAxisFontDeleteButton_clicked()
{
    setButtonColor(ui->colorAxisFontButton, QColor(0,0,0));
    updateSettings();
}
