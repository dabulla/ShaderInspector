#include "shaderparameterinfo.h"

QString ShaderParameterInfo::name() const
{
    return m_name;
}

QVariant ShaderParameterInfo::type() const
{
    return QVariant::fromValue(m_type);
}

QVariant ShaderParameterInfo::value() const
{
    return m_value;
}

int ShaderParameterInfo::uniformLocation() const
{
    return m_uniformLocation;
}

bool ShaderParameterInfo::isSubroutine() const
{
    return m_isSubroutine;
}

QStringList ShaderParameterInfo::subroutineValues() const
{
    return m_subroutineValues;
}

QVariant ShaderParameterInfo::datatype() const
{
    return QVariant::fromValue(m_datatype);
}

ShaderParameterInfo::ShaderParameterInfo(QObject *parent)
    : QObject(parent)
    , m_type(Uniform)
    , m_datatype(FLOAT)
    , m_uniformLocation(-1)
    , m_isSubroutine(false)
{

}

ShaderParameterInfo::ShaderParameterInfo(const ShaderParameterInfo &other)
    : m_name(other.m_name)
    , m_type(other.m_type)
    , m_datatype(other.m_datatype)
    , m_value(other.m_value)
    , m_uniformLocation(other.m_uniformLocation)
    , m_isSubroutine(other.m_isSubroutine)
    , m_subroutineValues(other.m_subroutineValues)
{
}

QVariant::Type ShaderParameterInfo::fromGLDatatype(ShaderParameterDatatype type)
{
    switch(type)
    {
    case     FLOAT:
        return QVariant::Double;
    case     FLOAT_VEC2:
        return QVariant::Vector2D;
    case     FLOAT_VEC3:
        return QVariant::Vector3D;
    case     FLOAT_VEC4:
        return QVariant::Vector4D;
    case     DOUBLE:
        return QVariant::Double;
    case     DOUBLE_VEC2:
        return QVariant::Vector2D;
    case     DOUBLE_VEC3:
        return QVariant::Vector3D;
    case     DOUBLE_VEC4:
        return QVariant::Vector4D;
    case     INT:
        return QVariant::Int;
    case     INT_VEC2:
        return QVariant::Vector2D;
    case     INT_VEC3:
        return QVariant::Vector3D;
    case     INT_VEC4:
        return QVariant::Vector4D;
    case     UNSIGNED_INT:
        return QVariant::Int;
    case     UNSIGNED_INT_VEC2:
        return QVariant::Vector2D;
    case     UNSIGNED_INT_VEC3:
        return QVariant::Vector3D;
    case     UNSIGNED_INT_VEC4:
        return QVariant::Vector4D;
    case     BOOL:
        return QVariant::Bool;
    case     BOOL_VEC2:
        return QVariant::Vector2D;
    case     BOOL_VEC3:
        return QVariant::Vector3D;
    case     BOOL_VEC4:
        return QVariant::Vector4D;
    case     FLOAT_MAT2:
    case     FLOAT_MAT3:
    case     FLOAT_MAT2x3:
    case     FLOAT_MAT2x4:
    case     FLOAT_MAT3x2:
    case     FLOAT_MAT3x4:
    case     FLOAT_MAT4x2:
    case     FLOAT_MAT4x3:
    case     DOUBLE_MAT2:
    case     DOUBLE_MAT3:
    case     DOUBLE_MAT2x3:
    case     DOUBLE_MAT2x4:
    case     DOUBLE_MAT3x2:
    case     DOUBLE_MAT3x4:
    case     DOUBLE_MAT4x2:
    case     DOUBLE_MAT4x3:
        return QVariant::Matrix;
    case     DOUBLE_MAT4:
    case     FLOAT_MAT4:
        return QVariant::Matrix4x4;
    case     SAMPLER_1D:
    case     SAMPLER_2D:
    case     SAMPLER_3D:
    case     SAMPLER_CUBE:
    case     SAMPLER_1D_SHADOW:
    case     SAMPLER_2D_SHADOW:
    case     SAMPLER_1D_ARRAY:
    case     SAMPLER_2D_ARRAY:
    case     SAMPLER_1D_ARRAY_SHADOW:
    case     SAMPLER_2D_ARRAY_SHADOW:
    case     SAMPLER_2D_MULTISAMPLE:
    case     SAMPLER_2D_MULTISAMPLE_ARRAY:
    case     SAMPLER_CUBE_SHADOW:
    case     SAMPLER_BUFFER:
    case     SAMPLER_2D_RECT:
    case     SAMPLER_2D_RECT_SHADOW:
    case     INT_SAMPLER_1D:
    case     INT_SAMPLER_2D:
    case     INT_SAMPLER_3D:
    case     INT_SAMPLER_CUBE:
    case     INT_SAMPLER_1D_ARRAY:
    case     INT_SAMPLER_2D_ARRAY:
    case     INT_SAMPLER_2D_MULTISAMPLE:
    case     INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case     INT_SAMPLER_BUFFER:
    case     INT_SAMPLER_2D_RECT:
    case     UNSIGNED_INT_SAMPLER_1D:
    case     UNSIGNED_INT_SAMPLER_2D:
    case     UNSIGNED_INT_SAMPLER_3D:
    case     UNSIGNED_INT_SAMPLER_CUBE:
    case     UNSIGNED_INT_SAMPLER_1D_ARRAY:
    case     UNSIGNED_INT_SAMPLER_2D_ARRAY:
    case     UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
    case     UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case     UNSIGNED_INT_SAMPLER_BUFFER:
    case     UNSIGNED_INT_SAMPLER_2D_RECT:
    case     IMAGE_1D:
    case     IMAGE_2D:
    case     IMAGE_3D:
    case     IMAGE_2D_RECT:
    case     IMAGE_CUBE:
    case     IMAGE_BUFFER:
    case     IMAGE_1D_ARRAY:
    case     IMAGE_2D_ARRAY:
    case     IMAGE_2D_MULTISAMPLE:
    case     IMAGE_2D_MULTISAMPLE_ARRAY:
    case     INT_IMAGE_1D:
    case     INT_IMAGE_2D:
//    case     INT_IMAGE_3D:
    case     INT_IMAGE_2D_RECT:
    case     INT_IMAGE_CUBE:
    case     INT_IMAGE_BUFFER:
    case     INT_IMAGE_1D_ARRAY:
    case     INT_IMAGE_2D_ARRAY:
    case     INT_IMAGE_2D_MULTISAMPLE:
    case     INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    case     UNSIGNED_INT_IMAGE_1D:
    case     UNSIGNED_INT_IMAGE_2D:
    case     UNSIGNED_INT_IMAGE_3D:
    case     UNSIGNED_INT_IMAGE_2D_RECT:
    case     UNSIGNED_INT_IMAGE_CUBE:
    case     UNSIGNED_INT_IMAGE_BUFFER:
    case     UNSIGNED_INT_IMAGE_1D_ARRAY:
    case     UNSIGNED_INT_IMAGE_2D_ARRAY:
    case     UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
    case     UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
        return QVariant::Image;
    case     UNSIGNED_INT_ATOMIC_COUNTER:
    default:
        return QVariant::UserType;
    }
}

QString ShaderParameterInfo::qmlTypename() const
{
    return QString::fromLatin1(QVariant::typeToName(fromGLDatatype(m_datatype)));
}

void ShaderParameterInfo::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void ShaderParameterInfo::setType(ShaderParameterInfo::ShaderParameterType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
    emit qmlTypenameChanged(qmlTypename());
}

void ShaderParameterInfo::setDatatype(ShaderParameterDatatype datatype)
{
    if (m_datatype == datatype)
        return;

    m_datatype = datatype;
    emit datatypeChanged(m_datatype);
}

void ShaderParameterInfo::setUniformLocation(int uniformLocation)
{
    if (m_uniformLocation == uniformLocation)
        return;

    m_uniformLocation = uniformLocation;
    emit uniformLocationChanged(m_uniformLocation);
}

void ShaderParameterInfo::setIsSubroutine(bool isSubroutine)
{
    if (m_isSubroutine == isSubroutine)
        return;

    m_isSubroutine = isSubroutine;
    emit isSubroutineChanged(m_isSubroutine);
}

void ShaderParameterInfo::setSubroutineValues(QStringList subroutineValues)
{
    if (m_subroutineValues == subroutineValues)
        return;

    m_subroutineValues = subroutineValues;
    emit subroutineValuesChanged(m_subroutineValues);
}
