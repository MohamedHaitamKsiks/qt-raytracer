#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "raytracingglwidget.h"

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

private:
    Ui::MainWindow *ui;
    RaytracingGLWidget* renderer = nullptr;

};
#endif // MAINWINDOW_H
