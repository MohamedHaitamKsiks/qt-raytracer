#include "createentitypopup.h"
#include "ui_createentitypopup.h"
#include "entity3d/entitymanager.h"
#include "renderer/raytracingrenderer.h"

CreateEntityPopup::CreateEntityPopup(EntityItem* parentEntityItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateEntityPopup)
{
    ui->setupUi(this);

    this->parentEntityItem = parentEntityItem;

    // add entities to list
    EntityManager* entityManager = EntityManager::instance();
    QList<QString> entityTypeNames = entityManager->getRegisteredEntities();

    for (const auto& entityTypeName: entityTypeNames)
    {
        QListWidgetItem* entityItem = new QListWidgetItem(ui->entityTypesList);
        entityItem->setText(entityTypeName);
    }
}

CreateEntityPopup::~CreateEntityPopup()
{
    delete ui;
}


void CreateEntityPopup::on_createButton_pressed()
{
    // get entity type
    QString entityType = ui->entityTypesList->currentItem()->text();

    // create entity
    Entity3D* entityParent = this->parentEntityItem->getEntity();
    Entity3D* newEntity = EntityManager::instance()->create(entityType, entityParent);

    EntityItem* entityItem = new EntityItem("Entity3D", newEntity, this->parentEntityItem);

    // reset renderer
    RaytracingRenderer::instance()->resetRenderer();

    this->close();
}

