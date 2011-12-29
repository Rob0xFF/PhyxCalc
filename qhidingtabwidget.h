#ifndef QHIDINGTABWIDGET_H
#define QHIDINGTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class QHidingTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit QHidingTabWidget(QWidget *parent = 0);

protected:
    void tabInserted(int index);
    void tabRemoved(int index);

signals:

public slots:

};

#endif // QHIDINGTABWIDGET_H
