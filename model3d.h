#ifndef MODEL3D_H
#define MODEL3D_H

#include <QObject>
#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QFuture>
#include <QVariant>
#include <QQmlListProperty>
#include "vertexattribute.h"
#include "minimalobjreader.h"
#include "shaderparameter.h"
#include "shader.h"

#include "camera.h"

class Model3D : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QList<VertexAttribute*> vertexDeclaration READ vertexDeclaration NOTIFY vertexDeclarationChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(Shader* shader READ shader WRITE setShader NOTIFY shaderChanged)
    Q_PROPERTY(QQmlListProperty<ShaderParameter> parameters READ parameters)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(PrimitiveType primitiveType READ primitiveType WRITE setPrimitiveType NOTIFY primitiveTypeChanged)
    Q_CLASSINFO("DefaultProperty", "parameters")
public:

    enum PrimitiveType
    {
        Points = GL_POINTS
      , Lines = GL_LINES, LineLoop = GL_LINE_LOOP, LineStrip = GL_LINE_STRIP
      , Triangles = GL_TRIANGLES, TriangleStrip = GL_TRIANGLE_STRIP, TriangleFan = GL_TRIANGLE_FAN
      , LinesAdjacency = GL_LINES_ADJACENCY, TrianglesAdjacency = GL_TRIANGLES_ADJACENCY, LineStripAdjacency = GL_LINE_STRIP_ADJACENCY, TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY
      , Patches = GL_PATCHES
    };
    Q_ENUM(PrimitiveType)

    Model3D();

    // QQuickFramebufferObject interface
public:
    Renderer *createRenderer() const;

    // Model3D properties
public:
    QString filename() const;
    QList<VertexAttribute*> vertexDeclaration() const;
    Shader* shader() const;
    QQmlListProperty<ShaderParameter> parameters();
    Camera* camera() const;
    PrimitiveType primitiveType() const;

public slots:
    void setFilename(QString filename);
    void setShader(Shader* shader);
    void updateParameter(const ShaderParameterInfoBackend *parameter, QVariant value);
    void setCamera(Camera* camera);
    void setPrimitiveType(PrimitiveType primitiveType);

signals:
    void filenameChanged(QString filename);
    void loadingFile(QFuture<Vertices*> future);
    void vertexDeclarationChanged(QList<VertexAttribute*> vertexDeclaration);
    void shaderChanged(Shader* shader);
    void cameraChanged(Camera* camera);
    void primitiveTypeChanged(PrimitiveType primitiveType);

private:
    void registerParameter(ShaderParameter* p);
    void unregisterParameter(ShaderParameter *p);
    void setAllShaderParameters();

    static void appendParameter(QQmlListProperty<ShaderParameter> *list, ShaderParameter* p);
    static int parameterCount(QQmlListProperty<ShaderParameter> *list);
    static ShaderParameter *parameter(QQmlListProperty<ShaderParameter> *list, int index);
    static void clearParameters(QQmlListProperty<ShaderParameter> *list);

    QString m_filename;
    QFuture<Vertices*> m_fileLoading;
    QList<VertexAttribute*> m_vertexDeclaration;
    Shader* m_shader;
    QList<ShaderParameter *> m_parameters;
    QMap<QString, QPair<const ShaderParameterInfoBackend *, QVariant>> m_updates;
    friend class Model3DRenderer;
    Camera* m_camera;
    PrimitiveType m_primitiveType;
    //QSet<QString> m_warnedNonexistenParameters;
};

#endif // MODEL3D_H
