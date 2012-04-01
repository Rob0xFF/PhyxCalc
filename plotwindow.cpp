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

    ui->qwtPlot->replot();
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

void PlotWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot"), QDir::homePath() + "/" + ui->settingsTitleEdit->text(), tr("PNG images (*.png)"));

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

void PlotWindow::on_datasetList_itemChanged(QListWidgetItem *item)
{
    int index = item->listWidget()->row(item);
    m_datasets->at(index)->name = item->text();
}
