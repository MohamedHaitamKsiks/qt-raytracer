#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "raytracingglwidget.h"
#include "inspectorwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_addEntityButton_pressed();

    void on_sceneTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_sceneTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    RaytracingGLWidget* renderer = nullptr;
    InspectorWidget* inspector = nullptr;

};
#endif // MAINWINDOW_H
