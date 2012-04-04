#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    m_datasets = NULL;

    ui->splitter->setStretchFactor(0,2);
    ui->splitter->setStretchFactor(1,1);

    ui->qwtPlot->canvas()->setFrameStyle(QFrame::NoFrame);
    //ui->qwtPlot->setMargin(5);
    ui->qwtPlot->setStyleSheet("QwtPlot { padding: 5px }"); //this is no fix at all

    plotGrid = new QwtPlotGrid();
    plotGrid->setPen(QPen(Qt::lightGray));
    plotGrid->attach(ui->qwtPlot);

    plotZoomer = new QwtPlotZoomer(ui->qwtPlot->canvas());

    setButtonColor(ui->colorBackgroundButton,Qt::white);
    setButtonColor(ui->colorPlotBackgroundButton, Qt::white);
    setButtonColor(ui->colorAxisTicksButton,Qt::black);
    setButtonColor(ui->colorAxisFontButton,Qt::black);
    setButtonColor(ui->colorGridButton,QColor("#c3c3c3"));
    setButtonColor(ui->colorGridMinButton, QColor("#dcdcdc"));

    updatePixels();

    updateSettings();

    connect(ui->saveButton, SIGNAL(clicked()),
            this, SLOT(saveDocument()));
    connect(ui->printButton, SIGNAL(clicked()),
            this, SLOT(printPlot()));
    connect(ui->clipboardButton, SIGNAL(clicked()),
            this, SLOT(copyToClipboard()));

    connect(ui->settingsLegendGroup, SIGNAL(clicked(bool)),
            this, SLOT(updateSettings()));
    connect(ui->settingsTitleGroup, SIGNAL(clicked(bool)),
            this, SLOT(updateSettings()));
    connect(ui->settingsXTitleGroup, SIGNAL(clicked(bool)),
            this, SLOT(updateSettings()));
    connect(ui->settingsYTitleGroup, SIGNAL(clicked(bool)),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridGroup, SIGNAL(clicked(bool)),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridXCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridYCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridXMinCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsGridYMinCheck, SIGNAL(clicked()),
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
    connect(ui->settingsXAutoscaleCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsYAutoscaleCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsXScaleMinSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsXScaleMaxSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsXScaleStepSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsYScaleMinSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsYScaleMaxSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsYScaleStepSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateSettings()));
    connect(ui->settingsXLogCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsYLogCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsXInvCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));
    connect(ui->settingsYInvCheck, SIGNAL(clicked()),
            this, SLOT(updateSettings()));

    connect(ui->exportWidthSpin, SIGNAL(editingFinished()),
            this, SLOT(updateMMs()));
    connect(ui->exportHeightSpin, SIGNAL(editingFinished()),
            this, SLOT(updateMMs()));
    connect(ui->exportDpiXSpin, SIGNAL(editingFinished()),
            this, SLOT(updateMMs()));
    connect(ui->exportDpiYSpin, SIGNAL(editingFinished()),
            this, SLOT(updateMMs()));
    connect(ui->exportWidthMMSpin, SIGNAL(editingFinished()),
            this, SLOT(updatePixels()));
    connect(ui->exportHeightMMSpin, SIGNAL(editingFinished()),
            this, SLOT(updatePixels()));
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit visibilityChanged(false);
}

void PlotWindow::showEvent(QShowEvent *event)
{
    event->accept();
    emit visibilityChanged(true);
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
    if (ui->settingsTitleGroup->isChecked())
        ui->qwtPlot->setTitle(ui->settingsTitleEdit->text());
    else
        ui->qwtPlot->setTitle("");


    plotGrid->setVisible(ui->settingsGridGroup->isChecked());
    plotGrid->enableX(ui->settingsGridXCheck->isChecked());
    plotGrid->enableXMin(ui->settingsGridXMinCheck->isChecked());
    plotGrid->enableY(ui->settingsGridYCheck->isChecked());
    plotGrid->enableYMin(ui->settingsGridYMinCheck->isChecked());

    if (ui->settingsLegendGroup->isChecked())
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

    if (ui->settingsXTitleGroup->isChecked())
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, ui->settingsXTitleEdit->text());
    else
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "");

    if (ui->settingsYTitleGroup->isChecked())
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, ui->settingsYTitleEdit->text());
    else
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "");

    //axis things
    if (ui->settingsXLogCheck->isChecked())
        ui->qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLog10ScaleEngine);
    else
        ui->qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);

    if (ui->settingsYLogCheck->isChecked())
        ui->qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
    else
        ui->qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);

    ui->qwtPlot->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Inverted,ui->settingsXInvCheck->isChecked());
    ui->qwtPlot->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Inverted,ui->settingsYInvCheck->isChecked());

    if (ui->settingsXAutoscaleCheck->isChecked())
    {
        ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
        ui->qwtPlot->updateAxes();
        ui->settingsXScaleMinSpin->setValue(ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom)->lowerBound());
        ui->settingsXScaleMaxSpin->setValue(ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom)->upperBound());
        ui->settingsXScaleStepSpin->setValue(ui->qwtPlot->axisStepSize(QwtPlot::xBottom));
    }
    else
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,
                                  ui->settingsXScaleMinSpin->value(),
                                  ui->settingsXScaleMaxSpin->value(),
                                  ui->settingsXScaleStepSpin->value());

    if (ui->settingsYAutoscaleCheck->isChecked())
    {
        ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
        ui->qwtPlot->updateAxes();
        ui->settingsYScaleMinSpin->setValue(ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft)->lowerBound());
        ui->settingsYScaleMaxSpin->setValue(ui->qwtPlot->axisScaleDiv(QwtPlot::yLeft)->upperBound());
        ui->settingsYScaleStepSpin->setValue(ui->qwtPlot->axisStepSize(QwtPlot::yLeft));
    }
    else
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,
                                  ui->settingsYScaleMinSpin->value(),
                                  ui->settingsYScaleMaxSpin->value(),
                                  ui->settingsYScaleStepSpin->value());

    //update line settings
    for (int i = 0; i < plotCurves.size(); i++)
    {
        QPen pen = plotCurves.at(i)->pen();
        pen.setWidth(ui->settingsLineThicknessSpin->value());
        plotCurves.at(i)->setPen(pen);
    }

    ui->qwtPlot->setCanvasBackground(QColor(ui->colorBackgroundButton->toolTip()));
    QPalette p = ui->qwtPlot->palette();
    p.setColor(QPalette::Window, QColor(ui->colorPlotBackgroundButton->toolTip()));
    ui->qwtPlot->setPalette(p);

    plotGrid->setMajPen(QPen(QColor(ui->colorGridButton->toolTip())));
    plotGrid->setMinPen(QPen(QColor(ui->colorGridMinButton->toolTip())));

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

void PlotWindow::updatePixels()
{
    ui->exportHeightSpin->setValue(ui->exportHeightMMSpin->value() / 25.4 * ui->exportDpiXSpin->value());
    ui->exportWidthSpin->setValue(ui->exportWidthMMSpin->value() / 25.4 * ui->exportDpiXSpin->value());
}

void PlotWindow::updateMMs()
{
    ui->exportWidthMMSpin->setValue(ui->exportWidthSpin->value() / ui->exportDpiXSpin->value() * 25.4);
    ui->exportHeightMMSpin->setValue(ui->exportHeightSpin->value() / ui->exportDpiYSpin->value() * 25.4);
}

void PlotWindow::copyToClipboard()
{
    QPixmap pixmap(ui->exportWidthSpin->value(), ui->exportHeightSpin->value());
    QPalette p = ui->qwtPlot->palette();
    pixmap.fill(p.color(QPalette::Window));

#if QWT_VERSION >= 0x060000
        QwtPlotRenderer renderer;

        // flags to make the document look like the widget
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames, true);

        renderer.renderTo(ui->qwtPlot, pixmap);
#else
    ui->qwtPlot->print(pixmap);
#endif

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

void PlotWindow::saveDocument()
{
    const QList<QByteArray> imageFormats =
        QImageWriter::supportedImageFormats();

    QStringList filter;
#ifndef QWT_NO_SVG
    filter += "SVG Documents (*.svg)";
#endif
#if QWT_VERSION >= 0x060000
    filter += "PDF Documents (*.pdf)";
    filter += "Postscript Documents (*.ps)";
#endif

    if ( imageFormats.size() > 0 )
    {
        QString imageFilter("Images (");
        for ( int i = 0; i < imageFormats.size(); i++ )
        {
            if ( i > 0 )
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    QString docName = ui->qwtPlot->title().text();
    if (!docName.isEmpty())
        docName.replace (QRegExp(QString::fromLatin1("\n")), tr(" -- "));
    else
        docName = "plot";

    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Plot"), QDir::homePath() + "/" + docName + ".svg",
        filter.join(";;"));

    if ( !fileName.isEmpty() )
    {
#if QWT_VERSION >= 0x060000
        QwtPlotRenderer renderer;

        // flags to make the document look like the widget
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames, true);

        renderer.renderDocument(ui->qwtPlot, fileName, QSizeF(ui->exportWidthMMSpin->value(), ui->exportHeightMMSpin->value()), ui->exportDpiXSpin->value());
#else
        QString extension = fileName.mid(fileName.lastIndexOf("."));

        if (extension == ".svg")
        {
            QSvgGenerator generator;
            generator.setFileName(fileName);
            generator.setSize(QSize(ui->exportWidthSpin->value(), ui->exportHeightSpin->value()));
            ui->qwtPlot->print(generator);
        }
        else
        {
            QPixmap pixmap(ui->exportWidthSpin->value(), ui->exportHeightSpin->value());
            QPalette p = ui->qwtPlot->palette();
            pixmap.fill(p.color(QPalette::Window));
            ui->qwtPlot->print(pixmap);
            pixmap.save(fileName);
        }
#endif
    }
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
#if QWT_VERSION >= 0x060000
        QwtPlotRenderer renderer;

        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
            renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales);
        }

        renderer.renderTo(ui->qwtPlot, printer);
#else
        QwtPlotPrintFilter filter;
        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            int options = QwtPlotPrintFilter::PrintAll;
            options &= ~QwtPlotPrintFilter::PrintBackground;
            options |= QwtPlotPrintFilter::PrintFrameWithScales;
            filter.setOptions(options);
        }
        ui->qwtPlot->print(printer, filter);
#endif
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

#if QWT_VERSION >= 0x060000
    plotCurve->setSamples(x,y);
#else
    plotCurve->setData(x,y);
#endif
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
                                          tr("Select Canvas Background Color"));
    if (color.isValid())
        setButtonColor(ui->colorBackgroundButton, color);

    updateSettings();
}

void PlotWindow::on_colorBackgroundDeleteButton_clicked()
{
    setButtonColor(ui->colorBackgroundButton, QColor(255,255,255));
    updateSettings();
}

void PlotWindow::on_colorPlotBackgroundButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorPlotBackgroundButton->toolTip()),
                                          this,
                                          tr("Select Plot Background Color"));
    if (color.isValid())
        setButtonColor(ui->colorPlotBackgroundButton, color);

    updateSettings();
}

void PlotWindow::on_colorPlotBackgroundDeleteButton_clicked()
{
    setButtonColor(ui->colorPlotBackgroundButton, QColor(255,255,255));
    updateSettings();
}

void PlotWindow::on_colorGridButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorGridButton->toolTip()),
                                          this,
                                          tr("Select Grid Color"));
    if (color.isValid())
        setButtonColor(ui->colorGridButton, color);

    updateSettings();
}

void PlotWindow::on_colorGridDeleteButton_clicked()
{
    setButtonColor(ui->colorGridButton, QColor("#c3c3c3"));
    updateSettings();
}

void PlotWindow::on_colorGridMinButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorGridMinButton->toolTip()),
                                          this,
                                          tr("Select Grid Color"));
    if (color.isValid())
        setButtonColor(ui->colorGridMinButton, color);

    updateSettings();
}

void PlotWindow::on_colorGridMinDeleteButton_clicked()
{
    setButtonColor(ui->colorGridMinButton, QColor("#dcdcdc"));
    updateSettings();
}

void PlotWindow::on_colorAxisTicksButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(ui->colorAxisTicksButton->toolTip()),
                                          this,
                                          tr("Select Axis Color"));
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
                                          tr("Select Axis Font Color"));
    if (color.isValid())
        setButtonColor(ui->colorAxisFontButton, color);

    updateSettings();
}

void PlotWindow::on_colorAxisFontDeleteButton_clicked()
{
    setButtonColor(ui->colorAxisFontButton, QColor(0,0,0));
    updateSettings();
}

void PlotWindow::on_lineColorButton_clicked()
{
    for (int i = 0; i < plotCurves.size(); i++)
    {
        QPen pen = plotCurves.at(i)->pen();
        pen.setColor(qRgb(qrand() % 256,qrand() % 256,qrand() % 256));
        plotCurves.at(i)->setPen(pen);
    }
    ui->qwtPlot->replot();
}

void PlotWindow::on_settingsYAutoscaleCheck_toggled(bool checked)
{
    ui->settingsYScaleMaxSpin->setEnabled(!checked);
    ui->settingsYScaleMinSpin->setEnabled(!checked);
    ui->settingsYScaleStepSpin->setEnabled(!checked);
    ui->yScaleLabel1->setEnabled(!checked);
    ui->yScaleLabel2->setEnabled(!checked);
    ui->yScaleLabel3->setEnabled(!checked);
}

void PlotWindow::on_settingsXAutoscaleCheck_toggled(bool checked)
{
    ui->settingsXScaleMaxSpin->setEnabled(!checked);
    ui->settingsXScaleMinSpin->setEnabled(!checked);
    ui->settingsXScaleStepSpin->setEnabled(!checked);
    ui->xScaleLabel1->setEnabled(!checked);
    ui->xScaleLabel2->setEnabled(!checked);
    ui->xScaleLabel3->setEnabled(!checked);
}

void PlotWindow::on_exportCurrentButton_clicked()
{
    ui->exportWidthSpin->setValue(ui->qwtPlot->width());
    ui->exportHeightSpin->setValue(ui->qwtPlot->height());
    ui->exportDpiXSpin->setValue(QApplication::desktop()->physicalDpiX());
    ui->exportDpiYSpin->setValue(QApplication::desktop()->physicalDpiY());
    updateMMs();
}
