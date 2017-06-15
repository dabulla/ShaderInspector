#ifndef MODEL3DRENDERER_H
#define MODEL3DRENDERER_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QQuickWindow>
#include <QFutureWatcher>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include "minimalobjreader.h"
#include "vertexattribute.h"
#include "shaderparameterinfo.h"
#include "camera.h"
#include "model3d.h"

class ShaderParameterInfoBackend;
class Shader;
class Model3DRenderer : public QObject, public QQuickFramebufferObject::Renderer
{
    Q_OBJECT
public:
    Model3DRenderer();
    virtual ~Model3DRenderer();
    void synchronize(QQuickFramebufferObject *item);
private slots:
    void handleObjectLoaded();
    void loadFile(QFuture<Vertices*> future);
private:
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

    void render();
    void reinitializeFromVertexdata();
    void updateParameter(const ShaderParameterInfoBackend *uniform, const QVariant &value);

    QQuickWindow* m_window;
    QFutureWatcher<Vertices*> m_objectLoadingWatcher;
    Vertices* m_vertices;
    bool m_initializedFromItem;
    QList<VertexAttribute*> m_vertexDeclaration;
    QOpenGLBuffer m_indices;
    quint32 m_vertexCount;
    quint32 m_indexCount;

    Shader *m_tmpShader; //TODO: DBG, TMP
    QOpenGLShaderProgram* m_shader;
    QList<QOpenGLTexture*> m_textures;
    QOpenGLFunctions_4_0_Core *m_funcs;
    //Shader Stage to list of subroutine uniform location to value
    QMap<int, QMap<int, int> > m_subroutines;
    Camera *m_camera;
    Model3D::PrimitiveType m_primitiveType;
    bool m_tesselationEnabled;
    bool m_isInstanced;
    int  m_vertexCountPerPrimitive; // TODO add instancing support
    int  m_primitiveCount;
    bool m_reinitializeFromVertexdata;
    bool m_vertexdataInitialized;
    QOpenGLVertexArrayObject m_vao;

    void updateCameraParameters(Shader *shader);
    void recreateVao();
};

#endif // MODEL3DRENDERER_H
