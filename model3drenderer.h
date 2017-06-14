#ifndef MODEL3DRENDERER_H
#define MODEL3DRENDERER_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QQuickWindow>
#include <QFutureWatcher>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLVertexArrayObject>
#include "minimalobjreader.h"
#include "vertexattribute.h"
#include "logorenderer.h"
#include "shaderparameterinfo.h"
#include "camera.h"

class ShaderParameterInfoBackend;
class Shader;
class Model3DRenderer : public QObject, public QQuickFramebufferObject::Renderer
{
    Q_OBJECT
public:
    LogoRenderer m_logoRenderer;
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
    QMap<GLuint, QImage> m_textureUniforms;
    QOpenGLFunctions_4_0_Core *m_funcs;
    //Shader Stage to list of subroutine values
    QMap<int, QList<int> > m_subroutines;
    Camera *m_camera;
    bool m_drawPoints;
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
