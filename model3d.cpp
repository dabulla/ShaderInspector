#include "model3d.h"
#include "model3drenderer.h"
#include <QVector3D>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QtConcurrent/QtConcurrent>

Model3D::Model3D()
    : m_shader( nullptr )
    , m_camera( nullptr )
{
    qRegisterMetaType<Vertices*>("Vertices");
}

QQuickFramebufferObject::Renderer *Model3D::createRenderer() const
{
    return new Model3DRenderer();
}

QString Model3D::filename() const
{
    return m_filename;
}

QList<VertexAttribute *> Model3D::vertexDeclaration() const
{
    return m_vertexDeclaration;
}

Shader *Model3D::shader() const
{
    return m_shader;
}

QQmlListProperty<ShaderParameter> Model3D::parameters()
{
    return QQmlListProperty<ShaderParameter>(this, &m_parameters,
                                             &Model3D::appendParameter,
                                             &Model3D::parameterCount,
                                             &Model3D::parameter,
                                             &Model3D::clearParameters);
}

Camera *Model3D::camera() const
{
    return m_camera;
}

bool Model3D::drawPoints() const
{
    return m_drawPoints;
}

void Model3D::setFilename(QString filename)
{
    if (m_filename == filename)
        return;

    m_filename = filename;
    m_fileLoading = QtConcurrent::run(&MinimalObjReader::readFile, filename, new Vertices);
    emit loadingFile(m_fileLoading);
    emit filenameChanged(m_filename);
}

void Model3D::setShader(Shader *shader)
{
    if (m_shader == shader)
        return;

    m_shader = shader;
    for(auto param = m_parameters.cbegin(); param != m_parameters.cend(); ++param)
    {
        registerParameter(*param);
    }
    emit shaderChanged(m_shader);
}

void Model3D::updateParameter(const ShaderParameterInfoBackend* parameter, QVariant value)
{
    // parameters must be communicated to the backend
    // backend will process this list in the syncronize() method
    m_updates.append(QPair<const ShaderParameterInfoBackend*, QVariant>(parameter, value));
    update();
}

void Model3D::setCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    emit cameraChanged(m_camera);
}

void Model3D::setDrawPoints(bool drawPoints)
{
    if (m_drawPoints == drawPoints)
        return;

    m_drawPoints = drawPoints;
    emit drawPointsChanged(m_drawPoints);
}

// TODO: unregister
void Model3D::registerParameter(ShaderParameter *p)
{
    if(this->m_shader->parameterInfo(p->name()) == nullptr)
    {
        qDebug() << "Parameter" << p->name() << "is not part of the current shader.";
    }
    connect(p, &ShaderParameter::valueChanged, [this, p]()
    {
        ShaderParameterInfoBackend* paramInfo = this->m_shader->parameterInfo(p->name());
        if(paramInfo == nullptr)
        {
            qDebug() << "Parameter" << p->name() << "changed, but is not part of the current shader.";
        }
        else
        {
            this->updateParameter( paramInfo, p->value());
        }
    });
}

void Model3D::appendParameter(QQmlListProperty<ShaderParameter> *list, ShaderParameter *p)
{
    Model3D* that = qobject_cast<Model3D*>(list->object);
    if(that->m_shader)
    {
        that->registerParameter(p);
    }
    that->m_parameters.append(p);
}

int Model3D::parameterCount(QQmlListProperty<ShaderParameter> *list)
{
    Model3D* that = qobject_cast<Model3D*>(list->object);
    return that->m_parameters.count();
}

ShaderParameter *Model3D::parameter(QQmlListProperty<ShaderParameter> *list, int index)
{
    Model3D* that = qobject_cast<Model3D*>(list->object);
    return that->m_parameters.at(index);
}

void Model3D::clearParameters(QQmlListProperty<ShaderParameter> *list)
{
    Model3D* that = qobject_cast<Model3D*>(list->object);
    return that->m_parameters.clear();
}
