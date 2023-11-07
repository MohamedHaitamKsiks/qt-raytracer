#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createentitypopup.h"
#include "entityitem.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setup ui
    ui->setupUi(this);

    // add rendering context
    renderer = new RaytracingGLWidget(ui->centralwidget);
    renderer->show();

    ui->centralwidget->layout()->addWidget(renderer);

    // create root item for scene
    EntityItem* rootItem = new EntityItem("Root", RaytracingRenderer::instance()->getSceneRoot(), ui->sceneTreeWidget);
    rootItem->setExpanded(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addEntityButton_pressed()
{
    EntityItem* currentItem = dynamic_cast<EntityItem*>(ui->sceneTreeWidget->currentItem());

    CreateEntityPopup* popup = new CreateEntityPopup(currentItem, ui->centralwidget);
    popup->exec();
    delete popup;
}

