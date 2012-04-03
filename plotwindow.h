#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QPrinter>
#include <QPrintDialog>
#include <QColorDialog>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_data.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_div.h>
#include "phyxvariablemanager.h"
#include "global.h"
#include <QDebug>
#ifdef QT_SVG_LIB
#include <QSvgGenerator>
#endif

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

    void saveImage();
    void saveVector();
    void printPlot();

    void on_datasetList_itemSelectionChanged();
    void on_datasetList_itemChanged(QListWidgetItem *item);

    void on_colorBackgroundButton_clicked();

    void on_colorBackgroundDeleteButton_clicked();

    void on_colorGridButton_clicked();

    void on_colorGridDeleteButton_clicked();

    void on_colorAxisTicksButton_clicked();

    void on_colorAxisTicksDeleteButton_clicked();

    void on_colorAxisFontButton_clicked();

    void on_colorAxisFontDeleteButton_clicked();

    void on_colorGridMinButton_clicked();

    void on_colorGridMinDeleteButton_clicked();

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

    void setButtonColor(QPushButton *button, QColor color);
};

#endif // PLOTWINDOW_H
