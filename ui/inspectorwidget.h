#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "entity3d/entity3d.h"
#include <QWidget>

namespace Ui {
class InspectorWidget;
}

class InspectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InspectorWidget(QWidget *parent = nullptr);
    ~InspectorWidget();

    void inspectEntity(Entity3D* entity);

private:
    Ui::InspectorWidget *ui;
};

#endif // INSPECTORWIDGET_H
