#include "ui/mainwindow.h"

#include <QApplication>
#include "entity3d/entitymanager.h"

#include "entity3d/entity3d.h"
#include "entity3d/ball3d.h"
#include "entity3d/meshinstance3d.h"
#include "entity3d/cube3d.h"

int main(int argc, char *argv[])
{
    // init entity manager
    EntityManager* entityManager = new EntityManager();
    entityManager->registerEntity<Entity3D>("Entity3D");
    entityManager->registerEntity<Ball3D>("Ball3D");
    //entityManager->registerEntity<MeshInstance3D>("MeshInstance3D");
    entityManager->registerEntity<Cube3D>("Cube3D");

    // create application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();

    // end application
    delete entityManager;
    return 0;
}
