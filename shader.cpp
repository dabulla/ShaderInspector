#include "shader.h"

#include <QOpenGLFunctions_4_0_Core>
#include <QQuickWindow>
#include "shadermodel.h"

class ShaderBackend : public QQuickFramebufferObject::Renderer
{
public:
    ShaderBackend()
        : m_window( nullptr)
        , m_shaderCreated( false )
    {}
    // Renderer interface
protected:
    void render()
    {

    }

    void synchronize(QQuickFramebufferObject *item)
    {
        Shader *modelItem = qobject_cast<Shader *>(item);
        if(!m_window) m_window = modelItem->window();
        bool reload = false;
        if(m_vertexShader != modelItem->vertexShader())
        {
            m_vertexShader = modelItem->vertexShader();
            reload = true;
        }
        if(m_tessellationControlShader != modelItem->tessellationControlShader())
        {
            m_tessellationControlShader = modelItem->tessellationControlShader();
            reload = true;
        }
        if(m_tessellationEvaluationShader != modelItem->tessellationEvaluationShader())
        {
            m_tessellationEvaluationShader = modelItem->tessellationEvaluationShader();
            reload = true;
        }
        if(m_geometryShader != modelItem->geometryShader())
        {
            m_geometryShader = modelItem->geometryShader();
            reload = true;
        }
        if(m_fragmentShader != modelItem->fragmentShader())
        {
            m_fragmentShader = modelItem->fragmentShader();
            reload = true;
        }
        if(m_computeShader != modelItem->computeShader())
        {
            m_computeShader = modelItem->computeShader();
            reload = true;
        }
        if(reload || modelItem->m_reloadScheduled)
        {
            reloadShader();
            modelItem->setShaderProgram( &m_shaderProgram );
            QMetaObject::invokeMethod(modelItem->m_model, "syncModel", Qt::QueuedConnection, Q_ARG(ShaderParameterMap, m_parameters));
            modelItem->emitShaderUpdated();
            modelItem->m_reloadScheduled = false;
        }
    }

private:

    void reloadShader()
    {
        if(m_shaderCreated)
        {
            m_shaderProgram.removeAllShaders();
        }
        else
        {
            m_shaderProgram.create();
            m_shaderCreated = true;
        }
        if ( !m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertexShader ) ) {
            qCritical() << QObject::tr( "Could not compile vertex shader. Log:" ) << m_shaderProgram.log();
            return;
        }
        if ( !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Fragment, m_fragmentShader ) ) {
            qCritical() << QObject::tr( "Could not compile fragment shader. Log:" ) << m_shaderProgram.log();
            return;
        }
        if(m_geometryShader.size() != 0) {
            if ( !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Geometry, m_geometryShader ) ) {
                qCritical() << QObject::tr( "Could not compile geometry shader. Log:" ) << m_shaderProgram.log();
                return;
            }
        }
        if(m_tessellationControlShader.size() != 0) {
            if ( !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::TessellationControl, m_tessellationControlShader ) ) {
                qCritical() << QObject::tr( "Could not compile vertex tessellation control. Log:" ) << m_shaderProgram.log();
                return;
            }
        }
        if(m_tessellationEvaluationShader.size() != 0) {
            if ( !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::TessellationEvaluation, m_tessellationEvaluationShader ) ) {
                qCritical() << QObject::tr( "Could not compile vertex tessellation evaluation. Log:" ) << m_shaderProgram.log();
                return;
            }
        }
        if(m_computeShader.size() != 0) {
            if ( !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Compute, m_computeShader ) ) {
                qCritical() << QObject::tr( "Could not compile compute shader. Log:" ) << m_shaderProgram.log();
                return;
            }
        }
        QOpenGLFunctions_4_0_Core *glFuncs = m_window->openglContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();
        if ( !glFuncs ) {
            qWarning() << "Unsupported Profile: QOpenGLFunctions_4_0_Core";
            return;
        }
        if ( !glFuncs->initializeOpenGLFunctions() ) {
            qWarning() << "Could not initialize gl functions";
            return;
        }
        if(!m_shaderProgram.link())
        {
            qWarning() << "Could not link shader program";
        }
        m_shaderProgram.bind();
        QList<QString> allFoundUniforms;
        GLint total = -1;
        glFuncs->glGetProgramiv( m_shaderProgram.programId(), GL_ACTIVE_UNIFORMS, &total );
        for(int i=0; i<total; ++i)
        {
            int name_len=-1, num=-1;
            GLenum type = GL_ZERO;
            char nameBuff[200];
            glFuncs->glGetActiveUniform( m_shaderProgram.programId(), GLuint(i), sizeof(nameBuff)-1,
                &name_len, &num, &type, nameBuff );
            nameBuff[name_len] = 0;
            GLuint location = glFuncs->glGetUniformLocation( m_shaderProgram.programId(), nameBuff );
            Q_ASSERT(location == i);
            QString name(nameBuff);
            if(m_blacklist.contains(name))
            {
                continue;
            }
            allFoundUniforms.append(name);
            ShaderParameterInfoBackend *theUniform;
            if(m_parameters.contains(name))
            {
                theUniform = &m_parameters[name];
            }
            else
            {
                m_parameters.insert(name, ShaderParameterInfoBackend() );
                theUniform = &m_parameters[name];
            }
            theUniform->m_name = name;
            theUniform->m_type = ShaderParameterInfo::Uniform;
            theUniform->m_datatype = static_cast<ShaderParameterInfo::ShaderParameterDatatype>(type);
            theUniform->m_uniformLocation = location;
            theUniform->m_isSubroutine = false;
        }
        /// Subroutines are special...
        /// Subroutines can be different for each shader stage
        total = -1;
        int shaderStages[] = {GL_VERTEX_SHADER,
                              GL_TESS_CONTROL_SHADER,
                              GL_TESS_EVALUATION_SHADER,
                              GL_GEOMETRY_SHADER,
                              GL_FRAGMENT_SHADER,
                              GL_COMPUTE_SHADER};
        for(unsigned int shaderStageIndex=0 ; shaderStageIndex<sizeof(shaderStages)/sizeof(shaderStages[0]) ; ++shaderStageIndex)
        {
            int shaderStage = shaderStages[shaderStageIndex];

            QList<QString> subroutineNames;

            glFuncs->glGetProgramStageiv(m_shaderProgram.programId(), shaderStage, GL_ACTIVE_SUBROUTINES, &total);
            for(int i=0; i<total; ++i)
            {
                int name_len=-1;
                char nameBuff[200];
                glFuncs->glGetActiveSubroutineName(m_shaderProgram.programId(), shaderStage, GLuint(i), 200, &name_len, nameBuff);
                nameBuff[name_len] = 0;
                subroutineNames.append( nameBuff );
            }

            //m_qmlShaderProperties.m_subroutines[ shaderStage ].clear();
            glFuncs->glGetProgramStageiv(m_shaderProgram.programId(), shaderStage, GL_ACTIVE_SUBROUTINE_UNIFORMS, &total);
            for(int i=0; i<total; ++i)
            {
                int name_len=-1;
                char nameBuff[200];
                GLint num_subroutines;
                int subroutines[100];
                glFuncs->glGetActiveSubroutineUniformiv(m_shaderProgram.programId(), shaderStage, GLuint(i), GL_NUM_COMPATIBLE_SUBROUTINES, &num_subroutines);
                glFuncs->glGetActiveSubroutineUniformiv(m_shaderProgram.programId(), shaderStage, GLuint(i), GL_COMPATIBLE_SUBROUTINES, subroutines);
                glFuncs->glGetActiveSubroutineUniformName(m_shaderProgram.programId(), shaderStage, GLuint(i), 200, &name_len, nameBuff);
                nameBuff[name_len] = 0;
                QString name(nameBuff);
                if(m_blacklist.contains(name))
                {
                    continue;
                }
                allFoundUniforms.append(name);
                ShaderParameterInfoBackend *theUniform;
                if(m_parameters.contains(name))
                {
                    theUniform = &m_parameters[name];
                }
                else
                {
                    m_parameters.insert(name, ShaderParameterInfoBackend() );
                    theUniform = &m_parameters[name];
                }

                theUniform->m_name = name;
                theUniform->m_type = ShaderParameterInfo::Uniform;
                theUniform->m_datatype = static_cast<ShaderParameterInfo::ShaderParameterDatatype>(ShaderParameterInfo::UNSIGNED_INT);
                theUniform->m_uniformLocation = i;
                theUniform->m_isSubroutine = true;
                theUniform->m__subroutineShaderType = shaderStage;
                theUniform->m_subroutineValues.clear();
                for( int idx=0 ; idx < num_subroutines ; ++idx)
                {
                    theUniform->m_subroutineValues.append(subroutineNames.at(subroutines[idx]));
                }
            }
        }
        QMutableHashIterator<QString, ShaderParameterInfoBackend> iter(m_parameters);
        while (iter.hasNext()) {
            if (!allFoundUniforms.contains(iter.next()->m_name))
                iter.remove();
        }
    }

    QOpenGLShaderProgram m_shaderProgram;
    QQuickWindow *m_window;
    QStringList m_blacklist;
    ShaderParameterMap m_parameters;
    QString m_vertexShader;
    QString m_geometryShader;
    QString m_tessellationControlShader;
    QString m_tessellationEvaluationShader;
    QString m_fragmentShader;
    QString m_computeShader;
    bool m_shaderCreated;
};

Shader::Shader(QQuickItem *parent)
    : QQuickFramebufferObject( parent )
    , m_shaderProgram( nullptr )
    , m_reloadScheduled( false )
{
    qRegisterMetaType<QOpenGLShaderProgram*>("ShaderProgram");
    qRegisterMetaType<QHash<QString, ShaderParameterInfoBackend> >("ShaderParameterMap");

    m_model = new ShaderModel(this);
}

void Shader::reload()
{
    m_reloadScheduled = true;
    update();
}

void Shader::emitShaderUpdated()
{
    emit shaderUpdated();
}

bool Shader::isReloading()
{
    return m_reloadScheduled;
}

QString Shader::vertexShader() const
{
    return m_vertexShader;
}

QString Shader::geometryShader() const
{
    return m_geometryShader;
}

QString Shader::tessellationControlShader() const
{
    return m_tessellationControlShader;
}

QString Shader::tessellationEvaluationShader() const
{
    return m_tessellationEvaluationShader;
}

QString Shader::fragmentShader() const
{
    return m_fragmentShader;
}

QString Shader::computeShader() const
{
    return m_computeShader;
}

void Shader::setVertexShader(QString vertexShader)
{
    if (m_vertexShader == vertexShader)
        return;

    m_vertexShader = vertexShader;
    emit vertexShaderChanged(m_vertexShader);
}

void Shader::setGeometryShader(QString geometryShader)
{
    if (m_geometryShader == geometryShader)
        return;

    m_geometryShader = geometryShader;
    emit geometryShaderChanged(m_geometryShader);
}

void Shader::setTessellationControlShader(QString tessellationControlShader)
{
    if (m_tessellationControlShader == tessellationControlShader)
        return;

    m_tessellationControlShader = tessellationControlShader;
    emit tessellationControlShaderChanged(m_tessellationControlShader);
}

void Shader::setTessellationEvaluationShader(QString tessellationEvaluationShader)
{
    if (m_tessellationEvaluationShader == tessellationEvaluationShader)
        return;

    m_tessellationEvaluationShader = tessellationEvaluationShader;
    emit tessellationEvaluationShaderChanged(m_tessellationEvaluationShader);
}

void Shader::setFragmentShader(QString fragmentShader)
{
    if (m_fragmentShader == fragmentShader)
        return;

    m_fragmentShader = fragmentShader;
    emit fragmentShaderChanged(m_fragmentShader);
}

void Shader::setComputeShader(QString computeShader)
{
    if (m_computeShader == computeShader)
        return;

    m_computeShader = computeShader;
    emit computeShaderChanged(m_computeShader);
}

void Shader::setShaderProgram(QOpenGLShaderProgram *shaderProgram)
{
    // Shader may has changed without changing the object
//    if (m_shaderProgram == shaderProgram)
//        return;

    m_shaderProgram = shaderProgram;
    emit shaderProgramChanged(m_shaderProgram);
}

QQuickFramebufferObject::Renderer *Shader::createRenderer() const
{
    return new ShaderBackend();
}

ShaderParameterInfoBackend *Shader::parameterInfo(QString name)
{
    if(m_model->m_parameters.contains(name))
    {
        return &m_model->m_parameters[name];
    }
    else
    {
        //qDebug() << "Tried to set non existent parameter";
        return nullptr;
    }

}

bool Shader::tesselationEnabled()
{
    return !m_tessellationControlShader.isEmpty() || !m_tessellationEvaluationShader.isEmpty();
}

QStandardItemModel *Shader::model() const
{
    return m_model;
}

QOpenGLShaderProgram *Shader::shaderProgram() const
{
    return m_shaderProgram;
}
