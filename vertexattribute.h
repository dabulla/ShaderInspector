#ifndef VERTEXATTRIBUTE_H
#define VERTEXATTRIBUTE_H

#include <QObject>
#include <QOpenGLBuffer>

class VertexAttribute : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(DataType dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(qint32 divisor READ divisor WRITE setDivisor NOTIFY divisorChanged)

    enum DataType
    {
        Float,
        Vector2,
        Vector3,
        Vector4,
        Matrix4x4,
        Int
    };
    Q_ENUM(DataType);

    QOpenGLBuffer m_buffer;

    VertexAttribute(QString name, DataType type);

    QString name() const;
    DataType dataType() const;
    qint32 divisor() const;

public slots:
    void setName(QString name);
    void setDataType(DataType dataType);
    void setDivisor(qint32 divisor);

signals:
    void nameChanged(QString name);
    void dataTypeChanged(DataType dataType);
    void divisorChanged(qint32 divisor);

private:
    QString m_name;
    DataType m_dataType;
    qint32 m_divisor;
};

#endif // VERTEXATTRIBUTE_H
