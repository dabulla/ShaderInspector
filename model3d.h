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
    Q_PROPERTY(bool drawPoints READ drawPoints WRITE setDrawPoints NOTIFY drawPointsChanged)
    Q_CLASSINFO("DefaultProperty", "parameters")
public:
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

    bool drawPoints() const;

public slots:
    void setFilename(QString filename);
    void setShader(Shader* shader);
    void updateParameter(const ShaderParameterInfoBackend *parameter, QVariant value);

    void setCamera(Camera* camera);

    void setDrawPoints(bool drawPoints);

signals:
    void filenameChanged(QString filename);
    void loadingFile(QFuture<Vertices*> future);
    void vertexDeclarationChanged(QList<VertexAttribute*> vertexDeclaration);

    void shaderChanged(Shader* shader);

    void cameraChanged(Camera* camera);

    void drawPointsChanged(bool drawPoints);

private:
    void registerParameter(ShaderParameter* p);
    static void appendParameter(QQmlListProperty<ShaderParameter> *list, ShaderParameter* p);

    static int parameterCount(QQmlListProperty<ShaderParameter> *list);

    static ShaderParameter *parameter(QQmlListProperty<ShaderParameter> *list, int index);

    static void clearParameters(QQmlListProperty<ShaderParameter> *list);
    QString m_filename;
    QFuture<Vertices*> m_fileLoading;
    QList<VertexAttribute*> m_vertexDeclaration;
    Shader* m_shader;
    QList<ShaderParameter *> m_parameters;
    QList<QPair<const ShaderParameterInfoBackend *, QVariant>> m_updates;
    friend class Model3DRenderer;
    Camera* m_camera;
    bool m_drawPoints;
};

#endif // MODEL3D_H
