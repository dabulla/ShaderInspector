#include "shaderparameter.h"
#include "model3d.h"

ShaderParameter::ShaderParameter(QObject *parent)
 : QObject(parent)
{
}

QString ShaderParameter::name() const
{
    return m_name;
}

QVariant ShaderParameter::value() const
{
    return m_value;
}

void ShaderParameter::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void ShaderParameter::setValue(QVariant value)
{
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(m_value);
}
