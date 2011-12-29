#include "qhidingtabwidget.h"

QHidingTabWidget::QHidingTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

void QHidingTabWidget::tabInserted(int)
{
    this->tabBar()->setVisible(this->count() > 1);
}

void QHidingTabWidget::tabRemoved(int)
{
    this->tabBar()->setVisible(this->count() > 1);
}
