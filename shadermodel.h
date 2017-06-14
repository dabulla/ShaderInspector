#ifndef SHADERMODEL_H
#define SHADERMODEL_H

#include <QStandardItemModel>
#include "shaderparameterinfo.h"

class ShaderParameterInfoBackend;
typedef QHash<QString, ShaderParameterInfoBackend> ShaderParameterMap;

class ShaderModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum ShaderParameterRoles {
        ParameterName = Qt::DisplayRole,
        ParameterType = Qt::ToolTipRole,
        ParameterDatatype = Qt::UserRole,
        ParameterValue = Qt::UserRole + 1,
        ParameterUniformLocation = Qt::UserRole + 2,
        ParameterIsSubroutine = Qt::UserRole + 3,
        ParameterSubroutineValues = Qt::UserRole + 4,
        ParameterData = Qt::UserRole + 5 // This is used for creation of ui elements
    };
    Q_ENUM(ShaderParameterRoles)

    ShaderModel(QObject *parent = nullptr);
    ShaderModel(const ShaderModel &other);

public slots:
    void syncModel(const ShaderParameterMap &parameters);
    QHash<int, QByteArray> roleNames() const;
private:
    QHash<int, QByteArray> m_roleNameMapping;
    ShaderParameterMap m_parameters;
    friend class Shader;
};

Q_DECLARE_METATYPE(ShaderModel)
Q_DECLARE_METATYPE(ShaderModel::ShaderParameterRoles)

class ShaderParameterInfoBackend
{
public:
    QString m_name;
    ShaderParameterInfo::ShaderParameterType m_type;
    ShaderParameterInfo::ShaderParameterDatatype m_datatype;
    QVariant    m_value;
    int         m_uniformLocation;
    bool        m_isSubroutine;
    int         m__subroutineShaderType;
    QStringList m_subroutineValues;
};
#endif // SHADERMODEL_H
