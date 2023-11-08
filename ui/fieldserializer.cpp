#include "fieldserializer.h"

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSignalBlocker>

#include "renderer/raytracingrenderer.h"
#include "renderer/material.h"

template<>
QWidget* FieldSerializer::Serialize<int>(const QString& fieldName, int& field, QWidget* parent)
{
    auto* widget = new QWidget(parent);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    // add label
    auto* label = new QLabel(fieldName);
    layout->addWidget(label);

    // add input
    auto* inputField = new QSpinBox();
    inputField->setValue(field);
    inputField->setMaximum(INT_MAX);
    inputField->setMinimum(INT_MIN);

    layout->addWidget(inputField);

    QSpinBox::connect(inputField, &QSpinBox::valueChanged, [&field](int newValue) {
        field = newValue;
        RaytracingRenderer::instance()->resetRenderer();
    });

    return widget;
}


template<>
QWidget* FieldSerializer::Serialize<float>(const QString& fieldName, float& field, QWidget* parent)
{
    auto* widget = new QWidget(parent);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    // add label
    auto* label = new QLabel(fieldName);
    layout->addWidget(label);

    // add input
    auto* inputField = new QDoubleSpinBox();
    inputField->setValue(field);
    inputField->setMaximum(10000.0f);
    inputField->setMinimum(-10000.0f);
    inputField->setSingleStep(0.05);
    layout->addWidget(inputField);

    QDoubleSpinBox::connect(inputField, &QDoubleSpinBox::valueChanged, [&field](double newValue) {
        field = static_cast<float>(newValue);
        RaytracingRenderer::instance()->resetRenderer();
    });

    return widget;
}


template<>
QWidget* FieldSerializer::Serialize<QVector3D>(const QString& fieldName, QVector3D& field, QWidget* parent)
{
    // field container
    auto* widget = new QWidget(parent);
    auto* mainLayout = new QVBoxLayout(widget);
    mainLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    // add label
    auto* label = new QLabel(fieldName);
    mainLayout->addWidget(label);

    // vector container
    auto* vectorWidget = new QWidget();
    mainLayout->addWidget(vectorWidget);

    auto* vectorLayout = new QHBoxLayout(vectorWidget);
    vectorLayout->setSpacing(10);

    // serialize fields for x, y and z
    FieldSerializer::SerializeLayout("X", field[0], vectorLayout);
    FieldSerializer::SerializeLayout("Y", field[1], vectorLayout);
    FieldSerializer::SerializeLayout("Z", field[2], vectorLayout);

    return widget;
}

template<>
QWidget* FieldSerializer::Serialize<QVector4D>(const QString& fieldName, QVector4D& field, QWidget* parent)
{
    // field container
    auto* widget = new QWidget(parent);
    auto* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    // add label
    auto* label = new QLabel(fieldName);
    layout->addWidget(label);

    // serialize fields for rgba
    FieldSerializer::SerializeLayout("Red", field[0], layout);
    FieldSerializer::SerializeLayout("Green", field[1], layout);
    FieldSerializer::SerializeLayout("Blue", field[2], layout);
    FieldSerializer::SerializeLayout("Alpha", field[3], layout);

    return widget;
}

template<>
QWidget* FieldSerializer::Serialize<Material>(const QString& fieldName, Material& field, QWidget* parent)
{
    // field container
    auto* widget = new QWidget(parent);
    auto* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    // add label
    auto* label = new QLabel(fieldName);
    layout->addWidget(label);

    // serialize fields material params
    FieldSerializer::SerializeLayout("Is Emissive", field.emissive, layout);
    FieldSerializer::SerializeLayout("Smoothenss", field.smoothness, layout);
    FieldSerializer::SerializeLayout("Color", field.color, layout);

    return widget;
}
