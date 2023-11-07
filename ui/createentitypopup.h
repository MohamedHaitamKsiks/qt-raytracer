#ifndef CREATEENTITYPOPUP_H
#define CREATEENTITYPOPUP_H

#include <QDialog>
#include "entityitem.h"


namespace Ui {
class CreateEntityPopup;
}

class CreateEntityPopup : public QDialog
{
    Q_OBJECT

public:
    explicit CreateEntityPopup(EntityItem* parentEntityItem, QWidget *parent = nullptr);
    ~CreateEntityPopup();

private slots:
    void on_createButton_pressed();

private:
    Ui::CreateEntityPopup *ui;
    EntityItem* parentEntityItem = nullptr;
};

#endif // CREATEENTITYPOPUP_H
