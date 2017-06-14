#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <QObject>
#include <QVariant>

class Model3D;

class ShaderParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    ShaderParameter(QObject *parent = nullptr);
    QString name() const;
    QVariant value() const;

public slots:
    void setName(QString name);
    void setValue(QVariant value);

signals:
    void nameChanged(QString name);
    void valueChanged(QVariant value);

private:
    QString m_name;
    QVariant m_value;
};

#endif // SHADERPARAMETER_H
