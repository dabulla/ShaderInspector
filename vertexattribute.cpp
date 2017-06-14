#include "vertexattribute.h"

VertexAttribute::VertexAttribute(QString name, DataType type)
    :m_buffer( QOpenGLBuffer::VertexBuffer ),
     m_name( name ),
     m_dataType( type ),
     m_divisor( 0 )

{
}

QString VertexAttribute::name() const
{
    return m_name;
}

VertexAttribute::DataType VertexAttribute::dataType() const
{
    return m_dataType;
}

qint32 VertexAttribute::divisor() const
{
    return m_divisor;
}

void VertexAttribute::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void VertexAttribute::setDataType(VertexAttribute::DataType dataType)
{
    if (m_dataType == dataType)
        return;

    m_dataType = dataType;
    emit dataTypeChanged(dataType);
}

void VertexAttribute::setDivisor(qint32 divisor)
{
    if (m_divisor == divisor)
        return;

    m_divisor = divisor;
    emit divisorChanged(divisor);
}
