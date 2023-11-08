#include "inspectorwidget.h"
#include "ui_inspectorwidget.h"

#include <QVBoxLayout>

InspectorWidget::InspectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InspectorWidget)
{
    ui->setupUi(this);


}

InspectorWidget::~InspectorWidget()
{
    delete ui;
}

void InspectorWidget::inspectEntity(Entity3D* entity)
{
    // delete old layout
    auto* layout = ui->inspectorContainerContent->layout();
    if (layout)
    {
        delete layout;
        qDeleteAll(ui->inspectorContainerContent->children());
    }
    // create new layout
    layout = new QVBoxLayout(ui->inspectorContainerContent);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setSpacing(4);
    layout->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignTop));

    // set new layout
    entity->serializeFields(layout);
}
