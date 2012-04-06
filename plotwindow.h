#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QPrinter>
#include <QPrintDialog>
#include <QColorDialog>
#include <QImageWriter>
#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_div.h>
#include <qwt_scale_engine.h>
#if QWT_VERSION >= 0x060000
#include <qwt_plot_renderer.h>
#endif
#ifdef QT_SVG_LIB
#include <QSvgGenerator>
#endif
#include "phyxvariablemanager.h"
#include "global.h"

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(PhyxVariableManager::PhyxDatasetList *datasets READ datasets WRITE setDatasets)

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent (QShowEvent * event);
    
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
    void updatePlots();
    void updateSettings();
    void updatePixels();
    void updateMMs();

    void copyToClipboard();
    void saveDocument();
    void printPlot();

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

    void on_lineColorButton_clicked();

    void on_colorPlotBackgroundButton_clicked();

    void on_colorPlotBackgroundDeleteButton_clicked();

    void on_settingsYAutoscaleCheck_toggled(bool checked);

    void on_settingsXAutoscaleCheck_toggled(bool checked);

    void on_exportCurrentButton_clicked();

    void on_mobileSettingsButton_clicked(bool checked);

private:
    Ui::PlotWindow *ui;
    PhyxVariableManager::PhyxDatasetList * m_datasets;

    QList<QwtPlotCurve*> plotCurves;
    QwtPlotGrid *plotGrid;
    QwtPlotZoomer *plotZoomer;
    QwtLegend *legend;

    void initializeGUI();

    void updateDatasetList();
    void plotDataset(int index);
    void deletePlots();

    void setButtonColor(QPushButton *button, QColor color);

signals:
    void visibilityChanged(bool);
};

#endif // PLOTWINDOW_H
