#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_data.h>
#include "phyxvariablemanager.h"
#include "global.h"
#include <QDebug>

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(PhyxVariableManager::PhyxDatasetList *datasets READ datasets WRITE setDatasets)
    
public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();
    
    PhyxVariableManager::PhyxDatasetList * datasets() const
    {
        return m_datasets;
    }

public slots:
    void setDatasets(PhyxVariableManager::PhyxDatasetList * arg)
    {
        m_datasets = arg;
        updateDatasetList();
    }

private slots:
    void updateSettings();

    void on_datasetList_itemSelectionChanged();

    void on_saveButton_clicked();

    void on_datasetList_itemChanged(QListWidgetItem *item);

private:
    Ui::PlotWindow *ui;
    PhyxVariableManager::PhyxDatasetList * m_datasets;

    QList<QwtPlotCurve*> plotCurves;
    QwtPlotGrid *plotGrid;
    QwtPlotZoomer *plotZoomer;
    QwtLegend *legend;

    void updateDatasetList();
    void plotDataset(int index);
    void deletePlots();
};

#endif // PLOTWINDOW_H
