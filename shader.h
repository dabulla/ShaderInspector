#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QQuickFramebufferObject>
#include "shaderparameterinfo.h"
#include <QStandardItemModel>

class ShaderModel;
class ShaderParameterInfoBackend;

class Shader : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString vertexShader READ vertexShader WRITE setVertexShader NOTIFY vertexShaderChanged)
    Q_PROPERTY(QString geometryShader READ geometryShader WRITE setGeometryShader NOTIFY geometryShaderChanged)
    Q_PROPERTY(QString tessellationControlShader READ tessellationControlShader WRITE setTessellationControlShader NOTIFY tessellationControlShaderChanged)
    Q_PROPERTY(QString tessellationEvaluationShader READ tessellationEvaluationShader WRITE setTessellationEvaluationShader NOTIFY tessellationEvaluationShaderChanged)
    Q_PROPERTY(QString fragmentShader READ fragmentShader WRITE setFragmentShader NOTIFY fragmentShaderChanged)
    Q_PROPERTY(QString computeShader READ computeShader WRITE setComputeShader NOTIFY computeShaderChanged)
    Q_PROPERTY(QOpenGLShaderProgram* shaderProgram READ shaderProgram NOTIFY shaderProgramChanged)
    Q_PROPERTY(QStandardItemModel* model READ model NOTIFY modelChanged)

public:
    Renderer *createRenderer() const;
    ShaderParameterInfoBackend *parameterInfo(QString name);

    bool tesselationEnabled();
public:
    Shader(QQuickItem *parent = nullptr);
    QString vertexShader() const;
    QString geometryShader() const;
    QString tessellationControlShader() const;
    QString tessellationEvaluationShader() const;
    QString fragmentShader() const;
    QString computeShader() const;
    QOpenGLShaderProgram* shaderProgram() const;
    QStandardItemModel* model() const;

public slots:
    void setVertexShader(QString vertexShader);
    void setGeometryShader(QString geometryShader);
    void setTessellationControlShader(QString tessellationControlShader);
    void setTessellationEvaluationShader(QString tessellationEvaluationShader);
    void setFragmentShader(QString fragmentShader);
    void setComputeShader(QString computeShader);

signals:
    void vertexShaderChanged(QString vertexShader);
    void geometryShaderChanged(QString geometryShader);
    void tessellationControlShaderChanged(QString tessellationControlShader);
    void tessellationEvaluationShaderChanged(QString tessellationEvaluationShader);
    void fragmentShaderChanged(QString fragmentShader);
    void computeShaderChanged(QString computeShader);
    void shaderProgramChanged(QOpenGLShaderProgram* shaderProgram);
    void modelChanged(QStandardItemModel* model);

private:
    QString m_vertexShader;
    QString m_geometryShader;
    QString m_tessellationControlShader;
    QString m_tessellationEvaluationShader;
    QString m_fragmentShader;
    QString m_computeShader;

    // QQuickFramebufferObject interface
    QOpenGLShaderProgram* m_shaderProgram;
    void setShaderProgram(QOpenGLShaderProgram *shaderProgram);

    ShaderModel* m_model;
    friend class ShaderBackend;
};

#endif // SHADER_H
