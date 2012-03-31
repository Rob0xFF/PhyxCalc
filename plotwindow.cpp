#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    m_datasets = NULL;

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::lightGray));
    grid->attach(ui->qwtPlot);

    QwtPlotZoomer *zoom = new QwtPlotZoomer(ui->qwtPlot->canvas());
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
            ui->datasetList->addItem(m_datasets->at(i)->name);
        }
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

    ui->qwtPlot->replot();
}
