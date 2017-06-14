#include "shadermodel.h"

#include "shaderparameterinfo.h"

ShaderModel::ShaderModel(QObject *parent)
    :QStandardItemModel(parent)
{
    m_roleNameMapping[ParameterName] = "name";
    m_roleNameMapping[ParameterType] = "type";
    m_roleNameMapping[ParameterDatatype] = "datatype";
    m_roleNameMapping[ParameterValue] = "value";
    m_roleNameMapping[ParameterUniformLocation] = "location";
    m_roleNameMapping[ParameterData] = "data";
}

ShaderModel::ShaderModel(const ShaderModel &other)
    : m_roleNameMapping(other.m_roleNameMapping)
{

}

void ShaderModel::syncModel(const ShaderParameterMap &parameters)
{
    m_parameters = parameters;
    for( auto param = parameters.cbegin(); param != parameters.cend(); ++param )
    {
        QStandardItem *csi;
        bool found = false;
        for(int i=0 ; i<rowCount() ; ++i)
        {
            csi = item(i, 0);
            if(csi->data(ParameterName).toString() == param->m_name)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            csi = new QStandardItem();
            this->appendRow( csi );
        }
        csi->setData(param->m_name, ParameterName);
        csi->setData(QVariant::fromValue(param->m_type), ParameterType);
        csi->setData(QVariant::fromValue(param->m_datatype), ParameterDatatype);
        csi->setData(param->m_uniformLocation, ParameterUniformLocation);
        csi->setData(param->m_isSubroutine, ParameterIsSubroutine);
        csi->setData(param->m_subroutineValues, ParameterSubroutineValues);
        ShaderParameterInfo *theUniform = new ShaderParameterInfo(this);
        theUniform->setName( param->m_name );
        theUniform->setType( param->m_type );
        theUniform->setDatatype( param->m_datatype );
        theUniform->setUniformLocation( param->m_uniformLocation);
        theUniform->setIsSubroutine( param->m_isSubroutine );
        theUniform->m_subroutineValues = param->m_subroutineValues;
        csi->setData(QVariant::fromValue(theUniform), ParameterData);
    }
}

QHash<int, QByteArray> ShaderModel::roleNames() const
{
    return m_roleNameMapping;
}
