#include "shadermodel.h"
#include "shaderparameterinfo.h"

ShaderModel::ShaderModel(QObject *parent)
    :QStandardItemModel(parent)
{
    m_roleNameMapping[ParameterName] = "name";
    m_roleNameMapping[ParameterType] = "type";
    m_roleNameMapping[ParameterDatatype] = "datatype";
    m_roleNameMapping[ParameterUniformLocation] = "location";
    m_roleNameMapping[ParameterData] = "data";
    m_roleNameMapping[ParameterFound] = "found";
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
        ShaderParameterInfo *theUniform = new ShaderParameterInfo(this);
        theUniform->setName( param->m_name );
        theUniform->setType( param->m_type );
        theUniform->setDatatype( param->m_datatype );
        theUniform->setUniformLocation( param->m_uniformLocation);
        theUniform->setIsSubroutine( param->m_isSubroutine );
        theUniform->setSubroutineValues( param->m_subroutineValues );
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
        csi->setData(theUniform->found(), ParameterFound);
        csi->setData(QVariant::fromValue(theUniform), ParameterData);
    }
    markFound(this);
}

QHash<int, QByteArray> ShaderModel::roleNames() const
{
    return m_roleNameMapping;
}

void ShaderModel::markFound(QAbstractItemModel *model, QModelIndex parent)
{
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        QString name = model->data(index, ParameterName).toString();
        bool prevFound = model->data(index, ParameterFound).toBool();
        bool found = m_parameters.contains(name);
        if(prevFound != found)
        {
            model->setData(index, found, ParameterFound);
            ShaderParameterInfo *uniform = qvariant_cast<ShaderParameterInfo*>(model->data(index, ParameterData));
            uniform->setFound(found);
            model->setData(index, QVariant::fromValue(uniform), ParameterData);
            // Note: this is a hack to update the whole role
            QVector<int> updatedRoles = {ParameterName, ParameterData, ParameterFound};
            emit dataChanged(index, this->index(r, columnCount()-1, parent), updatedRoles);
        }
        if( model->hasChildren(index) ) {
            markFound(model, index);
        }
    }
}
