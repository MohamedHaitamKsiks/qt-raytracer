#ifndef FIELDSERIALIZER_H
#define FIELDSERIALIZER_H

#include <QLayout>
#include <QWidget>

class FieldSerializer
{
public:
    template<typename T>
    static QWidget* Serialize(const QString& fieldName, T& field, QWidget* parent = nullptr);

    template<typename T>
    static inline void SerializeLayout(const QString& fieldName, T& field, QLayout* layout)
    {
        layout->addWidget( FieldSerializer::Serialize<T>(fieldName, field) );
    }
};

#endif // FIELDSERIALIZER_H
