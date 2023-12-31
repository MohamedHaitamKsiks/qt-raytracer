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
    this->renderer = new RaytracingGLWidget(ui->centralwidget);
    this->renderer->show();
    ui->centralwidget->layout()->addWidget(this->renderer);

    // add insepector widget
    this->inspector = new InspectorWidget(ui->centralwidget);
    this->inspector->show();
    ui->centralwidget->layout()->addWidget(this->inspector);

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


void MainWindow::on_sceneTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    //EntityItem* entityItem = dynamic_cast<EntityItem*>(item);
    //this->inspector->inspectEntity(entityItem->getEntity());
}


void MainWindow::on_sceneTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    EntityItem* entityItem = dynamic_cast<EntityItem*>(current);
    this->inspector->inspectEntity(entityItem->getEntity());
}

